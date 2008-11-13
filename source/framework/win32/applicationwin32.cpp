#include"applicationwin32.h"

#include"shell.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/exception.h"
#include"../../storage/storage.h"
#include<shellapi.h>

namespace Maid
{

static const wchar_t* DEFAULT_CLASSNAME = L"Maid_application";

Application* GlobalPointer<Application>::s_pPointer;

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 	  @param	hInstance		[i ]	WinMain() から渡された引数
 	  @param	hPrevInstance	[i ]	WinMain() から渡された引数
 	  @param	lpCmdLine		[i ]	WinMain() から渡された引数
 	  @param	nCmdShow		[i ]	WinMain() から渡された引数
 */
Application::Application( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	m_Instance = hInstance;
	m_PrevInstance = hPrevInstance;
	m_CommandShow = nCmdShow;
  
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
Application::~Application()
{
}

HINSTANCE		Application::GetHINSTANCE()	const { return m_Instance; }
const String	Application::GetClassName()	const { return String::ConvertUNICODEtoMAID(DEFAULT_CLASSNAME); }
const String&	Application::GetCmdLineALL()	const { return m_CommandLine; }
const std::vector<String>&	Application::GetCmdLine()const { return m_DivComdList; }

String	Application::GetCmdLine( int no )const
{
	if( !(no<(int)m_DivComdList.size()) ) { return String(); }

	return m_DivComdList[no];
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! アプリが生きているか？
/*!
	詳しくは IApplication::IsValid() を参照
 */
bool Application::IsValid()
{
	MSG Msg;

	while( ::PeekMessage( &Msg, NULL, 0, 0, PM_REMOVE )!=0 )
	{
		if( Msg.message == WM_QUIT ){ return false;	}
		::TranslateMessage( &Msg );
		::DispatchMessage( &Msg );
	}

	return true;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! アプリケーションを終了させる
/*!
    詳しくは IApplication::ApplicationExit() を参照
 */
void Application::LocalExit()
{
	::PostQuitMessage(0);
}

void Application::OnSetup()
{
  Shell::CoInitialize();
	Shell::InitCommonControls();

	{	//	初期設定
		String ExePath = Shell::GetExeFileName();
		String ExeDir  = String::GetDirectory(ExePath);

		Shell::SetCurrentDirectory( ExeDir );
    m_HookManager.Initialize();
	//	CRand::SetSeed(::GetTickCount());

	}

  m_CommandLine = String::ConvertUNICODEtoMAID(GetCommandLine());

  {	//	コマンドラインの判別
    int    nArgs;
    LPTSTR *lplpszArgs;

    lplpszArgs = CommandLineToArgvW(GetCommandLine(), &nArgs);

    for (int i = 0; i < nArgs; i++) 
    {
      m_DivComdList.push_back(String::ConvertUNICODEtoMAID(lplpszArgs[i])); 
    }

    LocalFree(lplpszArgs);
  }

	{	//	プロセスの登録
		WNDCLASSEX		wc = {0};
		wc.cbSize	    = sizeof(WNDCLASSEX);
		wc.style      = CS_VREDRAW | CS_HREDRAW; 
		wc.lpfnWndProc	= ProcessProc; 
		wc.hInstance		= m_Instance;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.hIcon        = ::LoadIcon( m_Instance,IDI_APPLICATION); 
		wc.hCursor      = ::LoadCursor( NULL, IDC_ARROW ); 
		wc.hbrBackground	= (HBRUSH)::GetStockObject(BLACK_BRUSH);
		wc.lpszMenuName		= NULL; 
		wc.lpszClassName	= DEFAULT_CLASSNAME;
		 
		wc.hIconSm			= NULL;

		if( ::RegisterClassEx(&wc)==0 ) 
		{
			MAID_THROWEXCEPTION(MAIDTEXT("RegisterClassEx に失敗"));
		}
	}
}


void Application::OnLoop()
{

}

void Application::OnCleanup()
{
	Shell::CoUninitialize();
}


LRESULT CALLBACK Application::ProcessProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	WindowsMessage mess(hWnd,msg,wParam,lParam);
  return GlobalPointer<ApplicationWin32>::Get()->m_HookManager.OnMessage( mess );
}

}