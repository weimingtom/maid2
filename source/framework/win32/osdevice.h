/*!
    @file
    @brief Windows用フレームワークベースクラス
 */

#ifndef maid2_framework_win32_osdevice_h
#define maid2_framework_win32_osdevice_h

#include"../../config/define.h"
#include"../../config/Win32.h"

#include<vector>

#include"../iosdevice.h"

#include"../../auxiliary/string.h"
#include"messagehookmanager.h"
#include"dllwrapper.h"

namespace Maid
{
  class OSDevice : public IOSDevice,  public GlobalPointer<OSDevice>
  {
  public:
    void SetParam( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );

    virtual void Initialize();
    virtual void Finalize();
    virtual bool IsValid();
    virtual void Exit();


    HINSTANCE		GetHINSTANCE()	const;
    const String	GetClassName()	const;
    const String&	GetCmdLineALL()	const;
    const std::vector<String>&	GetCmdLine()const;
    String	GetCmdLine( int no )const;

  private:
    static LRESULT CALLBACK ProcessProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

    HINSTANCE	m_Instance;
    HINSTANCE	m_PrevInstance;
    int			  m_CommandShow;
    String	  m_CommandLine;
    DllWrapper m_d3dxdll;

    std::vector<String>	m_DivComdList;	//	分割されたコマンドライン

    MessageHookManager	m_HookManager;
  };
}
#endif
