
/*!
	@file
	@brief ゲームフレームワークのベース
 */

#ifndef framework_igameframework_h
#define framework_igameframework_h


#include"../config/define.h"


#include"../auxiliary/thread.h"
#include"../graphic/core/graphiccore.h"
#include"../graphic/core/ivideodevicelist.h"

#include"iapplication.h"
#include"igamethread.h"

namespace Maid
{
  class IGameFrameWork : public IApplication
	{
  public:
    IGameFrameWork( const SPGAMETHREAD& pGame );

  protected:
    bool BootCheck();
		void Initialize();
		void OnLoop();
		void Finalize();

  protected:
    virtual void LocalBootCheck()=0;
		virtual void LocalInitialize( const IGameThread::DEFAULTCONFIG& conf )=0;
		virtual void LocalLoop()=0;
		virtual void LocalFinalize()=0;

    virtual IVideoDeviceList& GetVideoDeviceList()=0;

  protected:
    void PostWindowMessage( const SPOSMESSAGE& pMess );

	private:
		unt ThreadFunction( volatile ThreadController::BRIGEDATA& brige );
    void ExecuteOSMessage();

	private:
		ThreadMutex	m_MessageMutex;
		ThreadController	m_Thread;

	private:
		SPGAMETHREAD	m_pGame;
    GraphicCore   m_Graphic;
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
