/*!
 	@file
 	@brief マウスドライバ（プロシージャ で取得する Ver. )
 */

#ifndef maid2_input_core_win32_mousedevicewindowsapi_h
#define maid2_input_core_win32_mousedevicewindowsapi_h

#include"../../../config/define.h"
#include"../../../config/win32.h"
#include"../../../frameWork/win32/messagehook.h"
#include"../../../framework/win32/window.h"
#include"../imousedevice.h"
#include<vector>

namespace Maid{ namespace Input {

	class MouseDeviceWindowsAPI
    :public IMouseDevice
    ,public MessageHook
	{
	public:
		MouseDeviceWindowsAPI( const Window& hWnd );

		virtual void	Initialize();
		virtual void	Finalize();
		virtual void	GetState( std::vector<int>& PosList, std::vector<bool>& KeyList ) const;
		virtual int		GetPositionMax()								const;
		virtual int		GetButtonMax()									const;

		virtual RETURNCODE OnMessage( WindowsMessage& mess );

	private:
		std::vector<int>	m_Position;
		std::vector<bool>	m_IsDown;
		const Window&			m_Window;

	};
}}

#endif