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

  { //  各種デバイスリストの初期化
    GraphicsDeviceList.Initialize();
  }

  std::vector<Graphics::IDeviceList::INFO> GraphicsDevice;

  { //  使えるデバイスリストの取得
    GraphicsDeviceList.GetList(GraphicsDevice);
  }


  {
    const bool ret = m_pGame->SelectDevice(GraphicsDevice);

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
      m_JobPool.Initialize();
    }
    {
      Graphics::IDeviceList::CREATEPARAM param;
      param.IsSoftwareVertexProcessing = Config.Graphics.IsSoftwareVertexProcessing;
      Graphics::SPDEVICE pGraphics = GraphicsDeviceList.Create(Config.Graphics.DeviceID,param );
      m_Graphics.Initialize( pGraphics );
    }

    {
      m_Storage.Initialize();

    }

  }
  {
    //  ゲームの初期化
		IGameThread::BEGINPARAM	param;

    Graphics::SPDRAWCOMMAND p = m_Graphics.GetDevice()->GetDrawCommandExecute();
    param.DrawCommand.push_back( p );

    m_pGame->Initialize(param);
  }
}

void IGameFrameWork::OnLoop()
{
  if( m_pGame->IsExitGame() )
  {
    OnExit(0);
    return ;
  }

  LocalLoop();
  m_pGame->UpdateFrame(0,0);


  {
    RenderTargetScreen target;

    m_pGame->UpdateDraw(target,0,0);
    m_Graphics.GetDevice()->Present();
  }
}

void IGameFrameWork::Finalize()
{
  m_pGame->Finalize();
  m_pGame.reset();

  LocalFinalize();
  m_Graphics.Finalize();
}


IGameFrameWork::OSMESSAGESTATE IGameFrameWork::OnOSMessage( const OSMessage::Base& Message )
{
  const IGameThread::OSMESSAGESTATE ret = m_pGame->OnOSMessage( Message );

  if( ret==IGameThread::OSMESSAGESTATE_RETURN ) { return OSMESSAGESTATE_RETURN; }

  return OSMESSAGESTATE_DEFAULT;
}

}


