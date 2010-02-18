#include"warning.h"
#include"../string.h"
#include"../thread.h"

namespace Maid
{

Warning::FUNCTION Warning::s_Function[5];

  /*!
    @class	warning warning.h
    @brief	警告用クラス
   */
  Warning::Warning( const char* szFile, int Line ) 
    : m_FileName(szFile), m_Line(Line), std::ostream(&m_StreamBuf)
  {
  }

  //!	警告の発生
  /*!	
 	    @param	level	[i ]	警告レベル。基本的に任意
  */
  void Warning::OnAlert( int level )
  {
    static ThreadMutex  s_Douki;
    ThreadMutexLocker Lock(s_Douki);
    std::string text = m_StreamBuf.GetBuffer();

    if( !text.empty() )
    {
      int len = (int)text.length();
      //	末尾に \n がついてたらはずす
      if( text[len-1]=='\n' )
      {
        text.resize(len-1);
      }
    }

    std::string line;
    {
      char buf[256];
      sprintf( buf, "%d", m_Line );
      line = buf;
    }

    const std::string output = text + " " + line + "行目 " + m_FileName + "\n";

    if( !(level<NUMELEMENTS(s_Function)) || !(s_Function[level]) )
    {
 		  FILE* hFile = ::fopen( "warning.txt", "a" );
		  ::fprintf( hFile, "%s", output.c_str() );
		  ::fclose( hFile );
    }else
    {
      s_Function[level]( output );
    }
  }

  //!	警告を発生させる関数の設定
  /*!	
 	    @param	level	[i ]	設定する警告レベル。
 	    @param	func	[i ]	呼び出す関数。
  */
  void Warning::SetAlertFunction( int level, const FUNCTION& func )
  {
    if( !(level<NUMELEMENTS(s_Function)) )
    {
      MAID_WARNING( "範囲外です" );
      return ;
    }
    s_Function[level] = func;

  }

}
