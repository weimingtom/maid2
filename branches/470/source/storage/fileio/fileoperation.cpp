#include"fileoperation.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/memorybuffer.h"
#include"../../import/md5/md5.h"
#include<io.h>

#include<direct.h>

namespace Maid
{
  namespace FileOperation
  {
    bool IsFileExist( const String& FileName )
    {
      return ::access(String::ConvertMAIDtoSJIS(FileName).c_str(),0)==0;
    }



    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ディレクトリの作成
    /*!
        階層がある場合でも一括して作成できます

        @param  DirectoryPath	[i ]	作成するディレクトリ名

        @exception Exception ディレクトリの作成に失敗した場合
     */
    void CreateDirectory( const String& DirectoryPath )
    {
      std::vector<String> dat;
      CreateDirectory( DirectoryPath, dat );
    }

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ディレクトリの作成
    /*!
        階層がある場合でも一括して作成できます

        @param  DirectoryPath [i ]  作成するディレクトリ名
        @param  CreateList    [ o]  作成したディレクトリ

        @exception Exception ディレクトリの作成に失敗した場合
     */
    void CreateDirectory( const String& DirectoryPath, std::vector<String>& CreateList )
    {
      String src = DirectoryPath;

      if( src[src.length()-1]!=String::CODE_BACKSLASH && src[src.length()-1]!=String::CODE_SLASH )
      {
        src += String::CODE_BACKSLASH;
      }


      for( int i=0; i<(int)src.length(); ++i )
      {
        const unt32 c = src[i];

        if( c==String::CODE_BACKSLASH || c==String::CODE_SLASH )
        {
          const String m_path = src.substr(0,i);
          const std::string path = String::ConvertMAIDtoSJIS(m_path);

          if (::access(path.c_str(),0)!=0)
          {
            if (::mkdir(path.c_str())!=0) 
            {
              MAID_WARNING(MAIDTEXT("ディレクトリ") + m_path + MAIDTEXT("の作成に失敗")); 
            }

            CreateList.push_back(m_path);
          }
        }
      }
    }


    std::vector<unt08>  CalcMD5( IFileRead& hFile )
    {
      const size_t READSIZE = 1*1024*1024;

      MemoryBuffer buf;

      buf.Resize( READSIZE );

      MD5LIB::MD5Context context;

      MD5LIB::MD5Init( &context );

      size_t nokori_size = hFile.GetSize();

      while( true )
      {
        if( nokori_size < READSIZE )
        {
          hFile.Read( buf.GetPointer(0), nokori_size );
          MD5LIB::MD5Update( &context, (const unsigned char*)buf.GetPointer(0), nokori_size );
          break;

        }else
        {
          hFile.Read( buf.GetPointer(0), buf.GetSize() );
          MD5LIB::MD5Update( &context, (const unsigned char*)buf.GetPointer(0), buf.GetSize() );

          nokori_size -= buf.GetSize();
        }
      }

      std::vector<unt08> ret(16);

      MD5LIB::MD5Final( &ret[0], &context );

      return ret;
    }











  }
}

