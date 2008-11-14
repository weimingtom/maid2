
/*!
    @file
    @brief Windows用ゲームフレームワーク
 */

#ifndef framework_win32_gameframework_h
#define framework_win32_gameframework_h


#include"../../config/define.h"
#include"../../config/Win32.h"

#include<boost/smart_ptr.hpp>

#include"../../auxiliary/thread/win32/threadcontroller.h"
#include"../../auxiliary/thread/win32/threadmutex.h"
#include"../gamethread.h"
#include"../gameframeworkbase.h"

#include"window.h"
#include"../osmessage.h"

namespace Maid
{
	/*!
      @class	GameFrameWork gameframework.h
      @brief	フレームワークベースクラス
	 */ 
	class GameFrameWork : public GameFrameWorkBase
	{
	public:
    GameFrameWork( const SPGAMETHREAD& pGame );

    int Run( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );

  protected:
		virtual void LocalSetup();
		virtual void LocalLoop();
		virtual void LocalCleanup();
		
		void ExecuteWindowMessage();
		void HardWareCursorCheck();

		virtual HICON	LoadIcon();

	private:
		Window::CALLBACKCODE wmClose( WindowsMessage& msg );
		Window::CALLBACKCODE wmActivate( WindowsMessage& msg );
		Window::CALLBACKCODE wmNcLButtonDblClk( WindowsMessage& msg ); 
		Window::CALLBACKCODE wmSysCommand( WindowsMessage& msg ); 
		Window::CALLBACKCODE wmKeyDown( WindowsMessage& msg ); 
		Window::CALLBACKCODE wmSysKeyDown( WindowsMessage& msg ); 
		Window::CALLBACKCODE wmPaint( WindowsMessage& msg ); 


	private:
		Window	m_Window;
	};

}
#endif
