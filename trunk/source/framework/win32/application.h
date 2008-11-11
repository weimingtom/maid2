/*!
    @file
    @brief Windows用フレームワークベースクラス
 */

#ifndef framework_win32_application_h
#define framework_win32_application_h

#include"../../config/define.h"
#include"../../config/Win32.h"
#include"../../auxiliary/string.h"
#include"../../auxiliary/globalpointer.h"

#include"messagehookmanager.h"
#include<vector>

namespace Maid
{
  class Application : public GlobalPointer<Application>
	{
	public:
		Application();
		virtual ~Application();


		int Run( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );

		HINSTANCE		GetHINSTANCE()	const;
		const String	GetClassName()	const;
		const String&	GetCmdLineALL()	const;
		const std::vector<String>&	GetCmdLine()const;
		String	GetCmdLine( int no )const;


	protected:
		void ApplicationExit();
		virtual void OnSetup()=0;
		virtual void OnLoop()=0;
		virtual void OnCleanup()=0;


	private:
		bool IsValid();
		static LRESULT CALLBACK ProcessProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

		HINSTANCE	m_Instance;
		HINSTANCE	m_PrevInstance;
		String		m_CommandLine;
		int			  m_CommandShow;

		std::vector<String>	m_DivComdList;	//	分割されたコマンドライン

		MessageHookManager	m_HookManager;

		int	m_ApplicationExitCode;
	};
}
#endif
