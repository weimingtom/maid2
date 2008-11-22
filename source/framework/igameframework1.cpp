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
  IGraphicsDeviceList& GraphicsDeviceList = GetGraphicsDeviceList();

  { //  各種デバイスリストの初期化
    GraphicsDeviceList.Initialize();
  }

  std::vector<IGraphicsDeviceList::INFO> GraphicsDevice;

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
      IGraphicsDeviceList::CREATEPARAM param;
      param.IsSoftwareVertexProcessing = Config.Graphics.IsSoftwareVertexProcessing;
      SPGRAPHICSDEVICE pGraphics = GraphicsDeviceList.Create(Config.Graphics.DeviceID,param );
      m_Graphics.Initialize( pGraphics );
    }


  }
  {
    //  ゲームスレッドの作成
	  m_Thread.SetFunc( MakeThreadObject(&IGameFrameWork::ThreadFunction,this) );
	  m_Thread.Execute();
  }
}

void IGameFrameWork::OnLoop()
{
  LocalLoop();

	{
		//	スレッドが終了してたら終了
		if( m_Thread.IsThreadExited() )
		{
			if( m_Thread.IsThreadAssert() )
			{
				//	エラー終了だったら没
			}else
			{
				ThreadMutexLocker Lock(m_MessageMutex);
			//	m_GraphicCore.DrawFrame(m_ReturnMessageQue);
			}

			OnExit(0);
			return ;
		}
	}

  //  OSからのメッセージを追加

/*

	if( m_IsFrameUpdating )
	{
		//	キューにたまっているものを描画
		ThreadMutexLocker Lock(m_MessageMutex);


//		m_GraphicCore.DrawFrame( m_ReturnMessageQue );
		m_IsFrameUpdating = false;
	}else
	{
		::Sleep(1);
		return ;
	}
*/
}

void IGameFrameWork::Finalize()
{
  m_Thread.Close();
  LocalFinalize();
}

}


