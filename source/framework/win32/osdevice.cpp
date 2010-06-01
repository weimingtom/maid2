#include"osdevice.h"

#include"shell.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/exception.h"
#include"../../auxiliary/mathematics.h"

#include<shellapi.h>

namespace Maid
{


static const wchar_t* DEFAULT_CLASSNAME = L"Maid_application";

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 起動パラメーターの設定
/*!
 	  @param	hInstance		[i ]	WinMain() から渡された引数
 	  @param	hPrevInstance	[i ]	WinMain() から渡された引数
 	  @param	lpCmdLine		[i ]	WinMain() から渡された引数
 	  @param	nCmdShow		[i ]	WinMain() から渡された引数
 */
void OSDevice::SetParam( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  m_Instance = hInstance;
  m_PrevInstance = hPrevInstance;
  m_CommandShow = nCmdShow;
}


HINSTANCE		OSDevice::GetHINSTANCE()	const { return m_Instance; }
const String	OSDevice::GetClassName()	const { return String::ConvertUNICODEtoMAID(DEFAULT_CLASSNAME); }
const String&	OSDevice::GetCmdLineALL()	const { return m_CommandLine; }
const std::vector<String>&	OSDevice::GetCmdLine()const { return m_DivComdList; }

String	OSDevice::GetCmdLine( int no )const
{
	if( !(no<(int)m_DivComdList.size()) ) { return String(); }

	return m_DivComdList[no];
}


void OSDevice::Initialize()
{
  Set();

  Shell::CoInitialize();
	Shell::InitCommonControls();

  D3DXMathInitialize();


	{	//	初期設定
		String ExePath = Shell::GetExeFileName();
		String ExeDir  = String::GetDirectory(ExePath);

		Shell::SetCurrentDirectory( ExeDir );
    m_HookManager.Initialize();
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

void OSDevice::Finalize()
{
  D3DXMathFinalize();
	Shell::CoUninitialize();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! アプリが生きているか？
/*!
    詳しくは OSDriver::IsValid() を参照
 */
bool OSDevice::IsValid()
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
    詳しくは OSDriver::ApplicationExit() を参照
 */
void OSDevice::Exit()
{
	::PostQuitMessage(0);
}


LRESULT CALLBACK OSDevice::ProcessProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	WindowsMessage mess(hWnd,msg,wParam,lParam);
  return GlobalPointer<OSDevice>::Get()->m_HookManager.OnMessage( mess );
}

}
