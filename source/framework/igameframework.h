
/*!
	@file
	@brief ゲームフレームワークのベース
 */

#ifndef framework_igameframework_h
#define framework_igameframework_h


#include"../config/define.h"


#include"../auxiliary/thread.h"
#include"../graphics/graphicscore.h"
#include"../graphics/core/idevicelist.h"
#include"../input/core/idevicelist.h"
#include"../sound/core/idevicelist.h"
#include"../sound/soundcore.h"
#include"../graphics/rendertargetscreen.h"
#include"../storage/storage.h"
#include"../auxiliary/jobpool.h"
#include"../auxiliary/fpstimer.h"

#include"osmessage.h"
#include"iapplication.h"
#include"igamethread.h"

namespace Maid
{
  class IGameFrameWork : public IApplication
	{
  public:
    IGameFrameWork( const SPGAMETHREAD& pGame );
    virtual ~IGameFrameWork(){}

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
    virtual Input::IDeviceList& GetInputDeviceList()=0;
    virtual Sound::IDeviceList& GetSoundDeviceList()=0;

    enum OSMESSAGESTATE
    {
      OSMESSAGESTATE_RETURN,   //!<	メッセージは処理されたのでこれ以降は呼ばない
      OSMESSAGESTATE_DEFAULT,  //!<	デフォルトの動作を呼び出す
    };
    OSMESSAGESTATE OnOSMessage( const OSMessage::Base& Message );

	private:
    GraphicsCore  m_Graphics;
    SoundCore     m_Sound;
    Storage       m_Storage;
    JobPool       m_JobPool;

    FPSTimer      m_Timer;
		SPGAMETHREAD	m_pGame;
  };
}


#endif
