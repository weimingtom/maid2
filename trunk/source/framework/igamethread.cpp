#include"igamethread.h"


namespace Maid
{

using namespace OSMessage;

const IGameThread::DEFAULTCONFIG& IGameThread::GetDefaultConfig() const
{
  return m_DefaultConfig;
}


bool	IGameThread::SelectDevice( const std::vector<Graphics::IDeviceList::INFO>& GraphicsDevice )
{
  DEFAULTCONFIG& config = m_DefaultConfig;

	config.WindowCaption = MAIDTEXT("GameFrameWork");
	config.Graphics.DeviceID = 0;
	config.Graphics.IsSoftwareVertexProcessing = true;
	config.Graphics.ScreenSize = SIZE2DI(800,600);
	config.Graphics.DrawCommandCount = 1;
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
	return LocalSelectDevice( GraphicsDevice, config );
}

void	IGameThread::Initialize( const BEGINPARAM& param )
{

	LocalInitialize(param);
}

void	IGameThread::OnLoop()
{
	LocalLoop();
}

void	IGameThread::Finalize()
{
	LocalFinalize();
}



bool IGameThread::IsExitGame()
{
	return m_IsExitGame;
}

void IGameThread::OnExitGame()
{
	m_IsExitGame = true;
}


void IGameThread::OnOSMessage( const SPOSMESSAGE& pMess )
{
  const OSMESSAGECALLBACK ret = LocalOnOSMessage( pMess );

  if( ret==OSMESSAGECALLBACK_RETURN ) { return ; }

  switch( pMess->Message )
  {
  case Base::PUSHCLOSEBUTTON: { OnExitGame(); }break;
  }
}



bool IGameThread::LocalSelectDevice( const std::vector<Graphics::IDeviceList::INFO>& VideoDevice, DEFAULTCONFIG& conf ){return true;}
void IGameThread::LocalInitialize( const BEGINPARAM& param ){}
void IGameThread::LocalLoop(){}
void IGameThread::LocalFinalize(){}
IGameThread::OSMESSAGECALLBACK IGameThread::LocalOnOSMessage( const SPOSMESSAGE& pMess )
{
  return OSMESSAGECALLBACK_DEFAULT;
}

}