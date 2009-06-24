#include"trace.h"
#include"../string.h"
#include"../../config/Win32.h"
#include"../thread.h"

namespace Maid
{

  /*!
    @class	Trace trace.h
    @brief	警告用クラス
   */

  bool Trace::s_IsCreateConsole = false;

  Trace::Trace( const char* szFile, int Line ) 
    : m_FileName(szFile), m_Line(Line), std::ostream(&m_StreamBuf)
  {
  }

  //!	ログの記録
  /*!	
  */
  void Trace::OnAlert()
  {
    static ThreadMutex  s_Douki;
    ThreadMutexLocker Lock(s_Douki);

    if( !s_IsCreateConsole )
    {
      AllocConsole();
      s_IsCreateConsole = true;
    }
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
    char Text[1024];
    ::sprintf( Text, "%s %d行目 %s\n", text.c_str(), m_Line, m_FileName.c_str() );

    const HANDLE hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);
    const std::wstring str = String::ConvertSJIStoUNICODE(Text).c_str();

    ::WriteConsole( hConsole, str.c_str(), str.length(), NULL, NULL );
  }
}
