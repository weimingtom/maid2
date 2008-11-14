
/*!
	@file
	@brief ゲームフレームワークのベース
 */

#ifndef framework_gameframeworkbase_h
#define framework_gameframeworkbase_h


#include"../config/define.h"


#include"../auxiliary/thread.h"

#include"applicationbase.h"
#include"gamethread.h"

namespace Maid
{
  class GameFrameWorkBase : public ApplicationBase
	{
  public:
    GameFrameWorkBase( const SPGAMETHREAD& pGame );

  protected:
		void OnSetup();
		void OnLoop();
		void OnCleanup();

  protected:
		virtual void LocalSetup()=0;
		virtual void LocalLoop()=0;
		virtual void LocalCleanup()=0;
		SPGAMETHREAD	m_pGame;

  protected:
    void PostWindowMessage( const SPOSMESSAGE& pMess );

	private:
		unt ThreadFunction( volatile ThreadController::BRIGEDATA& brige );
    void ExecuteOSMessage();

	private:
		ThreadMutex	m_MessageMutex;
		ThreadController	m_Thread;

	private:
		bool	m_Initialize;

		bool	m_IsFrameUpdating;
		OSMESSAGEMESSAGEQUE	m_OSMessageQue;

//		CGraphic::FRAMEINFO	m_FrameInfo;
//		RETURNMESSAGEQUE	m_ReturnMessageQue;

/*
		CGraphicCore	m_GraphicCore;
		CSoundCore		m_SoundCore;

		SPKEYBORDDRIVER		m_pKeybordDriver;
		SPMOUSEDRIVER		m_pMouseDriver;
		SPCHARCODEDRIVER	m_pCharCodeDriver;
		SPINPUTMETHODDRIVER	m_pIMEDriver;
*/
  };
}


#endif
