#include"igameframework.h"

namespace Maid
{

IGameFrameWork::IGameFrameWork( const SPGAMETHREAD& pGame )
  :m_pGame(pGame)
  ,m_DrawSkipping(false)
{

}

bool IGameFrameWork::BootCheck()
{
 return m_pGame->BootCheck();
}

void IGameFrameWork::Initialize()
{
  ThreadController::SetCurrentThreadProcesserMask( 1 );

  Graphics::IDeviceList& GraphicsDeviceList = GetGraphicsDeviceList();
  Input::IDeviceList& InputDeviceList = GetInputDeviceList();
  Sound::IDeviceList& SoundDeviceList = GetSoundDeviceList();

  { //  各種デバイスリストの初期化
    GraphicsDeviceList.Initialize();
    InputDeviceList.Initialize();
    SoundDeviceList.Initialize();
  }

  IGameThread::DEVICELIST Device;

  { //  使えるデバイスリストの取得
    GraphicsDeviceList.GetList(Device.Graphics);
    InputDeviceList.GetMouseList(Device.Mouse);
    InputDeviceList.GetKeybordList(Device.Keybord);
    SoundDeviceList.GetList(Device.Sound);
  }


  {
    IGameThread::DEFAULTCONFIG& config = m_DefaultConfig;

    config.SystemCursorDraw = true;
	  config.WindowCaption = MAIDTEXT("GameFrameWork");
	  config.Graphics.DeviceID = Device.Graphics.front().DeviceID;
	  config.Graphics.ScreenSize = SIZE2DI(800,600);

	  config.Input.MouseDeviceID = 0;

    const bool ret = m_pGame->SelectDevice( Device, config );

    //  m_pGame->SelectDevice() 内部で終了することもある
    if( !ret )
    {
      OnExit(0);
      return ;
    }
  }

  const IGameThread::DEFAULTCONFIG& Config = m_DefaultConfig;
  {
    //  ＯＳ特有の初期化
    INITIALIZEPARAM param;
    param.WindowSize = Config.Graphics.ScreenSize;
    param.WindowCaption = Config.WindowCaption;
    param.SystemCursorDraw = Config.SystemCursorDraw;
    LocalInitialize(param);
  }


  {
    //  各種デバイスの初期化
    {
      Graphics::SPDEVICE pGraphics = GraphicsDeviceList.Create(Config.Graphics.DeviceID);
      Graphics::SPFONTDEVICE pFont = GraphicsDeviceList.CreateFontDevice();
      m_Graphics.Initialize( pGraphics, pFont );
    }
    {
      SoundCore::INITIALIZEPARAM param;
      param.pDevice = SoundDeviceList.CreateDevice(Config.Sound.DeviceID );
      param.IsBackGround = true;

      m_Sound.Initialize( param );
    }
    {
      m_Storage.Initialize();
      m_JobPool.Initialize();
    }

    {
      m_Mouse.Initialize( InputDeviceList.CreateMouseDevice(Config.Input.MouseDeviceID) );
      m_Keybord.Initialize( InputDeviceList.CreateKeybordDevice(Config.Input.KeybordDeviceID) );
      m_TextInput.Initialize( 
        InputDeviceList.CreateCharCodeDevice(),
        InputDeviceList.CreateInputMethodDevice() 
        );

    }
    m_Timer.Initialize();
  }

  { //  初期化したデバイスをユーザー設定どおりに入れていく
    {
      GraphicsCore::SWAPCHAINFORMAT fmt = m_Graphics.GetSwapChainFormat();
      fmt.Size = Config.Graphics.ScreenSize;
      m_Graphics.SetSwapChainFormat( fmt );
    }
    {
      const SIZE2DI& size = Config.Graphics.ScreenSize;
      const RECT2DI rc( POINT2DI(0,0), size );
      m_Mouse.SetClipRange( rc );
      m_Mouse.SetMapping( rc );
    }
  }

  {
    //  ゲームの初期化
    m_pGame->m_pFrameWork = this;
    m_pGame->Initialize();
  }

  m_Timer.Reset();
}


GraphicsCore& IGameFrameWork::GetGraphicsCore() { return m_Graphics; }
SoundCore& IGameFrameWork::GetSoundCore() { return m_Sound; }
Storage& IGameFrameWork::GetStorage() { return m_Storage; }
Mouse& IGameFrameWork::GetMouse() { return m_Mouse; }
Keybord& IGameFrameWork::GetKeybord() { return m_Keybord; }
TextInput& IGameFrameWork::GetTextInput() { return m_TextInput; }
FPSTimer& IGameFrameWork::GetTimer() { return m_Timer; }
JobPool& IGameFrameWork::GetJobPool() { return m_JobPool; }
const GraphicsCore& IGameFrameWork::GetGraphicsCore() const { return m_Graphics; }
const SoundCore& IGameFrameWork::GetSoundCore() const { return m_Sound; }
const Storage& IGameFrameWork::GetStorage() const { return m_Storage; }
const Mouse& IGameFrameWork::GetMouse() const { return m_Mouse; }
const Keybord& IGameFrameWork::GetKeybord() const { return m_Keybord; }
const TextInput& IGameFrameWork::GetTextInput() const { return m_TextInput; }
const FPSTimer& IGameFrameWork::GetTimer() const { return m_Timer; }
const JobPool& IGameFrameWork::GetJobPool() const { return m_JobPool; }

const IGameThread::DEFAULTCONFIG& IGameFrameWork::GetDefaultConfig() const{ return m_DefaultConfig; }

void IGameFrameWork::OnLoop()
{
  if( m_pGame->IsExitGame() )
  {
    OnExit(0);
    return ;
  }

  LocalLoop();
  m_Mouse.Update();
  m_Keybord.Update();
  m_TextInput.Update();
  m_pGame->UpdateFrame();
  m_Sound.Update();


  if( !m_Timer.IsTimerOver() )
  { //  予定時間を過ぎていなかったら描画する
    if( !m_DrawSkipping )
    {
      m_pGame->UpdateDraw();
      m_Graphics.Present();
    }
  }

  m_Timer.Sleep();
}

void IGameFrameWork::Finalize()
{
  m_pGame->Finalize();
  m_pGame.reset();

  LocalFinalize();
  m_JobPool.Finalize();
  m_Storage.Finalize();
  m_Sound.Finalize();
  m_Graphics.Finalize();
  m_Keybord.Finalize();
  m_Mouse.Finalize();
  m_TextInput.Finalize();
}


IGameFrameWork::OSMESSAGESTATE IGameFrameWork::OnOSMessage( const OSMessage::Base& Message )
{
  if( m_pGame.get()!=NULL )
  {
    const IGameThread::OSMESSAGESTATE ret = m_pGame->OnOSMessage( Message );

    if( ret==IGameThread::OSMESSAGESTATE_RETURN ) { return OSMESSAGESTATE_RETURN; }
  }

  return OSMESSAGESTATE_DEFAULT;
}

void IGameFrameWork::SetDrawSkipState( bool on )
{
  m_DrawSkipping = on;  
}


}


