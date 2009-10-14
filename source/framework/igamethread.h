
/*!
    @file
    @brief ゲーム実装用フレームワーク
 */

#ifndef maid2_framework_igamethread_h
#define maid2_framework_igamethread_h


#include"../config/define.h"
#include"../config/typedef.h"
#include"../auxiliary/string.h"
#include"../auxiliary/mathematics.h"
#include"osmessage.h"

#include"../graphics/core/idevicelist.h"
#include"../input/core/idevicelist.h"
#include"../sound/core/idevicelist.h"

#include"../graphics/graphicscore.h"
#include"../sound/soundcore.h"
#include"../input/mouse.h"
#include"../input/keybord.h"
#include"../input/textinput.h"
#include"../auxiliary/jobpool.h"
#include"../storage/storage.h"


#include<boost/smart_ptr.hpp>

namespace Maid
{
  class IGameFrameWork;
  class IGameThread 
  {
    friend IGameFrameWork;
  public:
    IGameThread();
    virtual ~IGameThread();

    bool IsExitGame();
    void OnExitGame();


    GraphicsCore& GetGraphicsCore();
    SoundCore& GetSoundCore();
    Storage& GetStorage();
    Mouse& GetMouse();
    Keybord& GetKeybord();
    TextInput& GetTextInput();
    JobPool& GetJobPool();
    const GraphicsCore& GetGraphicsCore()const ;
    const SoundCore& GetSoundCore()const ;
    const Storage& GetStorage()const ;
    const Mouse& GetMouse()const ;
    const Keybord& GetKeybord()const ;
    const TextInput& GetTextInput()const ;
    const JobPool& GetJobPool()const ;

    void TimerReset();
    void SetScreenSize( const SIZE2DI& size );
    void SetFullScreenState( bool IsFull );
    void SetAspectLock( bool lock );
    bool IsFullScreen() const;
    bool IsAspectLock() const;

    struct DEFAULTCONFIG
    {
      struct GRAPHICS
      {
        int   DeviceID;            //!<  使うデバイスＩＤ
        SIZE2DI ScreenSize;
      };

      struct INPUT
      {
        int MouseDeviceID;    //  使うマウスID
        int KeybordDeviceID;  //  使うキーボードID
      };

      struct SOUND
      {
        int DeviceID;        //  使うデバイスID
        bool IsBackGround;  //  サウンド処理は別スレッドで行うか？
      };

      String WindowCaption;
      bool SystemCursorDraw;
      GRAPHICS  Graphics;
      INPUT     Input;
      SOUND     Sound;
    }; 
    
    const DEFAULTCONFIG& GetDefaultConfig() const;
    struct DEVICELIST
    {
      std::vector<Graphics::IDeviceList::INFO>   Graphics;
      std::vector<Input::IDeviceList::MOUSEINFO> Mouse;
      std::vector<Input::IDeviceList::KEYBORDINFO> Keybord;
      std::vector<Sound::IDeviceList::INFO> Sound;
    };

  protected:
    virtual bool SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf );

    virtual bool BootCheck();
    virtual void Initialize();
    virtual void UpdateFrame()=0;
    virtual void UpdateDraw()=0;
    virtual void Finalize();

    enum OSMESSAGESTATE
    {
      OSMESSAGESTATE_RETURN,   //!<	メッセージは処理されたのでこれ以降は呼ばない
      OSMESSAGESTATE_DEFAULT,  //!<	デフォルトの動作を呼び出す
    };
    virtual OSMESSAGESTATE OnOSMessage( const OSMessage::Base& Message );

    IGameFrameWork* GetFrameWork();
    const IGameFrameWork* GetFrameWork()const;
  private:
    void UpdateMouseMapping();


  private:
    bool		      m_IsExitGame;
    bool          m_IsAspectLock;
    IGameFrameWork* m_pFrameWork;
  };

  typedef	boost::shared_ptr<IGameThread>	SPGAMETHREAD;
}
#endif
