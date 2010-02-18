#include"app.h"

#include"startupdialog.h"


using namespace Maid;


App* GlobalPointer<App>::s_pPointer;

App::App()
  :m_Render(m_Command)
{

}

bool App::SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
{
  const RETURNCODE ret = BeginStartupInfo( DeviceList, conf );
  return ret==GAMESTART;
}

void App::Initialize()
{
  GlobalPointer<App>::Set();

  m_Command.Initialize();
  m_Render.Initialize();

  m_SceneAdmin.Initialize();

  m_State = STATE_INITIALIZING;

}

void App::UpdateFrame()
{
  switch( m_State )
  {
  case STATE_INITIALIZING:
    {
      if( m_Render.IsInitializing() ) { return ; }
      if( m_SceneAdmin.IsInitializing() ) { return ; }

      m_State = STATE_PLAY;


    }break;

  case STATE_PLAY:
    {
      m_SceneAdmin.UpdateFrame();
    }break;
  }

}

void App::UpdateDraw()
{
  const RenderTargetBase& rt = GetGraphicsCore().GetBackBuffer();
  const IDepthStencil& dp = GetGraphicsCore().GetDepthStencil();

  m_Command.SetRenderTarget( rt, dp );
  m_Command.ClearRenderTarget();
  m_Command.ClearDepthStencil();

  switch( m_State )
  {
  case STATE_INITIALIZING:
    {

    }break;

  case STATE_PLAY:
    {
      m_SceneAdmin.UpdateDraw( rt, dp );
    }break;
  }


}

void App::Finalize()
{

}


