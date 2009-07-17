#include"packfilecreater.h"
#include"packfileinfo.h"
#include"fileio/filewrite.h"
#include"fileio/filereadnormal.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/compress/compresszlib.h"
#include"../auxiliary/memorybuffer.h"
#include"../import/md5/md5.h"

#include<map>


namespace Maid
{

void PackFileCreater::BeginArchive( const String& ArchiveName, unt64 DivSize, const String& RootDirectory, const std::vector<FILEINFO>& FileInfo )
{
  m_ArchiveName = ArchiveName;
  m_RootDirectory = RootDirectory;
  m_FileInfo = FileInfo;

  m_DivSize = DivSize;


  AddExecutingText( MAIDTEXT("処理の準備中") );

  m_Thread.SetFunc( MakeThreadObject(&PackFileCreater::ThreadFunction,this) );
  m_Thread.Execute();

}

void PackFileCreater::PopStatus( std::list<STATUS>& st )
{
  ThreadMutexLocker lock(m_StatusMutex);

  st.splice( st.end(), m_StatusList );
}

void   PackFileCreater::OnCancel()
{

}

void PackFileCreater::AddExecutingText( const String& text )
{
  ThreadMutexLocker lock(m_StatusMutex);

  STATUS st;

  st.Text  = text;
  st.State = STATUS::STATE_EXECUTING;

  m_StatusList.push_back(st);
}

void PackFileCreater::AddErrorText( const String& text )
{
  ThreadMutexLocker lock(m_StatusMutex);

  STATUS st;

  st.Text  = text;
  st.State = STATUS::STATE_ERROR;

  m_StatusList.push_back(st);
}




std::string PackFileCreater::CalcMD5( const std::vector<unt08>& src ) const
{
  MD5LIB::MD5_CTX context;

  MD5LIB::MD5Init( &context );
  MD5LIB::MD5Update( &context, &(src[0]), src.size() );

  unsigned char digest[16];

  MD5LIB::MD5Final( digest, &context );

  std::string ret;

  for( unt32 i=0; i<NUMELEMENTS(digest); ++i )
  {
	  char buf[32];
	  sprintf( buf, "%02x", digest[i] );

	  ret += buf;
  }

  return ret;
}

unt PackFileCreater::ThreadFunction(volatile ThreadController::BRIGEDATA& brige)
{
  const String tmpfile = MAIDTEXT("_tmp1.dat");
  const String elementfile = MAIDTEXT("_tmp2.dat");

  //  作業フォルダにコピー＆圧縮＆統合する
  MemoryBuffer  ElementInfo;

  unt64 ArchiveSize = 0;
  unt64 HeaderSize = 0;

  std::map<std::string,std::wstring>  MD5Index; //  map[md5]==FileName となるインデックス

  {
    //  FileWrite を使う方法があるんだけど、アーカイブファイルが２Ｇ越えると動作しなそうなので
    //  地味に行う
    FILE* hDstFile = NULL;

    hDstFile = ::fopen( String::ConvertMAIDtoSJIS(tmpfile).c_str(), "wb" );
    if( hDstFile==NULL ) { AddErrorText(MAIDTEXT("ファイル作成に失敗")); return 0; }

    for( int i=0; i<(int)m_FileInfo.size(); ++i )
    {
      const FILEINFO& srcinfo = m_FileInfo[i];

      std::vector<unt08>  SrcFileImage; //  ファイルをメモリ上に置いておく場所
      std::vector<unt08>  DstFileImage; //  書き込むイメージ
      FileReadNormal hSrcFile( m_RootDirectory + srcinfo.SourceFile );

      const IFileRead::OPENRESULT ret =  hSrcFile.Open();
      if( ret!=IFileRead::OPENRESULT_OK ) { AddErrorText(MAIDTEXT("ファイルオープンに失敗")); return 0; }

      SrcFileImage.resize( hSrcFile.GetSize() );
      hSrcFile.Read( &(SrcFileImage[0]), hSrcFile.GetSize() );

      const std::string MD5Value = CalcMD5(SrcFileImage);

      std::map<std::string,std::wstring>::const_iterator ite = MD5Index.find(MD5Value);
      if( ite!=MD5Index.end() )
      { //  同じファイルがあったら、エイリアスの作成だけにしておく
        const std::wstring aliasname  = String::ConvertMAIDtoUNICODE(srcinfo.SourceFile);
        const std::wstring targetname = ite->second;

        const unt32 Elementsize = sizeof(PACKFILE::ELEMENTHEADER) + aliasname.length()*sizeof(wchar_t);
        const unt32 OldSize = ElementInfo.GetSize();

        ElementInfo.Resize( OldSize+Elementsize );

        unt08* pBegin = (unt08*)ElementInfo.GetPointer(OldSize);
        PACKFILE::ELEMENTHEADER* pInfo = (PACKFILE::ELEMENTHEADER*)pBegin;

        ZERO( pInfo, Elementsize );

        memcpy( pInfo->Info.Alias.Target, targetname.c_str(), targetname.length()*sizeof(wchar_t) );

        pInfo->Type       = PACKFILE::ELEMENTHEADER::TYPE_ALIAS;
        pInfo->StructSize = Elementsize;
        ::memcpy( pBegin + sizeof(PACKFILE::ELEMENTHEADER), aliasname.c_str(), aliasname.length()*sizeof(wchar_t) );

        AddExecutingText( srcinfo.SourceFile + MAIDTEXT("はエイリアス化されました") );

      }else
      { //  ない場合は書き出す
        DstFileImage.resize( hSrcFile.GetSize() );

        PACKFILE::COMPRESSTYPE CompType = PACKFILE::COMPRESSTYPE_NONE;

        if( srcinfo.IsCompress )
        {
          CompressZLIB comp;
          const size_t size = comp.Encode( &(SrcFileImage[0]), SrcFileImage.size(), &(DstFileImage[0]), DstFileImage.size() );

          //  圧縮したのに膨らんでしまったら、キャンセル
          if( SrcFileImage.size() <= size )
          {
            AddExecutingText( srcinfo.SourceFile + MAIDTEXT("は圧縮に失敗したので無圧縮で固めます") );
            DstFileImage = SrcFileImage;
            CompType = PACKFILE::COMPRESSTYPE_NONE;
          }else
          {
            AddExecutingText( srcinfo.SourceFile + MAIDTEXT("は圧縮しました") );
            DstFileImage.resize( size );
            CompType = PACKFILE::COMPRESSTYPE_ZLIB;
          }
        }else
        {
          AddExecutingText( srcinfo.SourceFile + MAIDTEXT("は無圧縮で固めます") );
          DstFileImage = SrcFileImage;
          CompType = PACKFILE::COMPRESSTYPE_NONE;
        }

        //  ファイル情報を記録
        {
          std::wstring str = String::ConvertMAIDtoUNICODE(srcinfo.SourceFile);

          const unt32 Elementsize = sizeof(PACKFILE::ELEMENTHEADER) + str.length()*sizeof(wchar_t);
          const unt32 OldSize = ElementInfo.GetSize();

          ElementInfo.Resize( OldSize+Elementsize );

          unt08* pBegin = (unt08*)ElementInfo.GetPointer(OldSize);
          PACKFILE::ELEMENTHEADER* pInfo = (PACKFILE::ELEMENTHEADER*)pBegin;

          ZERO( pInfo, Elementsize );

          pInfo->Info.File.CompressSize = DstFileImage.size();
          pInfo->Info.File.CompressType = CompType;
          pInfo->Info.File.Position = ArchiveSize;
          pInfo->Info.File.FileSize = SrcFileImage.size();

          pInfo->Type       = PACKFILE::ELEMENTHEADER::TYPE_FILE;
          pInfo->StructSize = Elementsize;
          ::memcpy( pBegin + sizeof(PACKFILE::ELEMENTHEADER), str.c_str(), str.length()*sizeof(wchar_t) );

          {
            //  ＭＤ５テーブルにも記録
            MD5Index[MD5Value] = str;
          }
        }

        //  書きんで次へ
        fwrite( &(DstFileImage[0]), DstFileImage.size(), 1, hDstFile );

        //  4byte境界にそろえる
        const int tmp = 4-(DstFileImage.size()&3);
        const unt08 tmpdat[] = { 0x00,0x00,0x00,0x00 };
        if( tmp!=0 )
        {
          fwrite( tmpdat, tmp, 1, hDstFile );
        }

        ArchiveSize += DstFileImage.size() + tmp;
      }
    }

    fclose(hDstFile);
  }


  {
    AddExecutingText( MAIDTEXT("ヘッダの作成中") );
    //  ヘッダの圧縮&ファイルに書き込む
    const unt DivCount = (sizeof(PACKFILE::HEADER) + (unt64)ElementInfo.GetSize() + ArchiveSize) / m_DivSize +1;
    const MemoryBuffer& SrcFileImage = ElementInfo;
    MemoryBuffer  DstFileImage; //  書き込むイメージ

    DstFileImage.Resize( SrcFileImage.GetSize() );

    PACKFILE::COMPRESSTYPE CompType = PACKFILE::COMPRESSTYPE_NONE;
    {
      CompressZLIB comp;
      const size_t size = comp.Encode( SrcFileImage.GetPointer(0), SrcFileImage.GetSize(), DstFileImage.GetPointer(0), DstFileImage.GetSize() );

      //  圧縮したのに膨らんでしまったら、キャンセル
      if( SrcFileImage.GetSize() <= size )
      {
        ::memcpy( DstFileImage.GetPointer(0), SrcFileImage.GetPointer(0), DstFileImage.GetSize() );
        CompType = PACKFILE::COMPRESSTYPE_NONE;
      }else
      {
        DstFileImage.Resize( size );
        CompType = PACKFILE::COMPRESSTYPE_ZLIB;
      }
    }

    {
      FILE* hFile = NULL;

      hFile = fopen( String::ConvertMAIDtoSJIS(elementfile).c_str(), "wb" );
      if( hFile==NULL ) { AddErrorText(MAIDTEXT("ファイル作成に失敗")); return 0; }

      PACKFILE::HEADER Header;

      const unt64 HeaderAndIndexSize = sizeof(Header) + DstFileImage.GetSize();
      const unt64 arcoff = 4-(HeaderAndIndexSize&3);

      ZERO( &Header, sizeof(Header) );
      Header.FileCode = StrToBinary32('M','P','F','2');
      Header.Version  = PACKFILE::HEADER::VERSION_0x0100;
      Header.InfoSizeComp = DstFileImage.GetSize();
      Header.InfoSize     = SrcFileImage.GetSize();
      Header.CompressType = CompType;
      Header.ArchiveOffset= HeaderAndIndexSize+arcoff;
      Header.DiveCount = DivCount;

      fwrite( &Header, sizeof(Header), 1, hFile );
      fwrite( DstFileImage.GetPointer(0), DstFileImage.GetSize(), 1, hFile );
      if( arcoff!=0 )
      {
        const unt08 tmpdat[] = { 0xDE,0xAD,0xBE,0xEF };
        fwrite( tmpdat, (size_t)arcoff, 1, hFile );
      }
      fclose( hFile );

      HeaderSize = Header.ArchiveOffset;
    }
  }


  AddExecutingText( MAIDTEXT("ファイルの分割中") );

  //  ファイルの分割
  FILE* hHeader = fopen( String::ConvertMAIDtoSJIS(elementfile).c_str(), "rb" );
  FILE* hArchive = fopen( String::ConvertMAIDtoSJIS(tmpfile).c_str(), "rb" );

  FILE* hFileList[] = { hHeader, hArchive };
  int pos = 0;

  const unt64 READMAX = 10*1024*1024; //  一回に読み書きする最大量は１０Ｍ
  unt64 WritedSize = 0;  //  現在書き込んだbyte数


  int FileCount = 0;  //  作ったファイル数

  FILE* hTargetFile = CreateArchiveFile(FileCount);
  if( hTargetFile==NULL ) { AddErrorText(MAIDTEXT("ファイル作成に失敗")); return 0; }

  while( true )
  {
    MemoryBuffer  ReadWork;
    ReadWork.Resize(READMAX);

    FILE* hFile = hFileList[pos];

    const size_t readsize = fread( ReadWork.GetPointer(0), 1, ReadWork.GetSize(), hFile );

    if( readsize<READMAX )
    {
      ++pos;

      ReadWork.Resize(readsize);
    }

    //  下のwhile()ループ内で書き込んだbyte数
    unt64 onmemorysize = 0;
    while( true )
    {
      if( onmemorysize==ReadWork.GetSize() ) { break; }

      const unt64 nextwrite = ReadWork.GetSize() - onmemorysize;
      const unt08* pSrc = (const unt08*)ReadWork.GetPointer(onmemorysize);

      if( WritedSize+nextwrite < m_DivSize )
      {
        fwrite( pSrc, 1, nextwrite, hTargetFile );

        onmemorysize += nextwrite;
        WritedSize   += nextwrite;
      }else
      {
        const unt64 write1 = m_DivSize-WritedSize;

        fwrite( pSrc, 1, (unt32)write1, hTargetFile );
        fclose(hTargetFile);

        ++FileCount;
        hTargetFile = CreateArchiveFile(FileCount);
        if( hTargetFile==NULL ) { AddErrorText(MAIDTEXT("ファイル作成に失敗")); return 0; }

        onmemorysize += write1;
        WritedSize    = 0;
      }
    }

    if( feof(hArchive)!=0 ) { break; }
  }

  fclose(hHeader);
  fclose(hArchive);
  fclose(hTargetFile);


  {
    ThreadMutexLocker lock(m_StatusMutex);
    STATUS st;
    st.Text  = MAIDTEXT("終了しました");
    st.State = STATUS::STATE_SUCCESS;
    m_StatusList.push_back( st );
  }
  return 0;
}

FILE* PackFileCreater::CreateArchiveFile( int count )
{
  const String FileName = PACKFILE::MakeArchiveFileName(m_ArchiveName,count);
  const std::string tmp = String::ConvertMAIDtoSJIS(FileName);

  FILE* hTargetFile = fopen( String::ConvertMAIDtoSJIS(FileName).c_str(), "wb" );

  return hTargetFile;
}

}