/*
  3Dモデルを表示するだけの雛形
*/


#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics3drender.h"
#include"../../source/graphics/graphics2drender.h"

#include"../../source/graphics/modelmqo.h"
#include"../../source/graphics/camera.h"
#include"../../source/auxiliary/counter/loopcounter.h"
#include"../../source/auxiliary/macro.h"
#include"../../source/auxiliary/mathematics.h"

#include"startupdialog.h"

using namespace Maid;

class MyApp : public IGameThread
{
public:
 MyApp::MyApp()
    :m_Render(m_Command)
    ,m_2DRender(m_Command)
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
    m_2DRender.Initialize( );
    m_Model.Load( MAIDTEXT("boxsphere.mqo") );

    const SIZE2DF Screen = GetDefaultConfig().Graphics.ScreenSize;

    m_Camera.SetPerspective( DEGtoRAD(60.0f), Screen.w/Screen.h, 0.1f, 1000.0f );
    m_Camera.SetPosition( POINT3DF(0,200,-600) );
    m_Camera.SetTarget( POINT3DF(0,0,0) );
    m_Camera.SetUpVector( VECTOR3DF(0,1,0) );

    m_ModelRotate.Set( DEGtoRAD(0), DEGtoRAD(360), 60 );
    m_LightRotate.Set( DEGtoRAD(0), DEGtoRAD(360), 60 );

    m_ModelRotate.ResetStep(5);
    m_Font.Create( SIZE2DI(8,16), true );
  }

  void UpdateFrame()
  {
    const Mouse& m = GetMouse();
    const Keybord& k = GetKeybord();

    if( m_2DRender.IsInitializing() ) { return ; }
    if( m_Render.IsInitializing() ) { return ; }
    if( m_Model.IsLoading() ) { return ; }

    if( k.IsDown(Keybord::BUTTON_RIGHT)     ) { --m_ModelRotate; }
    else if( k.IsDown(Keybord::BUTTON_LEFT) ) { ++m_ModelRotate; }
    if( k.IsDown('A')     ) { --m_LightRotate; }
    else if( k.IsDown('S') ) { ++m_LightRotate; }
  }

  void UpdateDraw()
  {
    if( m_2DRender.IsInitializing() ) { return ; }
    if( m_Render.IsInitializing() ) { return ; }
    if( m_Model.IsLoading() ) { return ; }

    const RenderTargetBase& rt = GetGraphicsCore().GetBackBuffer();
    const IDepthStencil& ds = GetGraphicsCore().GetDepthStencil();

    m_Command.SetRenderTarget( rt, ds );
    m_Command.ClearRenderTarget( COLOR_A32B32G32R32F(1,0.5f,0,0) );
    m_Command.ClearDepthStencil( 1.0f, 0x00 );

    m_Command.Begin();

    m_Render.SetCamera(m_Camera);

    LIGHT light;
    {
      light.Type = LIGHT::DIRECTIONAL;
      light.Diffuse = COLOR_R32G32B32A32F(1,1,1,1);
      light.Direction = VECTOR3DF(1,1,0);

      float aa = DEGtoRAD(90);
      const float rot = m_LightRotate;
      const float s = Math<float>::sin(rot);
      const float c = Math<float>::cos(rot);

      light.Direction = VECTOR3DF(c,s,0).Normalize();
    }


    std::vector<LIGHT> LightList;
    LightList.push_back(light);
    m_Render.SetLight( LightList );

    {
      const MATRIX4DF pos = MATRIX4DF().SetTranslate(0,0,0);
      const MATRIX4DF rot = MATRIX4DF().SetRotationY(m_ModelRotate);

      m_Render.Blt( rot*pos, m_Model );
    }
//    m_Render.Fill();

    {
      const float o = RADtoDEG(m_ModelRotate);
      const float l = RADtoDEG(m_LightRotate);
      const String str = String::PrintFormat( "objrot:%f, lightrot:%f", o, l );
      m_2DRender.BltText( POINT2DI(0,0), m_Font, str );
    }

    m_Command.End();

  }

  void Finalize()
  {

  }

private:
  GraphicsCommandControl  m_Command;
  Graphics2DRender  m_2DRender;
  Graphics3DRender  m_Render;
  ModelMQO          m_Model;

  Camera  m_Camera;
  LoopCounter<float>   m_LightRotate;
  LoopCounter<float>   m_ModelRotate;
  Font  m_Font;
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

