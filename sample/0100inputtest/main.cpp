#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphicsrender.h"
#include"../../source/input/mouse.h"
#include"../../source/input/keybord.h"

using namespace Maid;

class MyApp : public IGameThread
{
protected:
  bool LocalSelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
  {
    return true;
  }

  void LocalInitialize( const BEGINPARAM& param )
  {
    m_Mouse.Initialize( param.pMouseDevice );
    m_Keybord.Initialize( param.pKeybordDevice );
    m_Render.Initialize( param.ScreenSize, param.DrawCommand[0], param.pFontDevice );

    m_pFontS = param.pFontDevice->CreateDefaultFont( SIZE2DI(16,32) );

  }

  void LocalUpdateFrame()
  {
    if( m_Render.IsInitializing() ) { return ; }

    m_Mouse.Update();
    m_Keybord.Update();
  }

  void LocalUpdateDraw()
  {
    m_Render.ClearRenderTarget( COLOR_A32B32G32R32F(1,0,0,0) );

    if( m_Render.IsInitializing() ) { return ; }

    m_Render.Begin();

    char buf[256];
    sprintf( buf, "MouseX:%04d,MouseY:%04d", m_Mouse.GetX(), m_Mouse.GetY() );
    m_Render.BltText( POINT2DI(0,0), m_pFontS, String::ConvertSJIStoMAID(buf), COLOR_R32G32B32A32F(1,1,1,1) );

    sprintf( buf, "DeltaX:%04d,DeltaY:%04d", m_Mouse.GetDeltaX(), m_Mouse.GetDeltaY() );
    m_Render.BltText( POINT2DI(0,50), m_pFontS, String::ConvertSJIStoMAID(buf), COLOR_R32G32B32A32F(1,1,1,1) );

    if( m_Mouse.IsDownL() )
    {
      m_Render.BltText( POINT2DI(0,100), m_pFontS, MAIDTEXT("左が押されています"), COLOR_R32G32B32A32F(1,1,1,1) );
    }
    if( m_Mouse.IsInL() )
    {
      m_Render.BltText( POINT2DI(0,150), m_pFontS, MAIDTEXT("左が押されました"), COLOR_R32G32B32A32F(1,1,1,1) );
    }
    if( m_Keybord.IsDown('A') )
    {
      m_Render.BltText( POINT2DI(0,200), m_pFontS, MAIDTEXT("Aが押されています"), COLOR_R32G32B32A32F(1,1,1,1) );
    }
    if( m_Keybord.IsIn('B') )
    {
      m_Render.BltText( POINT2DI(0,250), m_pFontS, MAIDTEXT("Bが押されました"), COLOR_R32G32B32A32F(1,1,1,1) );
    }

    m_Render.End();
  }

  void LocalFinalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  GraphicsRender  m_Render;
  Mouse           m_Mouse;
  Keybord         m_Keybord;
  SPFONT          m_pFontS;
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

