
/*!
    @file
    @brief Windows用ゲームフレームワーク
 */

#ifndef framework_win32_gameframework_h
#define framework_win32_gameframework_h


#include"../../config/define.h"
#include"../../config/Win32.h"
#include"../../auxiliary/thread/win32/threadcontroller.h"
#include"../../auxiliary/thread/win32/threadmutex.h"
#include"../gamethread.h"
#include"application.h"
#include"window.h"
#include"../osmessage.h"
#include<boost/smart_ptr.hpp>

namespace Maid
{
	/*!
      @class	GameFrameWork gameframework.h
      @brief	フレームワークベースクラス
	 */ 
	class GameFrameWork : public Application
	{
	public:
		GameFrameWork( const SPGAMETHREAD& pGame );

//		void OnNextFrame( RETURNMESSAGEQUE& ReturnMessage );

	protected:
		
		void ExecuteWindowMessage();
		void HardWareCursorCheck();



	protected:
		virtual void LocalSetup();
		virtual void LocalLoop();
		virtual void LocalCleanup();

		virtual HICON	LoadIcon();

	private:
		unt ThreadFunction( volatile ThreadController::BRIGEDATA& brige );
		Window::CALLBACKCODE wmClose( WindowsMessage& msg );
		Window::CALLBACKCODE wmActivate( WindowsMessage& msg );
		Window::CALLBACKCODE wmNcLButtonDblClk( WindowsMessage& msg ); 
		Window::CALLBACKCODE wmSysCommand( WindowsMessage& msg ); 
		Window::CALLBACKCODE wmKeyDown( WindowsMessage& msg ); 
		Window::CALLBACKCODE wmSysKeyDown( WindowsMessage& msg ); 
		Window::CALLBACKCODE wmPaint( WindowsMessage& msg ); 

		void PostWindowMessage( const SPOSMESSAGE& pMess );

	private:
		HANDLE	m_hAppEvent;
		HANDLE	m_hGameEvent;
		bool	m_Initialize;

		bool	m_IsFrameUpdating;
		ThreadMutex	m_MessageMutex;


		OSMESSAGEMESSAGEQUE	m_OSMessageQue;

//		CGraphic::FRAMEINFO	m_FrameInfo;
//		RETURNMESSAGEQUE	m_ReturnMessageQue;

	private:
		Window	m_Window;
/*
		CGraphicCore	m_GraphicCore;
		CSoundCore		m_SoundCore;

		SPKEYBORDDRIVER		m_pKeybordDriver;
		SPMOUSEDRIVER		m_pMouseDriver;
		SPCHARCODEDRIVER	m_pCharCodeDriver;
		SPINPUTMETHODDRIVER	m_pIMEDriver;
*/

		ThreadController	m_Thread;

	private:


		SPGAMETHREAD	m_pGame;
	};

}
#endif
