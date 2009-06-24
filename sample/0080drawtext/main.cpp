﻿/*
  テキスト表示サンプル
*/

#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics2drender.h"
#include"../../source/graphics/graphicscommandcontrol.h"

#include"../../source/graphics/font.h"

#include"startupdialog.h"


using namespace Maid;

class MyApp : public IGameThread
{
public:
  MyApp()
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

    m_FontS.Create( SIZE2DI( 8,16), false );
    m_FontM.Create( SIZE2DI(16,32), true );
    m_FontL.Create( SIZE2DI(32,64), true );

    m_FrameCount = 0;
    m_DrawCount  = 0;
  }

  void UpdateFrame()
  {
    //  フレーム毎に呼ばれるとこ
    //  終了する場合は OnExitGame() を呼ぶ
    if( m_Render.IsInitializing() ) { return ; }

    m_Render.UpdateFrame();
    ++m_FrameCount;
  }

  void UpdateDraw()
  {
    //  定期的に描画するとこ

    m_Command.ClearRenderTarget( COLOR_A32B32G32R32F(1,0,0,0) );

    if( m_Render.IsInitializing() ) { return ; }

    m_Command.Begin();
    {

      char buf[256];
      sprintf( buf, "frame:%04d,draw:%04d", m_FrameCount, m_DrawCount );

      m_Render.BltText( POINT2DI(0,  0), m_FontS, String::ConvertSJIStoMAID(buf), COLOR_R32G32B32A32F(1,1,1,1) );
      m_Render.BltText( POINT2DI(0,100), m_FontS, MAIDTEXT("テキスト表示"), COLOR_R32G32B32A32F(1,1,1,1) );
      m_Render.BltText( POINT2DI(0,200), m_FontM, MAIDTEXT("色かえもＯＫ"), COLOR_R32G32B32A32F(1,0,1,1) );
      m_Render.BltText( POINT2DI(0,300), m_FontM, MAIDTEXT("半透明もできる"), COLOR_R32G32B32A32F(1,0,1,0.5f) );
      m_Render.BltText( POINT2DI(0,400), m_FontL, MAIDTEXT("改行も\nサポート"), COLOR_R32G32B32A32F(0,1,1,1) );
    }
    m_Command.End();
    ++m_DrawCount;
  }

  void Finalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  GraphicsCommandControl  m_Command;
  Graphics2DRender  m_Render;
  Font          m_FontS;
  Font          m_FontM;
  Font          m_FontL;

  int   m_FrameCount;
  int   m_DrawCount;
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

