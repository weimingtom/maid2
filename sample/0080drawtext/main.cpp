#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphicsrender.h"

#include"../../source/graphics/texture2d.h"

using namespace Maid;

class MyApp : public IGameThread
{
protected:
  bool LocalSelectDevice( const std::vector<Graphics::IDeviceList::INFO>& GraphicsDevice, DEFAULTCONFIG& conf )
  {
    //  ここでは引数で渡されたデバイスリストをユーザーに選択させる処理を書く
    //  ユーザーの選択結果は conf に設定するんだけど、設定しなくても自動的になんとかしてくれる

    //  ここでアプリケーションを終了したいときは return false です。
    return true;
  }

  void LocalInitialize( const BEGINPARAM& param )
  {
    //  LocalSelectDevice() で指定した設定で、各種デバイスデータが param に入ってます
    //  なので、もらったデータを元に、初期化する

    m_Render.Initialize( param.DrawCommand[0], param.pFontDevice );

    m_pFont = param.pFontDevice->CreateDefaultFont( SIZE2DI(16,32) );
  }

  void LocalUpdateFrame( float now, float delta )
  {
    //  フレーム毎に呼ばれるとこ
    //  終了する場合は OnExitGame() を呼ぶ

  }

  void LocalUpdateDraw( const IRenderTarget& target,float now, float delta )
  {
    //  定期的に描画するとこ
    //  描画先は target 

    m_Render.ClearRenderTarget( COLOR_A32B32G32R32F(1,0.5f,0,0) );

    if( m_Render.IsLoading() ) { return ; }

    m_Render.Begin();

    {
      const COLOR_A32B32G32R32F c(1,1,1,1);
      m_Render.BltText( POINT2DI(0,300), m_pFont, MAIDTEXT("テキスト表示"), c );

    }
    m_Render.End();
  }

  void LocalFinalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  GraphicsRender  m_Render;
  SPFONT          m_pFont;
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

