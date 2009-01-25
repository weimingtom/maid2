/*
  3Dモデルの描画
*/


#include"../../source/framework/gameframework.h"
#include"../../source/graphics/graphicsrender.h"

#include"../../source/graphics/modelmqo.h"

using namespace Maid;

class MyApp : public IGameThread
{
protected:
  bool SelectDevice( const std::vector<Graphics::IDeviceList::INFO>& GraphicsDevice, DEFAULTCONFIG& conf )
  {
     return true;
  }

  void Initialize()
  {
    m_Render.Initialize();

    m_Model.Load( MAIDTEXT("box.mqo") );
  }

  void UpdateFrame()
  {
    const Mouse& m = GetMouse();
    const Keybord& k = GetKeybord();
    GraphicsCore& core = GetGraphicsCore();


  }

  void UpdateDraw()
  {
    //  定期的に描画するとこ
    m_Render.SetRenderTarget( GetGraphicsCore().GetBackBuffer() );
    m_Render.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(0,0,0,0) );

    if( m_Render.IsInitializing() || m_Model.IsLoading() ) { return ; }

    m_Render.Begin();

/*
    const std::vector<MQOMATERIAL>& GetMaterialList() const;
    const std::vector<boost::shared_ptr<MQOOBJECT> >& GetObjectList() const;
*/

    m_Render.End();
  }

  void Finalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  GraphicsRender  m_Render;
  ModelMQO        m_Model;
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

