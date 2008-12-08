
/*!
	@file
	@brief ゲームフレームワークのベース
 */

#ifndef framework_igameframework_h
#define framework_igameframework_h


#include"../config/define.h"


#include"../auxiliary/thread.h"
#include"../graphics/core/core.h"
#include"../graphics/core/idevicelist.h"
#include"../graphics/rendertargetscreen.h"

#include"osmessage.h"
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
		virtual void LocalInitialize( const IGameThread::DEFAULTCONFIG& conf )=0;
		virtual void LocalLoop()=0;
		virtual void LocalFinalize()=0;

    virtual Graphics::IDeviceList& GetGraphicsDeviceList()=0;

    enum OSMESSAGESTATE
    {
      OSMESSAGESTATE_RETURN,   //!<	メッセージは処理されたのでこれ以降は呼ばない
      OSMESSAGESTATE_DEFAULT,  //!<	デフォルトの動作を呼び出す
    };
    OSMESSAGESTATE OnOSMessage( const OSMessage::Base& Message );

	private:
		SPGAMETHREAD	m_pGame;
    Graphics::Core  m_Graphics;
    RenderTargetScreen  m_RenderTarget;


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
