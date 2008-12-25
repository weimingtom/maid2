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

    m_Render.Initialize( param.DrawCommand[0] );

    m_Texture.LoadFile( MAIDTEXT("mo_005.bmp") );

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

    m_Render.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(1,0,0,0) );

    if( m_Render.IsLoading() || m_Texture.IsLoading() ) { return ; }

    const RECT2DI  rc( POINT2DI(0,0), m_Texture.GetImageSize() );
    const POINT2DI center(rc.w/2,rc.h/2);

    m_Render.Begin();

    //  矩形を普通に描画
		m_Render.Blt( POINT2DI(400,300), m_Texture, rc, center, 1.0f );

    //  横に伸ばして、縦に縮めて、不透明度５０％
		m_Render.BltS( POINT2DI(400,300), m_Texture, rc, center, 0.5f, SIZE2DF(1.5f,0.8f) );

    {

      static int rot;
      rot++;
      rot %= 360;
      //  (0,300)に回転させて描画(加算合成)
      m_Render.SetBlendState( GraphicsRender::BLENDSTATE_ADD );
      m_Render.BltR( POINT2DI(0,300), m_Texture, rc, center, 1.0f, DEGtoRAD(rot), Maid::VECTOR3DF(0,0,1) );

      //  (600,300)に大きさ50%,50%で回転させて描画(加算合成)
      m_Render.SetBlendState( GraphicsRender::BLENDSTATE_ADD );
      m_Render.BltSR( POINT2DI(600,300), m_Texture, rc, center, 1.0f, SIZE2DF(0.5f,0.5f), DEGtoRAD(-rot), Maid::VECTOR3DF(0,1,0) );
    }
    m_Render.End();
  }

  void LocalFinalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  GraphicsRender  m_Render;
  Texture2D       m_Texture;
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

