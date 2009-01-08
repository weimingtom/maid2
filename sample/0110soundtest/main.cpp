#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphicsrender.h"
#include"../../source/graphics/font.h"
#include"../../source/input/mouse.h"
#include"../../source/input/keybord.h"
#include"../../source/sound/soundfile.h"

using namespace Maid;

class MyApp : public IGameThread
{
protected:
  bool SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
  {
    return true;
  }

  void Initialize()
  {
    m_Render.Initialize();

    m_FontS.Create( SIZE2DI(16,32), true );

//    m_Sound.LoadFile( MAIDTEXT("tm2_bird001.wav") );
    m_Sound.LoadFile( MAIDTEXT("waru.wav") );
  }

  void UpdateFrame()
  {
    if( m_Render.IsInitializing() ) { return ; }
    if( m_Sound.IsLoading() ) { return ; }

    const Mouse& m = GetMouse();
    const Keybord& k = GetKeybord();


    if( k.IsIn('A') )
    {
      m_Sound.Play();
    }
    if( k.IsIn('B') )
    {
      m_Sound.Stop();
    }

  }

  void UpdateDraw()
  {
    m_Render.ClearRenderTarget( COLOR_A32B32G32R32F(1,0,0,0) );

    if( m_Render.IsInitializing() ) { return ; }

  }

  void Finalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  GraphicsRender  m_Render;
  Font            m_FontS;
  SoundFile       m_Sound;
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

