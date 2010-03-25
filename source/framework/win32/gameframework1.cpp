#include"gameframework.h"

#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/trace.h"
#include"../../auxiliary/exception.h"
#include"../../auxiliary/collision.h"



namespace Maid
{

GameFrameWork::GameFrameWork( const SPGAMETHREAD& pGame )
  :IGameFrameWork(pGame)
  ,m_GraphicsDeviceList(m_Window)
  ,m_InputDeviceList(m_Window)
  ,m_SoundDeviceList(m_Window)
  ,m_SysytemCursorDraw(true)
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 実行
/*!
 	  @param	hInstance		  [i ]	WinMain() から渡された引数
 	  @param	hPrevInstance	[i ]	WinMain() から渡された引数
 	  @param	lpCmdLine		  [i ]	WinMain() から渡された引数
 	  @param	nCmdShow		  [i ]	WinMain() から渡された引数
 */
int GameFrameWork::Run( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  //  メモリリークのチェック
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF|_CRTDBG_ALLOC_MEM_DF|_CRTDBG_CHECK_ALWAYS_DF);


  m_OSDevice.SetParam( hInstance,hPrevInstance,lpCmdLine,nCmdShow );

  return IApplication::Run();
}

IOSDevice& GameFrameWork::GetOSDevice() { return m_OSDevice; }

Graphics::IDeviceList& GameFrameWork::GetGraphicsDeviceList(){ return m_GraphicsDeviceList; }
Input::IDeviceList& GameFrameWork::GetInputDeviceList() { return m_InputDeviceList; }
Sound::IDeviceList& GameFrameWork::GetSoundDeviceList() { return m_SoundDeviceList; }

void GameFrameWork::LocalInitialize(const INITIALIZEPARAM& Conf)
{
  m_SysytemCursorDraw = Conf.SystemCursorDraw;
  m_ShowCursorCount = 0;

	{
		Window& win = m_Window;

		win.RegistCallBack( WM_ACTIVATE,        MakeCallBackObject( &GameFrameWork::wmActivate, this) );
		win.RegistCallBack( WM_ACTIVATEAPP,        MakeCallBackObject( &GameFrameWork::wmActivateApp, this) );
		win.RegistCallBack( WM_NCLBUTTONDBLCLK, MakeCallBackObject( &GameFrameWork::wmNcLButtonDblClk, this) );
		win.RegistCallBack( WM_SYSCOMMAND,      MakeCallBackObject( &GameFrameWork::wmSysCommand, this) );
		win.RegistCallBack( WM_KEYDOWN,         MakeCallBackObject( &GameFrameWork::wmKeyDown, this) );
		win.RegistCallBack( WM_SYSKEYDOWN,      MakeCallBackObject( &GameFrameWork::wmSysKeyDown, this) );
		win.RegistCallBack( WM_PAINT,           MakeCallBackObject( &GameFrameWork::wmPaint, this) );
		win.RegistCallBack( WM_CLOSE,           MakeCallBackObject( &GameFrameWork::wmClose, this) );
		win.RegistCallBack( WM_DISPLAYCHANGE,   MakeCallBackObject( &GameFrameWork::wmDisplayChange, this) );
		win.RegistCallBack( WM_IME_SETCONTEXT,  MakeCallBackObject( &GameFrameWork::wmImeSetContext, this) );

    const DWORD style = WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_DLGFRAME | WS_CAPTION;
//    const DWORD style = WS_OVERLAPPEDWINDOW;
//    win.Create( Conf.WindowCaption, POINT2DI(0,0), SIZE2DI(0,0), style );
    win.Create( Conf.WindowCaption, POINT2DI(CW_USEDEFAULT,CW_USEDEFAULT), SIZE2DI(0,0), style );
    win.SetClientSize( Conf.WindowSize );

		{
			HICON hIcon = LoadIcon();
			if( hIcon!=NULL )
			{
				::SendMessage( win.GetHWND(), WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)hIcon );
			}
		}

		win.Show();

		::SetForegroundWindow( win.GetHWND() );
	}
}


void GameFrameWork::LocalLoop()
{
	HardWareCursorCheck();
}

void GameFrameWork::LocalFinalize()
{

}

HICON	GameFrameWork::LoadIcon()
{
	return NULL; 
}

void GameFrameWork::HardWareCursorCheck()
{
  if( m_SysytemCursorDraw ) { return ; }
  POINT pos;

  GetCursorPos( &pos );
  ScreenToClient( m_Window.GetHWND(), &pos );

	RECT2DI ClientRect;
	{
		RECT rc;
		::GetClientRect( m_Window.GetHWND(), &rc );

		ClientRect.x = 0;
		ClientRect.y = 0;
		ClientRect.w = rc.right - rc.left;
		ClientRect.h = rc.bottom - rc.top;
	}

	const bool IsWindowIn = Collision<int>::IsPointToRect( pos.x, pos.y,  ClientRect );

	if( IsWindowIn )
	{
		if( 0<=m_ShowCursorCount ) { m_ShowCursorCount = ShowCursor(FALSE); }
	}else
	{
		if( m_ShowCursorCount<0 ) { m_ShowCursorCount = ShowCursor(TRUE); }
	}
}


}
