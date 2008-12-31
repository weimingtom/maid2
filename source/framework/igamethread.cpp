#include"igamethread.h"


namespace Maid
{

using namespace OSMessage;

const IGameThread::DEFAULTCONFIG& IGameThread::GetDefaultConfig() const
{
  return m_DefaultConfig;
}


bool	IGameThread::SelectDevice( const DEVICELIST& DeviceList )
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

	config.Input.MouseDeviceID = 0;


	m_IsExitGame = false;
	return LocalSelectDevice( DeviceList, config );
}

void	IGameThread::Initialize( const BEGINPARAM& param )
{
	LocalInitialize(param);
}


void IGameThread::UpdateFrame()
{
	LocalUpdateFrame();
}

void IGameThread::UpdateDraw()
{
	LocalUpdateDraw();
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


IGameThread::OSMESSAGESTATE IGameThread::OnOSMessage( const OSMessage::Base& Message )
{
  const OSMESSAGESTATE ret = LocalOnOSMessage( Message );

  if( ret==OSMESSAGESTATE_RETURN ) { return OSMESSAGESTATE_RETURN; }

  switch( Message.Message )
  {
  case Base::PUSHCLOSEBUTTON: { OnExitGame(); }break;
  }

  return OSMESSAGESTATE_DEFAULT;
}



bool IGameThread::LocalSelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf ){return true;}
void IGameThread::LocalInitialize( const BEGINPARAM& param ){}

void IGameThread::LocalUpdateFrame(){}
void IGameThread::LocalUpdateDraw(){}

void IGameThread::LocalFinalize(){}
IGameThread::OSMESSAGESTATE IGameThread::LocalOnOSMessage( const OSMessage::Base& Message )
{
  return OSMESSAGESTATE_DEFAULT;
}

}