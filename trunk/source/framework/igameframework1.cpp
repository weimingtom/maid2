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
      Graphics::IDeviceList::CREATEPARAM param;
      param.IsSoftwareVertexProcessing = Config.Graphics.IsSoftwareVertexProcessing;
      Graphics::SPDEVICE pGraphics = GraphicsDeviceList.Create(Config.Graphics.DeviceID,param );
      m_Graphics.Initialize( pGraphics );
    }


  }
  {
    //  ゲームの初期化
		IGameThread::BEGINPARAM	param;
    param.DrawCommand.push_back( Graphics::SPDRAWCOMMAND(m_Graphics.GetDevice()->CreateDrawCommandPlayer()) );

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
  m_pGame->OnLoop();
  m_Graphics.GetDevice()->Flip();
}

void IGameFrameWork::Finalize()
{
  m_pGame->Finalize();
  LocalFinalize();
}


IGameFrameWork::OSMESSAGESTATE IGameFrameWork::OnOSMessage( const OSMessage::Base& Message )
{
  const IGameThread::OSMESSAGESTATE ret = m_pGame->OnOSMessage( Message );

  if( ret==IGameThread::OSMESSAGESTATE_RETURN ) { return OSMESSAGESTATE_RETURN; }

  return OSMESSAGESTATE_DEFAULT;
}

}


