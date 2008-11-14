#include"gamethread.h"


namespace Maid
{

using namespace OSMessage;

const APPCONFIG& GameThread::GetAppConfig()
{
	return m_Config;
}


void	GameThread::OnSetup()
{
	m_Config.WindowCaption = MAIDTEXT("GameFrameWork");
	m_Config.Graphic.IsFullScreen = false;
	m_Config.Graphic.Size = SIZE2DI(800,600);
/*
	m_Config.Graphic.RefreshRate = 60;
	m_Config.Graphic.IsSoftTnL = false;
	m_Config.Graphic.IsWaitVSync = true;
	m_Config.Graphic.IsAspectLock = true;

	m_Config.Sound.SamplesPerSec = 44100;
	m_Config.Sound.BitsPerSample = 16;
	m_Config.Sound.Channels = 2;
*/
	m_IsExitGame = false;
	LocalSetup( m_Config );
}

void	GameThread::OnBegin( const BEGINPARAM& param )
{

	LocalBegin(param);
}

void	GameThread::OnLoop()
{
	LocalLoop();
}

void	GameThread::OnEnd()
{
	LocalEnd();
}

/*
void GameThread::OnNextFrame( CGraphic& gra )
{
	RETURNMESSAGEQUE ret;

	gra.NextFrame();
	GetFrameWork()->OnNextFrame(ret);
	gra.OnReturnMessage(ret);
}
*/
bool GameThread::IsExitGame()
{
	return m_IsExitGame;
}

void GameThread::OnExitGame()
{
	m_IsExitGame = true;
}


void GameThread::OnOSMessage( const SPOSMESSAGE& pMess )
{
  const OSMESSAGECALLBACK ret = LocalOnOSMessage( pMess );

  if( ret==OSMESSAGECALLBACK_RETURN ) { return ; }

  switch( pMess->Message )
  {
  case Base::PUSHCLOSEBUTTON: { OnExitGame(); }break;
  }
}



void GameThread::LocalSetup( APPCONFIG& conf ){}
void GameThread::LocalBegin( const BEGINPARAM& param ){}
void GameThread::LocalLoop(){}
void GameThread::LocalEnd(){}
GameThread::OSMESSAGECALLBACK GameThread::LocalOnOSMessage( const SPOSMESSAGE& pMess )
{
  return OSMESSAGECALLBACK_DEFAULT;
}

}