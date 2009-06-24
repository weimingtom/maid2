#include"exception.h"


namespace Maid
{
    /*!
        @class  Exception Exception.h
        @brief  例外クラス
     */
  Exception::ExceptionString::ExceptionString( const char* szFile, int Line ) 
    : m_FileName(szFile), m_Line(Line), std::ostream(&m_StreamBuf)
  {
  }
  std::string Exception::ExceptionString::GetLog() const
  {
    const std::string& t = m_StreamBuf.GetBuffer();
    const std::string& f = m_FileName;
    const int Line = m_Line;

    char buf[1024];
    ::sprintf( buf, "例外発生: %s %d行目 %s",t.c_str(), Line, f.c_str() );

    return buf;
  }

  Exception::Exception()
  {
  }

  Exception::Exception( const ExceptionString& log )
  {
    m_Log = log.GetLog();
  }

  void Exception::AddLog( const std::string& str )
  {
    m_Log += "\n";
    m_Log += str;
  }

  const std::string& Exception::GetLog()const
  {
    return m_Log;
  }


  void Exception::WriteLog()
  {
    const std::string& t = m_Log;

    FILE* hFile = ::fopen( "warning.txt", "a" );
    ::fprintf( hFile, "%s\n",t.c_str() );
    ::fclose( hFile );
  }



}