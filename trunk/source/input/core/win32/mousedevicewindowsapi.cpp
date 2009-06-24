#include"mousedevicewindowsapi.h"
#include<winuser.h>


#if !defined(WM_MOUSEWHEEL)
	#define WM_MOUSEWHEEL  (0x020A)	//!<	マウスホイールが回ったときに投げられるメッセージ
#endif



namespace Maid{ namespace Input {

	/*!
	 	@class	MouseDeviceEventMessage mousedeviceeventmessage.h
	 	@brief	マウスドライバ
	 */ 

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
MouseDeviceWindowsAPI::MouseDeviceWindowsAPI( const Window& hWnd ) : m_Window(hWnd)
{
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! マウスドライバの初期化
/*!
 	引数などは IMouseDevice::Initialize を参照すること
 */
void MouseDeviceWindowsAPI::Initialize()
{
	for( int i=0; i<3; ++i ) { m_Position.push_back(0); }
	for( int i=0; i<3; ++i ) { m_IsDown.push_back(false); }

	AddProc( m_Window.GetHWND() );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! マウスドライバの解放
/*!
 *	引数などは IMouseDevice::Finalize を参照すること
 */
void MouseDeviceWindowsAPI::Finalize()
{

}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 各軸の座標を取得する
/*!
 	引数などは IMouseDevice::GetPosition を参照すること
 */
void MouseDeviceWindowsAPI::GetState( std::vector<int>& PosList, std::vector<bool>& KeyList ) const
{
	POINT pos;

	::GetCursorPos( &pos );
	::ScreenToClient( m_Window.GetHWND(), &pos );

	PosList = m_Position;
	PosList[0] = pos.x;
	PosList[1] = pos.y;

	const int key[] = { VK_LBUTTON, VK_RBUTTON, VK_MBUTTON };
	for( int i=0; i<NUMELEMENTS(key); ++i )
	{
		const_cast<MouseDeviceWindowsAPI*>(this)->m_IsDown[i] = IsFlag(::GetAsyncKeyState(key[i]),0x8000);
	}

	KeyList = m_IsDown;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! このドライバで識別できる軸の数を取得する
/*!
 	引数などは IMouseDevice::GetPositionMax を参照すること
 */
int	MouseDeviceWindowsAPI::GetPositionMax()	const
{
	return (int)m_Position.size();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! このドライバで識別できるボタンの数を取得する
/*!
 	引数などは IMouseDevice::GetButtonMax を参照すること
 */
int	MouseDeviceWindowsAPI::GetButtonMax()	const
{
	return (int)m_IsDown.size();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! プロシージャにコールバックしてもらう関数
/*!
 	引数などは IMouseDevice::OnMessage を参照すること
 */
MessageHook::RETURNCODE MouseDeviceWindowsAPI::OnMessage( WindowsMessage& Param )
{
	const UINT	 msg = Param.GetMSG();
	const WPARAM wParam = Param.GetWPARAM();
	const LPARAM lParam = Param.GetLPARAM();

	switch( msg )
	{
	case WM_MOUSEWHEEL:
		{
			m_Position[2] += (short)HIWORD( wParam );
		}break;
	}

	return MessageHook::RETURNCODE_DEFAULT;
}

}}