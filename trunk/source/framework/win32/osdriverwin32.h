/*!
    @file
    @brief Windows用フレームワークベースクラス
 */

#ifndef framework_win32_osdriverwin32_h
#define framework_win32_osdriverwin32_h

#include"../../config/define.h"
#include"../../config/Win32.h"

#include<vector>

#include"../iosdriver.h"

#include"../../auxiliary/string.h"
#include"messagehookmanager.h"

namespace Maid
{
  class OSDriverWin32 : public IOSDriver,  public GlobalPointer<OSDriverWin32>
  {
  public:
    OSDriverWin32( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );

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

		const HINSTANCE	m_Instance;
		const HINSTANCE	m_PrevInstance;
		const int			  m_CommandShow;
		String	      	m_CommandLine;

		std::vector<String>	m_DivComdList;	//	分割されたコマンドライン

		MessageHookManager	m_HookManager;

  };

}
#endif
