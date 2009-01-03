#include"igameframework.h"

namespace Maid
{

IGameFrameWork::IGameFrameWork( const SPGAMETHREAD& pGame )
  :m_pGame(pGame)
{

}

bool IGameFrameWork::BootCheck()
{
 #pragma  COMPILERMSG( "TODO:ここに２重起動チェック" )
 return true;
}

void IGameFrameWork::Initialize()
{
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
    const bool ret = m_pGame->SelectDevice( Device );

    //  m_pGame->SelectDevice() 内部で終了することもある
    if( !ret )
    {
      OnExit(0);
      return ;
    }
  }
  const IGameThread::DEFAULTCONFIG& Config = m_pGame->GetDefaultConfig();

  //  ＯＳ特有の初期化
  LocalInitialize(Config);

  {
    //  各種デバイスの初期化
    {
      Graphics::IDeviceList::CREATEPARAM param;
      param.IsSoftwareVertexProcessing = Config.Graphics.IsSoftwareVertexProcessing;
      Graphics::SPDEVICE pGraphics = GraphicsDeviceList.Create(Config.Graphics.DeviceID,param );
      m_Graphics.Initialize( pGraphics );
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
  }
  {
    //  ゲームの初期化
		IGameThread::BEGINPARAM	param;

    param.ScreenSize = Config.Graphics.ScreenSize;

    Graphics::SPDRAWCOMMAND p = m_Graphics.GetDevice()->GetDrawCommandExecute();

    p->SetRenderTarget(0
      ,m_Graphics.GetDevice()->GetDefaultRenderTarget()
      ,m_Graphics.GetDevice()->GetDefaultDepthStencil()
      );

    param.DrawCommand.push_back( p );
    param.pFontDevice = GetOSDevice().CreateFontDevice();

    #pragma  COMPILERMSG( "InputCoreみたいの必要かもなぁ" )
    param.pMouseDevice = InputDeviceList.CreateMouseDevice(Config.Input.MouseDeviceID);
    param.pKeybordDevice = InputDeviceList.CreateKeybordDevice(Config.Input.KeybordDeviceID);

    m_pGame->Initialize(param);
  }

  m_Timer.Initialize();
  m_Timer.Reset();
}

void IGameFrameWork::OnLoop()
{
  if( m_pGame->IsExitGame() )
  {
    OnExit(0);
    return ;
  }

  LocalLoop();
  m_pGame->UpdateFrame();
  m_Sound.Update();


  if( !m_Timer.IsTimerOver() )
  { //  予定時間を過ぎていなかったら描画する
    m_pGame->UpdateDraw();
    m_Graphics.GetDevice()->Present();
  }

  m_Timer.Sleep();
}

void IGameFrameWork::Finalize()
{
  m_pGame->Finalize();
  m_pGame.reset();

  LocalFinalize();
  m_Graphics.Finalize();
  m_Sound.Finalize();
}


IGameFrameWork::OSMESSAGESTATE IGameFrameWork::OnOSMessage( const OSMessage::Base& Message )
{
  const IGameThread::OSMESSAGESTATE ret = m_pGame->OnOSMessage( Message );

  if( ret==IGameThread::OSMESSAGESTATE_RETURN ) { return OSMESSAGESTATE_RETURN; }

  return OSMESSAGESTATE_DEFAULT;
}

}


