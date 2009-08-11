
/*!
    @file
    @brief Windows用ゲームフレームワーク
 */

#ifndef maid2_framework_win32_gameframework_h
#define maid2_framework_win32_gameframework_h


#include"../../config/define.h"
#include"../../config/Win32.h"

#include<boost/smart_ptr.hpp>

#include"../igamethread.h"
#include"../igameframework.h"

#include"window.h"
#include"../osmessage.h"
#include"../../graphics/core/win32/devicelist.h"
#include"../../input/core/win32/devicelist.h"
#include"../../sound/core/win32/devicelist.h"
#include"osdevice.h"

namespace Maid
{
	/*!
      @class	GameFrameWork gameframework.h
      @brief	フレームワークベースクラス
	 */ 
	class GameFrameWork : public IGameFrameWork
	{
	public:
    GameFrameWork( const SPGAMETHREAD& pGame );

    int Run( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );
  protected:
		virtual void LocalInitialize(const INITIALIZEPARAM& conf);
		virtual void LocalLoop();
		virtual void LocalFinalize();
    virtual IOSDevice& GetOSDevice();
    virtual Graphics::IDeviceList& GetGraphicsDeviceList();
    virtual Input::IDeviceList& GetInputDeviceList();
    virtual Sound::IDeviceList& GetSoundDeviceList();
		
		void ExecuteWindowMessage();
		void HardWareCursorCheck();

		virtual HICON	LoadIcon();

	private:
		Window::CALLBACKCODE wmClose( WindowsMessage& msg );
		Window::CALLBACKCODE wmActivate( WindowsMessage& msg );
		Window::CALLBACKCODE wmActivateApp( WindowsMessage& msg );
		Window::CALLBACKCODE wmNcLButtonDblClk( WindowsMessage& msg ); 
		Window::CALLBACKCODE wmSysCommand( WindowsMessage& msg ); 
		Window::CALLBACKCODE wmKeyDown( WindowsMessage& msg ); 
		Window::CALLBACKCODE wmSysKeyDown( WindowsMessage& msg ); 
		Window::CALLBACKCODE wmPaint( WindowsMessage& msg ); 

    Window::CALLBACKCODE ExecMessage( const OSMessage::Base& mess );

	private:
    OSDevice              m_OSDevice;
		Window	              m_Window;
    Graphics::DeviceList  m_GraphicsDeviceList;
    Input::DeviceList     m_InputDeviceList;
    Sound::DeviceList     m_SoundDeviceList;
    int                   m_ShowCursorCount;

    bool  m_SysytemCursorDraw;
	};

}
#endif
