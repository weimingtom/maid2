/*!
 	@file
 	@brief	入力ドライバの親 オブジェクト
 */

#ifndef maid2_input_core_win32_devicelist_h
#define maid2_input_core_win32_devicelist_h

#include"../../../config/define.h"
#include"../../../config/win32.h"
#include"../../../framework/win32/window.h"
#include"../idevicelist.h"


namespace Maid{ namespace Input {

  class DeviceList : public IDeviceList
	{
	public:
    DeviceList( Window& hWnd );
		void Initialize();

    void GetKeybordList( std::vector<KEYBORDINFO>& info );
		SPKEYBORDDEVICE CreateKeybordDevice( int DeviceID );

    void GetMouseList( std::vector<MOUSEINFO>& info );
  	SPMOUSEDEVICE   CreateMouseDevice( int DeviceID );

		SPCHARCODEDEVICE   CreateCharCodeDevice();
		SPINPUTMETHODDEVICE   CreateInputMethodDevice();


  private:
		Window&			m_Window;
	};


}}

#endif