/*
  サウンド再生サンプル
  0140gamesound を見るほうがいいかも
*/

#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics2drender.h"
#include"../../source/graphics/font.h"
#include"../../source/input/mouse.h"
#include"../../source/input/keybord.h"
#include"../../source/sound/soundfile.h"

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

    m_FontS.Create( SIZE2DI(16,32), true );

    m_SoundSE.LoadFile( MAIDTEXT("tm2_bird001.wav") );
    m_SoundBGM.LoadFile( MAIDTEXT("waru.wav") );
  }

  void UpdateFrame()
  {
    if( m_Render.IsInitializing() ) { return ; }
    if( m_SoundSE.IsLoading() ) { return ; }
    if( m_SoundBGM.IsLoading() ) { return ; }

    const Mouse& m = GetMouse();
    const Keybord& k = GetKeybord();


    if( k.IsIn('A') )
    {
      m_SoundBGM.Play();
    }
    if( k.IsIn('B') )
    {
      m_SoundBGM.Stop();
    }
    if( k.IsIn('C') )
    {
      m_SoundBGM.SetPosition( 10.0f );
    }

    if( k.IsIn('H') )
    {
      m_SoundSE.Play();
    }
    if( k.IsIn('I') )
    {
      m_SoundSE.Stop();
    }
    if( k.IsIn('J') )
    {
      m_SoundSE.SetPosition( 3.0f );
    }
  }

  void UpdateDraw()
  {
    m_Render.ClearRenderTarget( COLOR_A32B32G32R32F(1,0,0,0) );

    if( m_Render.IsInitializing() ) { return ; }
    if( m_SoundSE.IsLoading() ) { return ; }
    if( m_SoundBGM.IsLoading() ) { return ; }

    m_Render.Begin();

    {
      {
        char buf[256];
        sprintf( buf, "bgm pos:%f ", m_SoundBGM.GetPosition() );
        std::string str(buf);

        if( m_SoundBGM.IsPlay() ) { str += "再生中"; }
        else                      { str += "停止中"; }

        m_Render.BltText( POINT2DI(0,  0), m_FontS, String::ConvertSJIStoMAID(str), COLOR_R32G32B32A32F(1,1,1,1) );
      }

      {
        char buf[256];
        sprintf( buf, "se pos:%f ", m_SoundSE.GetPosition() );
        std::string str(buf);

        if( m_SoundSE.IsPlay() ) { str += "再生中"; }
        else                      { str += "停止中"; }

        m_Render.BltText( POINT2DI(0,  100), m_FontS, String::ConvertSJIStoMAID(str), COLOR_R32G32B32A32F(1,1,1,1) );
      }
    }
    m_Render.End();

  }

  void Finalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  Graphics2DRender  m_Render;
  Font            m_FontS;
  SoundFile       m_SoundBGM;
  SoundFile       m_SoundSE;
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

