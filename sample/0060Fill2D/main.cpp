/*
  矩形塗りつぶしのサンプル
*/

#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphics2drender.h"
#include"../../source/graphics/graphicscommandcontrol.h"

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

  }

  void UpdateFrame()
  {
    //  フレーム毎に呼ばれるとこ
    //  終了する場合は OnExitGame() を呼ぶ

  }

  void UpdateDraw()
  {
    //  定期的に描画するとこ

    m_Command.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(1,0.5f,0,0) );
    if( m_Render.IsInitializing() ) { return ; }
    m_Command.Begin();
    {
      m_Render.SetVirtualScreenSize( SIZE2DI(800,600) );

      m_Render.SetDefaultSetting();

      m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_ALPHA );

      //  普通に青い四角を塗る
      m_Render.Fill( POINT2DI(600,300), Maid::COLOR_R32G32B32A32F(0,0,1,1), SIZE2DI(100,50), POINT2DI(50,25) );

      //  赤い四角を横に伸ばして塗る
      m_Render.FillS( POINT2DI(200,300), Maid::COLOR_R32G32B32A32F(1,0,0,1), SIZE2DI(100,50), POINT2DI(50,25), SIZE2DF(1.5f,1.0f) );

      {
        //  緑の四角を回転させて塗る(加算合成)
        m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_ADD );

        const POINT2DI pos(400,300);
        const SIZE2DI  size(200,200);
        const POINT2DI center(size.w/2,size.h/2);

        static int rot;
        rot++;
        rot %= 360;
        m_Render.FillR( pos, Maid::COLOR_R32G32B32A32F(0,1,0,1), size, center, DEGtoRAD(rot), Maid::VECTOR3DF(0,0,1) );

        //  青の四角を回転させて塗る(加算合成)

        m_Render.FillSR( pos, Maid::COLOR_R32G32B32A32F(0,0,1,1), size, center, SIZE2DF(2,0.3f), DEGtoRAD(-rot), Maid::VECTOR3DF(0,0,1) );
      }

    }
    m_Command.End();
  }

  void LocalFinalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  GraphicsCommandControl  m_Command;
  Graphics2DRender  m_Render;
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

