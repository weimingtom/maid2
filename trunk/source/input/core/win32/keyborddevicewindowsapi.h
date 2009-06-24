/*!
 	@file
 	@brief キーボードドライバ（プロシージャ で取得する Ver. )
 */

#ifndef maid2_input_core_win32_keyborddevicewindowsapi_h
#define maid2_input_core_win32_keyborddevicewindowsapi_h

#include"../../../config/define.h"
#include"../../../config/win32.h"
#include"../ikeyborddevice.h"
#include<vector>


namespace Maid{ namespace Input {

	class KeybordDeviceWindowsAPI 
    :public IKeybordDevice
	{
	public:
		KeybordDeviceWindowsAPI();
		virtual ~KeybordDeviceWindowsAPI();

		virtual void	Initialize();
		virtual void	Finalize();
		virtual void	IsDown( std::vector<bool>& KeyList )	const;
		virtual int		GetButtonnMax()							const;
	};

}}

#endif