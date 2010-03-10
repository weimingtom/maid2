/*
  3Dモデルを表示するだけの雛形
*/


#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics3drender.h"
#include"../../source/graphics/graphics2drender.h"
#include"../../source/graphics/texture2d.h"

#include"../../source/graphics/modelmqo.h"
#include"../../source/graphics/camera.h"
#include"../../source/auxiliary/counter/loopcounter.h"
#include"../../source/auxiliary/counter/linearcounter.h"
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
    m_Render.Initialize();
    m_2DRender.Initialize();

//    m_Texture.LoadFile( MAIDTEXT("nc1673.bmp") );

//    m_Model.Load( MAIDTEXT("box.mqo") );
//    m_Model.Load( MAIDTEXT("boxsphere.mqo") );
    m_Model.Load( MAIDTEXT("boxspheretexture.mqo") );

    const SIZE2DF Screen = GetDefaultConfig().Graphics.ScreenSize;

    m_Camera.SetPerspective( DEGtoRAD(60.0f), Screen.w/Screen.h, 400.0f, 1000.0f );
    m_Camera.SetPosition( POINT3DF(0,200,-600) );
    m_Camera.SetTarget( POINT3DF(0,0,0) );
    m_Camera.SetUpVector( VECTOR3DF(0,1,0) );

    m_ModelRotate.Set( DEGtoRAD(0), DEGtoRAD(360), 60 );
    m_LightRotate.Set( DEGtoRAD(0), DEGtoRAD(360), 60 );
    m_ModelAlpha.Set( 0.0f, 1.0f, 60 );

    m_Font.Create( SIZE2DI(8,16), true );
    m_IsLight = false;
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
    if( k.IsDown(Keybord::BUTTON_UP)     ) { ++m_ModelAlpha; }
    else if( k.IsDown(Keybord::BUTTON_DOWN) ) { --m_ModelAlpha; }
    if( k.IsDown('A')     ) { --m_LightRotate; }
    else if( k.IsDown('S') ) { ++m_LightRotate; }

    if( k.IsIn('Q')     ) { m_IsLight = !m_IsLight; }
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

    {
      std::vector<LIGHT> LightList;
      if( m_IsLight )
      {
        LIGHT light;
        light.Type = LIGHT::DIRECTIONAL;
        light.Diffuse = COLOR_R32G32B32A32F(1,1,1,1);
        light.Direction = VECTOR3DF(1,1,0);

        const float rot = m_LightRotate;
        const float s = Math<float>::sin(rot);
        const float c = Math<float>::cos(rot);

        light.Direction = VECTOR3DF(c,s,0).Normalize();

        LightList.push_back(light);
      }
      m_Render.SetLight( LightList );
//      const float amb = 0.5f;
//      m_Render.SetAmbient( COLOR_R32G32B32A32F(amb,amb,amb,1) );
    }

    {
      m_Render.BltR( POINT3DF(0,0,0), m_Model, 1, m_ModelRotate, VECTOR3DF(0,1,0) );
    }
    {
      const MATRIX4DF pos = MATRIX4DF().SetTranslate(80,0,0);
      const MATRIX4DF rot = MATRIX4DF().SetRotationY(0);
      const float alpha = m_ModelAlpha;

      m_Render.Blt( rot*pos, m_Model, alpha );
    }

    {
      const String str = MAIDTEXT( "←→でモデル回転　↑↓でモデル透明　ASでライト回転 QでライトＯＮＯＦＦ" );
      m_2DRender.BltText( POINT2DI(0,0), m_Font, str );
    }
    {
      const float o = RADtoDEG(m_ModelRotate);
      const float l = RADtoDEG(m_LightRotate);
      const float a = m_ModelAlpha;
      const String str = String::PrintFormat( "objrot:%f, lightrot:%f alpha:%f", o, l, a );
      m_2DRender.BltText( POINT2DI(0,20), m_Font, str );
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
  Texture2D         m_Texture;

  Camera  m_Camera;
  LoopCounter<float>   m_LightRotate;
  LoopCounter<float>   m_ModelRotate;
  LinearCounter<float>   m_ModelAlpha;
  Font  m_Font;
  bool  m_IsLight;
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

