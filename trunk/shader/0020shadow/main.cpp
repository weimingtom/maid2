/*
  シャドウマップを使った影テスト
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



static const POINT3DF s_OBJECT1POS(0,0,0);  //  回転操作できるモデルの配置座標
static const POINT3DF s_OBJECT2POS(1.5f,0,0); //  透明度操作できるモデルの配置座標


static const float s_CAMERA_NEAR = 0.5f;  //  カメラの手前カリング位置 50cm 先から見える
static const float s_CAMERA_FAR = 50.0f;  //  カメラの奥カリング位置   30mmまで見える
static const POINT3DF s_CAMERAPOS(0,1,-3); //  カメラの設置位置（注視はs_OBJECT1POSを見ている）



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

//    m_Model.Load( MAIDTEXT("box.mqo") );
//    m_Model.Load( MAIDTEXT("boxsphere.mqo") );
//    m_Model.Load( MAIDTEXT("boxspheretexture.mqo") );
    m_Model.Load( MAIDTEXT("boxspheretexturebump.mqo") );
    m_Field.Load( MAIDTEXT("field.mqo") );

    const SIZE2DF Screen = GetDefaultConfig().Graphics.ScreenSize;

    m_Camera.SetPerspective( DEGtoRAD(60.0f), Screen.w/Screen.h, s_CAMERA_NEAR, s_CAMERA_FAR );
    m_Camera.SetPosition( s_CAMERAPOS );
    m_Camera.SetTarget( s_OBJECT1POS );
    m_Camera.SetUpVector( VECTOR3DF(0,1,0) );

    m_ModelRotate.Set( DEGtoRAD(0), DEGtoRAD(360), 60 );
    m_LightRotate.Set( DEGtoRAD(0), DEGtoRAD(360), 240 );
    m_ModelAlpha.Set( 0.0f, 1.0f, 60 );

    m_Font.Create( SIZE2DI(8,16), true );
    m_IsLight = false;

    m_ShadowBuffer.Create( SIZE2DI(2048,2048), PIXELFORMAT_X08R08G08B08I );
    m_ShadowDepth.CreateCompatible( m_ShadowBuffer, PIXELFORMAT_D32I );

  }

  void UpdateFrame()
  {
    const Mouse& m = GetMouse();
    const Keybord& k = GetKeybord();

    if( m_2DRender.IsInitializing() ) { return ; }
    if( m_Render.IsInitializing() ) { return ; }
    if( m_Model.IsLoading() ) { return ; }
    if( m_Field.IsLoading() ) { return ; }

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
    if( m_Field.IsLoading() ) { return ; }


    m_Command.Begin();

    MATRIX4DF WLightP;
    VECTOR3DF LightDir;
    {
      const float rot = m_LightRotate;
      const float s = Math<float>::sin(rot);
      const float c = Math<float>::cos(rot);

      LightDir = VECTOR3DF(c,s,0).Normalize();
    }
    {
      //  シャドウマップの作成
      const RenderTargetBase& rt = m_ShadowBuffer;
      const IDepthStencil& ds = m_ShadowDepth;

      m_Command.SetRenderTarget( rt, ds );
      m_Command.ClearRenderTarget( COLOR_A32B32G32R32F(1,1,1,1) );
      m_Command.ClearDepth( 1.0f );


      const Camera&  nc = m_Camera;
      Camera  sc;  //  シャドウマップを作成するときに使うカメラ

      //  シャドウマップを作るために光源にカメラ(shadow camera=sc)を置くわけですが
      //  角度以外の設定をレンダリングに使うカメラ(normal camera=nc)から導く
      //  レンダリングに使うカメラの表示範囲をすっぽり覆う球をシャドウマップの範囲にする

      const VECTOR3DF camera_vec = VECTOR3DF(nc.GetPosition(),nc.GetTarget()).Normalize();
      const POINT3DF  center = nc.GetPosition() + (camera_vec * ((s_CAMERA_FAR-s_CAMERA_NEAR)/2));

      float CullR = 0;
      {
        //  ncの描画範囲の端っこは射影変換後の座標から逆算させる
        const VECTOR4DF pos_w = VECTOR4DF(1,1,1,1) * (nc.GetViewMatrix()*nc.GetProjectionMatrix()).GetInverse();
        const POINT3DF pos = POINT3DF(pos_w.x/pos_w.w, pos_w.y/pos_w.w, pos_w.z/pos_w.w );

        CullR = VECTOR3DF(center,pos).Length();
      }

      //  これで center を中心とした半径 CullR がすっぽり入るシャドウマップを作れる


//      const VECTOR4DF _pos_w = pos * (nc.GetViewMatrix()*nc.GetProjectionMatrix());
//      const VECTOR4DF _pos = VECTOR4DF(_pos_w.x/_pos_w.w, _pos_w.y/_pos_w.w, _pos_w.z/_pos_w.w, 1 );

      int k=0;

      const SIZE2DF size = static_cast<Texture2DBase&>(m_ShadowBuffer).GetTextureSize();
      const POINT3DF target = center;
      const POINT3DF pos = target - (LightDir*CullR);

//      sc.SetPerspective( DEGtoRAD(60.0f), size.w/size.h, 1.0f, CullR*2 );
      sc.SetPerspective( DEGtoRAD(60.0f), size.w/size.h, CullR*0.1f, CullR*2 );
      sc.SetPosition( pos );
      sc.SetTarget( target );
      sc.SetUpVector( VECTOR3DF(0,1,0) );

      m_Render.SetCamera(sc);

      WLightP = sc.GetViewMatrix() * sc.GetProjectionMatrix();

      m_Render.BltShadow1( WLightP, POINT3DF(0,0,0), m_Field );
      m_Render.BltShadow1R( WLightP, s_OBJECT1POS, m_Model, m_ModelRotate, VECTOR3DF(0,1,0) );
      m_Render.BltShadow1( WLightP, s_OBJECT2POS, m_Model );

    }
    {
      const RenderTargetBase& rt = GetGraphicsCore().GetBackBuffer();
      const IDepthStencil& ds = GetGraphicsCore().GetDepthStencil();

      m_Command.SetRenderTarget( rt, ds );
      m_Command.ClearRenderTarget( COLOR_A32B32G32R32F(1,0.5f,0,0) );
      m_Command.ClearDepthStencil( 1.0f, 0x00 );
      m_Render.SetCamera(m_Camera);

      {
        std::vector<LIGHT> LightList;
        if( m_IsLight )
        {
          LIGHT light;
          light.Type = LIGHT::DIRECTIONAL;
          light.Diffuse = COLOR_R32G32B32A32F(1,1,1,1);
          light.Direction = LightDir;

          LightList.push_back(light);
        }
        m_Render.SetLight( LightList );
        const float amb = 0.5f;
        m_Render.SetAmbient( COLOR_R32G32B32A32F(amb,amb,amb,1) );
      }

//void Graphics3DRender::BltShadow2( const MATRIX4DF& world, const ModelMQO& model, const MATRIX4DF& LightMat, const Texture2DBase& ShadowMap )

      //const VECTOR4DF pos_w = VECTOR4DF(1,1,1,1) * (nc.GetViewMatrix()*nc.GetProjectionMatrix()).GetInverse();

      m_Render.BltShadow2( MATRIX4DF().SetTranslate(0,0,0), m_Field, 1.0f, WLightP, m_ShadowBuffer );

//      m_Render.Blt( POINT3DF(0,0,0), m_Field, 1 );

      MATRIX4DF mat = MATRIX4DF().SetRotationY(m_ModelRotate)
        * MATRIX4DF().SetTranslate(s_OBJECT1POS.x,s_OBJECT1POS.y,s_OBJECT1POS.z)
        ;

      m_Render.BltShadow2( mat, m_Model, 1.0f, WLightP, m_ShadowBuffer );
//      m_Render.BltR( s_OBJECT1POS, m_Model, 1, m_ModelRotate, VECTOR3DF(0,1,0) );
//      m_Render.BltShadow2( MATRIX4DF().SetTranslate(0,0,0), m_Model, m_ModelAlpha, WLightP, m_ShadowBuffer );
//      m_Render.Blt( s_OBJECT2POS, m_Model, m_ModelAlpha );
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

    {
      const SIZE2DI size = static_cast<Texture2DBase&>(m_ShadowBuffer).GetSize();
      const RECT2DI rc( POINT2DI(0,0), size );
      const SIZE2DF scale( 200.0f/(float)size.w, 200.0f/(float)size.h );
      m_2DRender.BltS( POINT2DI(0,40), m_ShadowBuffer, rc, POINT2DI(0,0), 1, scale );
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
  ModelMQO          m_Field;

  RenderTargetTexture m_ShadowBuffer;
  DepthStencil        m_ShadowDepth;

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

