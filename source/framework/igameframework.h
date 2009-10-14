
/*!
	@file
	@brief ゲームフレームワークのベース
 */

#ifndef maid2_framework_igameframework_h
#define maid2_framework_igameframework_h


#include"../config/define.h"


#include"../graphics/core/idevicelist.h"
#include"../input/core/idevicelist.h"
#include"../sound/core/idevicelist.h"

#include"osmessage.h"
#include"iapplication.h"
#include"igamethread.h"

#include"../graphics/graphicscore.h"
#include"../sound/soundcore.h"
#include"../input/mouse.h"
#include"../input/keybord.h"
#include"../input/textinput.h"

#include"../auxiliary/jobpool.h"
#include"../auxiliary/fpstimer.h"
#include"../storage/storage.h"


namespace Maid
{
  class IGameFrameWork : public IApplication
	{
  public:
    IGameFrameWork( const SPGAMETHREAD& pGame );
    virtual ~IGameFrameWork(){}


    GraphicsCore& GetGraphicsCore();
    SoundCore& GetSoundCore();
    Storage& GetStorage();
    Mouse& GetMouse();
    Keybord& GetKeybord();
    TextInput& GetTextInput();
    FPSTimer& GetTimer();
    JobPool& GetJobPool();

    const GraphicsCore& GetGraphicsCore()const ;
    const SoundCore& GetSoundCore()const ;
    const Storage& GetStorage()const ;
    const Mouse& GetMouse()const ;
    const Keybord& GetKeybord()const ;
    const TextInput& GetTextInput()const ;
    const FPSTimer& GetTimer()const ;
    const JobPool& GetJobPool()const;

    const IGameThread::DEFAULTCONFIG& GetDefaultConfig() const;

    enum OSMESSAGESTATE
    {
      OSMESSAGESTATE_RETURN,   //!<	メッセージは処理されたのでこれ以降は呼ばない
      OSMESSAGESTATE_DEFAULT,  //!<	デフォルトの動作を呼び出す
    };

  protected:
    bool BootCheck();
		void Initialize();
		void OnLoop();
		void Finalize();

  protected:
    struct INITIALIZEPARAM
    {
      SIZE2DI WindowSize;
      String  WindowCaption;
      bool    SystemCursorDraw;
    };
		virtual void LocalInitialize( const INITIALIZEPARAM& conf )=0;
		virtual void LocalLoop()=0;
		virtual void LocalFinalize()=0;

    virtual Graphics::IDeviceList& GetGraphicsDeviceList()=0;
    virtual Input::IDeviceList& GetInputDeviceList()=0;
    virtual Sound::IDeviceList& GetSoundDeviceList()=0;

    OSMESSAGESTATE OnOSMessage( const OSMessage::Base& Message );

	private:
    IGameThread::DEFAULTCONFIG m_DefaultConfig;

    GraphicsCore  m_Graphics;
    SoundCore     m_Sound;
    Storage       m_Storage;
    JobPool       m_JobPool;
    Mouse         m_Mouse;
    Keybord       m_Keybord;
    TextInput     m_TextInput;

    FPSTimer      m_Timer;

		SPGAMETHREAD	m_pGame;
  };
}


#endif
