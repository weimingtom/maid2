#include"assert.h"
#include"../string.h"

#include"../../config/Win32.h"
#include"../thread.h"


namespace Maid
{

  /*!
    @class  assert assert.h
    @brief  アサート関数
   */
  Assert::Assert( const char* szFile, int Line ) 
    : m_FileName(szFile), m_Line(Line), std::ostream(&m_StreamBuf)
  {
  }

  //! assertの発生
  /*!
      @return デバッガを起動させるなら true
              させないなら false
  */
  bool Assert::OnAlert()
  {
    static ThreadMutex  s_Douki;
    ThreadMutexLocker Lock(s_Douki);
    std::string text = m_StreamBuf.GetBuffer();

    if( !text.empty() )
    {
      int len = (int)text.length();
      //  末尾に \n がついてたらはずす
      if( text[len-1]=='\n' )
      {
        text.resize(len-1);
      }
    }

    char Text[1024];
    ::sprintf( Text, "エラーが起きました\n"
              "ファイル名%s\n"
              "行番号%d\n"
              "内容：%s\n"
              "デバッガに入るときは「はい」をクリックしてください"
              ,m_FileName.c_str(), m_Line, text.c_str()  );

    ::ShowCursor(TRUE);
    const int ret = ::MessageBox( NULL, String::ConvertSJIStoUNICODE(Text).c_str(), L"assertion", MB_ICONSTOP|MB_YESNO|MB_DEFBUTTON2 );
    ::ShowCursor(FALSE);

    return ret==IDYES;
  }
}
