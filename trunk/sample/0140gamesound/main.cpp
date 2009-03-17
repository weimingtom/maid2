/*
  ゲームサウンドサンプル
  通常はこれをつかうといい
*/


#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics2drender.h"
#include"../../source/graphics/font.h"
#include"../../source/input/mouse.h"
#include"../../source/input/keybord.h"
#include"../../source/sound/gamesound.h"
#include"../../source/sound/gamesoundmanager.h"

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

    m_SoundManager.Initialize();

    m_Sound1.Load( GameSound::TYPE_SE,  MAIDTEXT("tm2_bird001.wav") );
    m_Sound2.Load( GameSound::TYPE_SE,  MAIDTEXT("nc7802.ogg") );
  }

  void UpdateFrame()
  {
    if( m_Render.IsInitializing() ) { return ; }

    const Mouse& m = GetMouse();
    const Keybord& k = GetKeybord();

    m_SoundManager.UpdateFrame();

    if( k.IsIn('A') )
    {
      m_Sound1.Play();
    }
    if( k.IsIn('B') )
    {
      m_Sound1.Stop();
    }
    if( k.IsIn('C') )
    {
      m_SoundManager.SetVolume( GameSoundManager::TYPE_SE, 1.0f );
    }

    if( k.IsIn('H') )
    {
      m_Sound2.Play();
    }
    if( k.IsIn('I') )
    {
      m_Sound2.Stop();
    }
    if( k.IsIn('J') )
    {
      m_SoundManager.SetVolume( GameSoundManager::TYPE_BGM, 0.0f );
    }
    if( k.IsIn('K') )
    {
      m_SoundManager.SetVolume( GameSoundManager::TYPE_BGM, 1.0f );
    }
  }

  void UpdateDraw()
  {
    m_Render.ClearRenderTarget( COLOR_A32B32G32R32F(1,0,0,0) );

    if( m_Render.IsInitializing() ) { return ; }

    m_Render.Begin();
    {

      {
        char buf[256];
        sprintf( buf, "se pos:%f ", m_Sound1.GetPosition() );
        std::string str(buf);

        if( m_Sound1.IsPlay() ) { str += "再生中"; }
        else                      { str += "停止中"; }

        m_Render.BltText( POINT2DI(0,  0), m_FontS, String::ConvertSJIStoMAID(str), COLOR_R32G32B32A32F(1,1,1,1) );
      }

      {
        char buf[256];
        sprintf( buf, "bgm pos:%f vol:%f", m_Sound2.GetPosition(), m_Sound2.GetVolume() );
        std::string str(buf);

        if( m_Sound2.IsPlay() ) { str += "再生中"; }
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
  GameSoundManager   m_SoundManager;
  GameSound          m_Sound1;
  GameSound          m_Sound2;
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

