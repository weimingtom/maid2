//#define LOCAL_PROFILE

/*
  ムービーの再生(コーデックは ogg theora&vorbis)
*/

#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics2drender.h"

#include"../../source/graphics/texture2dmemory.h"
#include"../../source/sound/pcmwriter.h"

#include"../../source/framework/movie/movieplayer.h"

#include"../../source/auxiliary/debug/trace.h"
#include"../../source/auxiliary/debug/profile.h"

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
    m_Render.Initialize();
    m_Player.Initialize( MAIDTEXT("nc4259.ogv") );
//    m_Player.Initialize( MAIDTEXT("op_800x600v10.ogv") );

    m_Font.Create( SIZE2DI(16,32), true );
    m_Frame = 0;
    m_State = STATE_INITIALIZING;
  }

  void UpdateFrame()
  {
    const Mouse& m = GetMouse();
    const Keybord& k = GetKeybord();

    switch( m_State )
    {
    case STATE_INITIALIZING:
      {
        if( m_Player.IsStandby() )
        {
          m_Info = m_Player.GetFileInfo();

          if( m_Info.IsImage )
          {
            m_Texture.Create( m_Info.Image.EncodedSize, PIXELFORMAT_R08G08B08I );
          }

          if( m_Info.IsPCM )
          {
            m_Sound.Initialize( m_Info.Pcm.Format, 1.0f );
            m_Sound.SetVolume(1.0f);
          }

          m_State = STATE_PLAING;

          //  画像のエンコードは重いので、描画の時に行う
          //  サウンドは毎回やる
          UpdateTexture();
          UpdateSound();
          m_Frame = 0;
          m_Sound.Play();
          m_Player.Play();
        }
      }break;

    case STATE_PLAING:
      {
        //  画像のデコードは重いので、描画の時に行う
        //  サウンドは毎回やる

        if( m_Player.IsEnd() )
        {
          //  末端まできたら終了
          m_Sound.Stop();
          m_State = STATE_END;
        }
        ++m_Frame;
        if( k.IsIn( Keybord::BUTTON_SPACE) )
        {
          //  スペースを押したら20秒にシーク
          m_Sound.Stop();
          const int frame = 1200;
          m_Player.Stop();
          m_Player.SetPosition( 20.0 );
          m_Frame = frame;
          m_State = STATE_INITIALIZING;
        }else
        {

          UpdateTexture();
          UpdateSound();
        }
      }break;

    case STATE_END:
      {

      }break;
    }
  }

  void UpdateDraw()
  {
    //  定期的に描画するとこ

    m_Command.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(1,0.5f,0,0) );

    if(  m_Render.IsInitializing() ) { return ; }

    m_Command.Begin();

    switch( m_State )
    {
    case STATE_INITIALIZING:
      {
        String text = String::PrintFormat( "初期化中" );
        m_Render.BltText( POINT2DI(0,300), m_Font, text, COLOR_R32G32B32A32F(0,1,0,1) );

      }break;

    case STATE_PLAING:
      {
        const RECT2DI rc( POINT2DI(0,0), m_Texture.GetSize() );
        const POINT2DI off(rc.w/2,rc.h/2 );
        m_Render.Blt( POINT2DI(400,300), m_Texture, rc, off, 1 );

        String text = String::PrintFormat( "%f", m_Player.GetPosition() );
        m_Render.BltText( POINT2DI(0,300), m_Font, text, COLOR_R32G32B32A32F(0,1,0,1) );
      }break;

    case STATE_END:
      {
        const RECT2DI rc( POINT2DI(0,0), m_Texture.GetSize() );
        const POINT2DI off(rc.w/2,rc.h/2 );
        m_Render.Blt( POINT2DI(400,300), m_Texture, rc, off, 1 );

        String text = String::PrintFormat( "終了！" );
        m_Render.BltText( POINT2DI(0,300), m_Font, text, COLOR_R32G32B32A32F(0,1,0,1) );
      }break;
    }

    m_Command.End();
  }

  void Finalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  void UpdateTexture()
  {
    if( !m_Info.IsImage ) { return ; }

    Movie::SPSAMPLEFRAME pImage;
    double time;
    m_Player.FlushImage( time, pImage );

    if( pImage.get()==NULL ) { return ; }

//    MAID_WARNING( "time:" << time <<" frame:" << m_Frame );
    m_Command.UpdateTexture( m_Texture, pImage );
  }

  void UpdateSound()
  {
    if( !m_Info.IsPCM ) { return ; }

    MemoryBuffer buf;
    double time;
    m_Player.FlushPCM( time, buf );

    m_Sound.Write( buf.GetPointer(0), buf.GetSize() );
  }

private:
  GraphicsCommandControl  m_Command;
  Graphics2DRender  m_Render;


  int m_Frame;

  enum STATE
  {
    STATE_INITIALIZING, //  初期化中
    STATE_PLAING,       //  再生中
    STATE_END,         //  再生終了
  };

  STATE m_State;


  Texture2DDynamic m_Texture;
  PCMWriter   m_Sound;
  MoviePlayer   m_Player;
  MoviePlayer::FILEINFO m_Info;

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

