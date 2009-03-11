/*
  スクリーンモード変更サンプル

  Aを押すと フルスクリーン 640x480 32bpp
  Bを押すと フルスクリーン 1024x768 32bpp
  Cを押すと ウィンドウ 640x480 32bpp
  Dを押すと ウィンドウ 800x600 32bpp

  スペースキーでアスペクト比を調節する
*/


#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics2drender.h"

#include"../../source/graphics/texture2d.h"

#include"startupdialog.h"


using namespace Maid;

class MyApp : public IGameThread
{
protected:
  bool SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
  {
    const RETURNCODE ret = BeginStartupInfo( DeviceList, conf );
    return ret==GAMESTART;
  }

  void Initialize()
  {
    m_Render.Initialize();

    m_AspectCount = 0;

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
      fmt.Type = GraphicsCore::SWAPCHAINFORMAT::BPP32;
      fmt.RefreshRate = 0;
      fmt.IsWaitVSync = true;
      core.SetSwapChainFormat( fmt );
    }
#if 0
    if( k.IsDown('B') )
    {
      GraphicsCore::SCREENMODE mode;
      mode.Format.Size = SIZE2DI(1024,768);
      mode.Format.Type = GraphicsCore::DISPLAYFORMAT::BPP32;
      mode.Format.RefreshRate = 0;
      mode.IsFullScreen = true;
      mode.IsWaitVSync = true;
      core.SetScreenMode( mode );
    }

    if( k.IsDown('C') )
    {
      GraphicsCore::SCREENMODE mode;
      mode.Format.Size = SIZE2DI(640,480);
      mode.Format.Type = GraphicsCore::DISPLAYFORMAT::BPP32;
      mode.Format.RefreshRate = 0;
      mode.IsFullScreen = false;
      mode.IsWaitVSync = true;
      core.SetScreenMode( mode );
    }
    if( k.IsDown('D') )
    {
      GraphicsCore::SCREENMODE mode;
      mode.Format.Size = SIZE2DI(800,600);
      mode.Format.Type = GraphicsCore::DISPLAYFORMAT::BPP32;
      mode.Format.RefreshRate = 0;
      mode.IsFullScreen = false;
      mode.IsWaitVSync = true;
      core.SetScreenMode( mode );
    }

    if( k.IsIn(Keybord::BUTTON_SPACE) )
    {
      ++m_AspectCount;
      bool IsLock;
      const int no = m_AspectCount%2;

      if( no==0 ) { IsLock = false; }
      ef( no==1 ) { IsLock = true;  }

      GetGraphicsCore().SetAspectLock( IsLock );

    }
#endif

  }

  void UpdateDraw()
  {
    //  定期的に描画するとこ
    m_Render.SetRenderTarget( GetGraphicsCore().GetBackBuffer() );
    m_Render.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(0,0.5f,0,0) );

    if( m_Render.IsInitializing() || m_Texture.IsLoading() ) { return ; }

    const RECT2DI  rc( POINT2DI(0,0), m_Texture.GetImageSize() );
    const POINT2DI center(rc.w/2,rc.h/2);

    m_Render.Begin();

    //  矩形を普通に描画
		m_Render.Blt( POINT2DI(400,300), m_Texture, rc, center, 1.0f );

    //  横に伸ばして、縦に縮めて、不透明度５０％
		m_Render.BltS( POINT2DI(400,100), m_Texture, rc, center, 0.5f, SIZE2DF(1.5f,0.8f) );

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
    m_Render.End();
  }

  void Finalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  Graphics2DRender  m_Render;
  Texture2D       m_Texture;
  int             m_AspectCount;
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

