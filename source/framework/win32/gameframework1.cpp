#include"gameframework.h"

#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/trace.h"
#include"../../auxiliary/exception.h"



namespace Maid
{

GameFrameWork::GameFrameWork( const SPGAMETHREAD& pGame )
  :IGameFrameWork(pGame)
{
}

int GameFrameWork::Run( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  SetOSDriver( SPOSDRIVER(new OSDriverWin32(hInstance,hPrevInstance,lpCmdLine,nCmdShow)) );
  SetVideoDeviceList( SPVIDEODEVICELIST(new VideoDeviceList(m_Window)) );

  return IApplication::Run();
}


void GameFrameWork::LocalSetup(const IGameThread::DEFAULTCONFIG& Conf)
{
	{
		Window& win = m_Window;

		win.RegistCallBack( WM_ACTIVATE,        MakeCallBackObject( &GameFrameWork::wmActivate, this) );
		win.RegistCallBack( WM_NCLBUTTONDBLCLK, MakeCallBackObject( &GameFrameWork::wmNcLButtonDblClk, this) );
		win.RegistCallBack( WM_SYSCOMMAND,      MakeCallBackObject( &GameFrameWork::wmSysCommand, this) );
		win.RegistCallBack( WM_KEYDOWN,         MakeCallBackObject( &GameFrameWork::wmKeyDown, this) );
		win.RegistCallBack( WM_SYSKEYDOWN,      MakeCallBackObject( &GameFrameWork::wmSysKeyDown, this) );
		win.RegistCallBack( WM_PAINT,           MakeCallBackObject( &GameFrameWork::wmPaint, this) );
		win.RegistCallBack( WM_CLOSE,           MakeCallBackObject( &GameFrameWork::wmClose, this) );

		win.SetSize( Conf.Graphic.ScreenSize );
		win.SetCaption( Conf.WindowCaption );
    win.SetStyle( WS_OVERLAPPEDWINDOW );
		
    win.Create();

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
/*
	SPTIMER pTimer( new CTimer_timeGetTime() );

	{
		const APPCONFIG::GRAPHIC& conf = Conf.Graphic;
		IVideoCardDriver::SCREENMODE m;

		m.Mode = (IVideoCardDriver::SCREENMODE::TYPE)conf.Screen;
		m.Size = conf.Size;
		m.RefreshRate  = conf.RefreshRate;
		m.IsSoftTnL    = conf.IsSoftTnL;
		m.IsWaitVSync  = conf.IsWaitVSync;
		m.IsAspectLock = conf.IsAspectLock;

		SPVIDEOCARDDRIVER pDriver( new CVideoCardDriverD3D9(m_Window.GetHWND()) );

		m_GraphicCore.Initialize( pDriver, m );
	}

	{
		SPSOUNDCARDDRIVER	pDriver( new CSoundCardDriverDS8(m_Window.GetHWND()) );

		ISoundCardDriver::PCMBUFFERFORMAT fmt;

		fmt.BitsPerSample	= Conf.Sound.BitsPerSample;
		fmt.Channels		= Conf.Sound.Channels;
		fmt.SamplesPerSec	= Conf.Sound.SamplesPerSec;

		m_SoundCore.Initialize( pDriver, pTimer, fmt );
	}

	{
		CInputDriverWindowsMSG driver(m_Window.GetHWND());

		m_pKeybordDriver  = driver.CreateKeybordDriver();
		m_pMouseDriver    = driver.CreateMouseDriver();
		m_pCharCodeDriver = driver.CreateCharCodeDriver();

		m_pKeybordDriver->Initialize();
		m_pMouseDriver->Initialize();

		{
			m_pCharCodeDriver->Initialize();
			m_pCharCodeDriver->SetCallBack(MakeCharCodeFunctionObject(&CSingleWindowAppWindows::OnCharCode,this) );
		}

		{
			CInputMethodDriverWindows* p = new CInputMethodDriverWindows;
			p->Initialize( m_Window.GetHWND() );

			m_pIMEDriver.reset( p );
			m_pIMEDriver->SetResultCodeFunction( MakeResultCodeFunctionObject(&CSingleWindowAppWindows::OnIMEResult,this) );
		}
	}
	m_IsFrameUpdating = false;
	m_Initialize = true;
	::SetEvent(m_hGameEvent);
*/
}


void GameFrameWork::LocalLoop()
{
	HardWareCursorCheck();

}

void GameFrameWork::LocalCleanup()
{
/*
	::CloseHandle(m_hAppEvent);
	::CloseHandle(m_hGameEvent);
*/
}

HICON	GameFrameWork::LoadIcon()
{
	return NULL; 
}

void GameFrameWork::HardWareCursorCheck()
{
/*
	MySTL::vector<int> pos;
	MySTL::vector<bool> key;

	m_pMouseDriver->GetState( pos, key );

	RECT2DI ClientRect;
	{
		RECT rc;
		::GetClientRect( m_Window.GetHWND(), &rc );

		ClientRect.x = 0;
		ClientRect.y = 0;
		ClientRect.w = rc.right - rc.left;
		ClientRect.h = rc.bottom - rc.top;
	}

	const bool IsWindowIn = Collision<int>::IsPointToRect( pos[0], pos[1],  ClientRect );
	const bool IsShow = m_pMouseDriver->IsCursorShow();

	if( IsWindowIn )
	{
		if( IsShow ) { m_pMouseDriver->SetDrawHardWareCursor(false); }
	}else
	{
		if( !IsShow ) { m_pMouseDriver->SetDrawHardWareCursor(true); }
	}
*/
}

void IGameFrameWork::PostWindowMessage( const SPOSMESSAGE& pMess )
{
	ThreadMutexLocker Lock(m_MessageMutex);

	m_OSMessageQue.push_back( pMess );
}

}
