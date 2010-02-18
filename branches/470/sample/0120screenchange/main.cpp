/*
  スクリーンモード変更サンプル

  Aを押すと 640x480 32Exbpp
  Bを押すと 1024x768 32bpp
  Cを押すと フルスクリーン化
  Dを押すと ウィンドウ化

  スペースキーでアスペクト比を調節する
  画面フォーマットが対応してない場合は無視されます
*/


#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics2drender.h"

#include"../../source/graphics/texture2d.h"

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
    GraphicsCore& core = GetGraphicsCore();

    core.SetTextureQuality( 2 );

    m_Command.Initialize();
    m_Render.Initialize();

    m_AspectCount = 0;
    m_Font.Create( SIZE2DI(8,16), true );

    m_Texture.LoadFile( MAIDTEXT("nc1673.bmp") );
  }

  void UpdateFrame()
  {
    const Mouse& m = GetMouse();
    const Keybord& k = GetKeybord();
    GraphicsCore& core = GetGraphicsCore();

    if( k.IsDown('A') )
    {
      GraphicsCore::SWAPCHAINFORMAT fmt;
      fmt.Size = SIZE2DI(640,480);
      fmt.Type = GraphicsCore::SWAPCHAINFORMAT::BPP32EX;
      fmt.RefreshRate = 0;
      fmt.IsWaitVSync = true;
      core.SetSwapChainFormat( fmt );
    }
    else if( k.IsDown('B') )
    {
      GraphicsCore::SWAPCHAINFORMAT fmt;
      fmt.Size = SIZE2DI(1024,768);
      fmt.Type = GraphicsCore::SWAPCHAINFORMAT::BPP32;
      fmt.RefreshRate = 0;
      fmt.IsWaitVSync = true;
      core.SetSwapChainFormat( fmt );
    }
    else if( k.IsDown('C') )
    {
      SetFullScreenState( true );
    }
    else if( k.IsDown('D') )
    {
      SetFullScreenState( false );
    }
    else if( k.IsIn(Keybord::BUTTON_SPACE) )
    {
      ++m_AspectCount;
      bool IsLock;
      const int no = m_AspectCount%2;

      if( no==0 ) { IsLock = false; }
      else if( no==1 ) { IsLock = true;  }

      SetAspectLock( IsLock );
    }

  }

  void UpdateDraw()
  {
    GraphicsCore& core = GetGraphicsCore();

    //  定期的に描画するとこ
    m_Command.SetRenderTarget( GetGraphicsCore().GetBackBuffer() );
    m_Command.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(0,0.5f,0,0) );

    if( m_Render.IsInitializing() || m_Texture.IsLoading() ) { return ; }

    const RECT2DI  rc( POINT2DI(0,0), m_Texture.GetImageSize() );
    const POINT2DI center(rc.w/2,rc.h/2);

    m_Command.Begin();

    //  矩形を普通に描画
		m_Render.Blt( POINT2DI(400,300), m_Texture, rc, center, 1.0f );

    //  横に伸ばして、縦に縮めて、不透明度５０％
		m_Render.BltS( POINT2DI(400,100), m_Texture, rc, center, 0.5f, SIZE2DF(1.5f,0.8f) );

    //  25%
		m_Render.BltS( POINT2DI(400,500), m_Texture, rc, center, 1.0f, SIZE2DF(0.25f,0.25f) );
    {
      static int rot;
      rot++;
      rot %= 360;
      //  (0,300)に回転させて描画(加算合成)
      m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_ADD );
      m_Render.BltR( POINT2DI(0,300), m_Texture, rc, center, 1.0f, DEGtoRAD(rot), Maid::VECTOR3DF(0,0,1) );

      //  (600,300)に大きさ50%,50%で回転させて描画(加算合成)
      m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_ADD );
      m_Render.BltSR( POINT2DI(600,300), m_Texture, rc, center, 1.0f, SIZE2DF(0.5f,0.5f), DEGtoRAD(-rot), Maid::VECTOR3DF(0,1,0) );
    }

    {
      m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_ALPHA );
      const GraphicsCore::SWAPCHAINFORMAT& fmt = core.GetSwapChainFormat();

      String text;

      switch( fmt.Type )
      {
      case GraphicsCore::SWAPCHAINFORMAT::BPP16:   { text += MAIDTEXT(" BPP:16"); }break;
      case GraphicsCore::SWAPCHAINFORMAT::BPP32:   { text += MAIDTEXT(" BPP:32"); }break;
      case GraphicsCore::SWAPCHAINFORMAT::BPP32EX: { text += MAIDTEXT(" BPP:32EX"); }break;
      }

      text += String::PrintFormat( " SIZE:%dx%d", fmt.Size.w, fmt.Size.h );
      text += String::PrintFormat( " FPS:%d", fmt.RefreshRate );
      text += String::PrintFormat( " VSync:%s", fmt.IsWaitVSync? "yes" : "no" );

      if( core.IsFullScreen() ) { text += MAIDTEXT("\n FullScreen:"); }
      else                      { text += MAIDTEXT("\n WindowMode:"); }

      const Mouse& m = GetMouse();
      text += String::PrintFormat( "\n %d %d", m.GetX(), m.GetY() );

      m_Render.BltText( POINT2DI(0,0), m_Font, text, COLOR_R32G32B32A32F(1,1,1,1) );
    }
    m_Command.End();
  }

  void Finalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  GraphicsCommandControl  m_Command;
  Graphics2DRender  m_Render;
  Texture2D       m_Texture;
  int             m_AspectCount;
  Font            m_Font;
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

