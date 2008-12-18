#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphicsrender.h"
#include"../../source/graphics/vertexshader.h"

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

    const char* vs =
      "vs.1.1\n"
//      "dcl_position v0\n"
//      "mov oPos, v0\n"
      ;

    m_Shader.Load( String::ConvertSJIStoMAID(vs) );
  }

  void LocalUpdateFrame( float now, float delta )
  {
    //  フレーム毎に呼ばれるとこ
    //  終了する場合は OnExitGame() を呼ぶ

//    IVertexShader& v = m_Shader;
    VertexShaderBase& v = m_Shader;
    VertexShader a = m_Shader;
    VertexShader b;
    b= a;
    if( b.IsLoading() )
    {
      int i=0;

    }
  }

  void LocalUpdateDraw( const IRenderTarget& target,float now, float delta )
  {
    //  定期的に描画するとこ
    //  描画先は target 

    m_Render.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(1,0.5f,0,0) );

  }

  void LocalFinalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  GraphicsRender  m_Render;
  VertexShader    m_Shader;
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

