/*!
    @file
    @brief  DLL 操作のラッパクラス
 */

#ifndef maid2_framework_win32_dllwrapper_h
#define maid2_framework_win32_dllwrapper_h

#include"../../config/define.h"
#include"../../config/win32.h"
#include"../../auxiliary/string.h"


namespace Maid
{
  class DllWrapper
  {
  public:
    DllWrapper();
    ~DllWrapper();

    enum LOADRETURN
    {
      LOADRETURN_SUCCESS, //  読み込みに成功した
      LOADRETURN_ERROR,   //  読み込みに失敗した
    };

    LOADRETURN	Load( const String& DllName );
    FARPROC	GetProcAddress( const String& FunctionName ) const;

    void Free();

    static bool IsExist( const String& DllName );

    HMODULE GetModule() const;

    bool Loaded() const;

  private:
    String	m_DllName;    //!<	読み込んだＤＬＬの名前
    HMODULE m_hDll;       //!<	読みこんだＤＬＬのハンドル
  };
}

#endif
