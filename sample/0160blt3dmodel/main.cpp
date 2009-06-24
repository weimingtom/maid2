/*
  3Dモデルの描画
*/


#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics3drender.h"

#include"../../source/graphics/modelmqo.h"
#include"../../source/graphics/camera.h"

#include"startupdialog.h"

using namespace Maid;

class MyApp : public IGameThread
{
public:
 MyApp::MyApp()
    :m_Render(m_Command)
  {

  }

protected:
  bool SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
  {
    const RETURNCODE ret = BeginStartupInfo( DeviceList, conf );
    return ret==GAMESTART;
  }

  void Initialize()
  {
    m_Command.Initialize();
    m_Render.Initialize( );
    m_Model.Load( MAIDTEXT("box.mqo") );

    const SIZE2DF Screen = GetDefaultConfig().Graphics.ScreenSize;

    m_Camera.SetPerspective( DEGtoRAD(60.0f), Screen.w/Screen.h, 0.1f, 1000.0f );
    m_Camera.SetPosition( POINT3DF(0,200,-400) );
    m_Camera.SetTarget( POINT3DF(0,0,0) );
    m_Camera.SetUpVector( VECTOR3DF(0,1,0) );
  }

  void UpdateFrame()
  {
    const Mouse& m = GetMouse();
    const Keybord& k = GetKeybord();

    if( m_Render.IsInitializing() ) { return ; }
    if( m_Model.IsLoading() ) { return ; }

  }

  void UpdateDraw()
  {
    if( m_Render.IsInitializing() ) { return ; }
    if( m_Model.IsLoading() ) { return ; }

    const RenderTargetBase& rt = GetGraphicsCore().GetBackBuffer();
    const IDepthStencil& ds = GetGraphicsCore().GetDepthStencil();

    m_Command.SetRenderTarget( rt, ds );
    m_Command.ClearRenderTarget( COLOR_A32B32G32R32F(1,0.5f,0,0) );
    m_Command.ClearDepthStencil( 1.0f, 0x00 );

    m_Command.Begin();

    m_Render.SetCamera(m_Camera);
    m_Render.Blt( POINT3DF(0,0,0), m_Model );
//    m_Render.Fill();

    m_Command.End();

  }

  void Finalize()
  {

  }

private:
  GraphicsCommandControl  m_Command;
  Graphics3DRender  m_Render;
  ModelMQO          m_Model;

  Camera  m_Camera;
};



int WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, 
    int nCmdShow
)
{
  GameFrameWork app( SPGAMETHREAD(new MyApp) );
  
  return app.Run(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
}

