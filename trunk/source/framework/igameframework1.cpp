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
  return LocalBootCheck();
}

void IGameFrameWork::Initialize()
{
  IVideoDeviceList& VideoDeviceList = GetVideoDeviceList();

  { //  各種デバイスリストの初期化
    VideoDeviceList.Initialize();
  }

  std::vector<IVideoDeviceList::INFO> VideoDevice;

  { //  使えるデバイスリストの取得
    VideoDeviceList.GetList(VideoDevice);
  }


  {
    m_pGame->Initialize(VideoDevice);

    //  m_pGame->Initialize() 内部で終了することもある
    if( m_pGame->IsExitGame() )
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
      IVideoDeviceList::CREATEPARAM param;
      param.IsSoftwareVertexProcessing = Config.Graphic.IsSoftwareVertexProcessing;
      SPVIDEODEVICE pVideo = VideoDeviceList.Create(Config.Graphic.DeviceID,param );
      m_Graphic.Initialize( pVideo );
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


