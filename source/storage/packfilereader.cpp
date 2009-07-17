#include"packfilereader.h"
#include"fileio/filereadnormal.h"
#include"fileio/filereadfiles.h"
#include"fileio/filereadmemory.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/memorybuffer.h"
#include"../auxiliary/compress/compresszlib.h"


namespace Maid
{

  void PackFileReader::Mount( const String& FileName )
  {
    ARCHIVEDATA arcdata;

    //  ヘッダをよむ

    FileReadNormal hFile(FileName);

    const FileReadNormal::OPENRESULT  ret = hFile.Open();
    if( ret!=FileReadNormal::OPENRESULT_OK )
    {
      MAID_WARNING( MAIDTEXT("オープンに失敗 ")<<FileName ); 
      return ; 
    }

    hFile.Read( &arcdata.Header, sizeof(arcdata.Header) );

    const PACKFILE::HEADER& head = arcdata.Header;
    if( head.FileCode!=StrToBinary32('M','P','F','2') )
    {
      MAID_WARNING( MAIDTEXT("ヘッダがおかしいです ") << head.FileCode << FileName ); 
      return ; 
    }

    if( head.Version!=PACKFILE::HEADER::VERSION_0x0100 )
    {
      MAID_WARNING( MAIDTEXT("バージョンが合いません ") << head.Version << FileName ); 
      return ; 
    }

    MemoryBuffer ElementBuffer;
    {
      MemoryBuffer src;
      src.Resize( head.InfoSizeComp );
      ElementBuffer.Resize( head.InfoSize );

      hFile.Read( src.GetPointer(0), head.InfoSizeComp );
      CompressZLIB comp;
      const size_t size = comp.Decode( src.GetPointer(0), src.GetSize(), ElementBuffer.GetPointer(0), ElementBuffer.GetSize() );

      if( size!=head.InfoSize )
      {
        MAID_WARNING( MAIDTEXT("展開サイズが合いません ") << size << FileName ); 
        return ; 
      }
    }

    {
      //ElementBuffer に生データが入ったので、検索しやすいように arcdata.Data にコピーする
      const unt08* pSrc = (unt08*)ElementBuffer.GetPointer(0);
      unt32 ElementInfoSize = head.InfoSize;  //  まだ読み取ってない長さ
      while( true )
      {
        if( ElementInfoSize==0 ) { break; }
        const PACKFILE::ELEMENTHEADER* pinfo = (PACKFILE::ELEMENTHEADER*)pSrc;

        const int strbyte = pinfo->StructSize - sizeof(PACKFILE::ELEMENTHEADER);

        const std::wstring str( (wchar_t*)(pSrc + sizeof(PACKFILE::ELEMENTHEADER)), strbyte/sizeof(wchar_t) );
        const String FileName = String::ConvertUNICODEtoMAID(str);

        switch( pinfo->Type )
        {
        case PACKFILE::ELEMENTHEADER::TYPE_FILE:
          {
            arcdata.FileInfo[FileName] = pinfo->Info.File;
          }break;

        case PACKFILE::ELEMENTHEADER::TYPE_ALIAS:
          {
            arcdata.AliasInfo[FileName] = pinfo->Info.Alias;
          }break;
        }



        pSrc += pinfo->StructSize;
        ElementInfoSize -= pinfo->StructSize;
      }
    }

    for( int i=0; i<arcdata.Header.DiveCount; ++i )
    {
      FileReadNormal hFile( PACKFILE::MakeArchiveFileName(FileName,i) );

      const FileReadNormal::OPENRESULT  ret = hFile.Open();
      if( ret!=FileReadNormal::OPENRESULT_OK )
      {
        MAID_WARNING( MAIDTEXT("オープンに失敗 ")<<FileName ); 
        return ; 
      }

      arcdata.ArchiveSize.push_back( hFile.GetSize() );
    }

    arcdata.FileName = FileName;
    ArchiveList.push_back(arcdata);
  }

  void PackFileReader::UnMount( const String& FileName )
  {
    for( ARCHIVELIST::iterator ite=ArchiveList.begin(); ite!=ArchiveList.end(); ++ite )
    {
      if( ite->FileName==FileName )
      {
        ArchiveList.erase(ite);
        return ;
      }
    }
  }

  bool PackFileReader::IsExist( const String& FileName )const
  {
    for( ARCHIVELIST::const_reverse_iterator arc=ArchiveList.rbegin(); arc!=ArchiveList.rend(); ++arc )
    {
      {
        ARCHIVEDATA::FILEINFOLIST::const_iterator ite = arc->FileInfo.find(FileName);
        if( ite!=arc->FileInfo.end() ) { return true; }
      }

      {
        ARCHIVEDATA::ALIASINFOLIST::const_iterator ite = arc->AliasInfo.find(FileName);
        if( ite!=arc->AliasInfo.end() ) { return true; }
      }
    }

    return false;
  }


  SPFILEREAD PackFileReader::FindFile( const String& FileName )const
  {
    for( ARCHIVELIST::const_reverse_iterator arc=ArchiveList.rbegin(); arc!=ArchiveList.rend(); ++arc )
    {
      ARCHIVEDATA::FILEINFOLIST::const_iterator ite = arc->FileInfo.find(FileName);

      if( ite==arc->FileInfo.end() ) 
      {
        //  ファイルが見つからない==エイリアスかも
        //  エイリアスならターゲット先のファイル名を使う

        ARCHIVEDATA::ALIASINFOLIST::const_iterator tmp = arc->AliasInfo.find(FileName);

        if( tmp==arc->AliasInfo.end() ) { continue; }

        ite = arc->FileInfo.find(String::ConvertUNICODEtoMAID(tmp->second.Target));

        if( ite==arc->FileInfo.end() )  { continue; }
      }

      const PACKFILE::FILEINFO& info = ite->second;

      //  まずはファイルデータのある位置までスキップ
      unt64 NowPos = arc->Header.ArchiveOffset + info.Position;

      int beginfileno = 0;
      for( ; beginfileno<(int)arc->ArchiveSize.size(); ++beginfileno )
      {
        if( NowPos < arc->ArchiveSize[beginfileno] ) { break; }

        NowPos -= arc->ArchiveSize[beginfileno];
      }

      std::vector<FileReadFiles::ELEMENT> FileImageElement;

      unt32 read_begin  = (unt32)NowPos;
      unt32 nokori_size = info.CompressSize;

      while( true )
      {
        if( arc->ArchiveSize[beginfileno]-read_begin < nokori_size )
        {
          const unt32 len = (unt32)(arc->ArchiveSize[beginfileno]-read_begin);

          FileImageElement.push_back( 
            FileReadFiles::ELEMENT(
              PACKFILE::MakeArchiveFileName(arc->FileName,beginfileno),
              read_begin,
              len
              )
            );

          read_begin = 0;
          ++beginfileno;

          nokori_size -= len;
        }else
        {
          FileImageElement.push_back( 
            FileReadFiles::ELEMENT(
              PACKFILE::MakeArchiveFileName(arc->FileName,beginfileno),
              read_begin,
              nokori_size
              )
            );
          break;
        }
      }

      SPFILEREAD pRet;

      switch( info.CompressType )
      {
      case PACKFILE::COMPRESSTYPE_NONE:
        {
          pRet.reset( new FileReadFiles(FileImageElement) );
        }break;

      case PACKFILE::COMPRESSTYPE_ZLIB:
        {
          MemoryBuffer  Src;
          Src.Resize( info.CompressSize );
          {
            FileReadFiles hSrcFile(FileImageElement);

            hSrcFile.Open();
            hSrcFile.Read( Src.GetPointer(0), info.CompressSize );
          }

          boost::shared_array<unt08>  pDst( new unt08[info.FileSize] );

          CompressZLIB comp;
          const size_t size = comp.Decode( Src.GetPointer(0), Src.GetSize(), pDst.get(), info.FileSize );

          if( size!=info.FileSize )
          {
            MAID_WARNING( "展開サイズがあいません " << size );
          }

          pRet.reset( new FileReadMemory(pDst,info.FileSize) );

        }break;
      }

      if( pRet.get()!=NULL ) { return pRet; }
    }

    return SPFILEREAD();
  }
}