#include"../../source/framework/gameframework.h"
#include"../../source/framework/gui.h"
#include"../../source/auxiliary/collision.h"
#include"../../source/graphics/graphicsrender.h"
#include"../../source/graphics/font.h"

#include"../../source/graphics/texture2d.h"
#include"../../source/graphics/rendertargetscreen.h"
#include"../../source/graphics/depthstencil.h"

using namespace Maid;


class Listner : public GUIButton::Listner
{
  GraphicsRender&  m_Render;
  SIZE2DI m_Size;
  COLOR_R32G32B32A32F m_Color;
public:
    Listner( GraphicsRender& r )
      :m_Render( r )
    {

    }

    virtual void Initialize()
    {
      m_hFont.Create( SIZE2DI(16,32), true );
      m_Size = SIZE2DI(100,100);
      m_Text = MAIDTEXT("通常");
      m_Color= COLOR_R32G32B32A32F(1,0,0,1);
    }

    virtual bool IsMouseIn( const POINT2DI& pos ) const
    {
      return Collision<int>::IsPointToRect( pos, RECT2DI(POINT2DI(0,0),m_Size) );
    }

    virtual void UpdateFrame(){}
    virtual void UpdateDraw( RenderTargetBase& Target, IDepthStencil& Depth, const POINT2DI& pos )
    {
      m_Render.Fill( pos, m_Color, m_Size, POINT2DI(0,0) );
      m_Render.BltText( pos, m_hFont, m_Text, COLOR_R32G32B32A32F(1,1,1,1) );
    }

    virtual void MouseIn( const POINT2DI& pos )
    {
      if( IsButtonDown() )
      {
        m_Text = MAIDTEXT("押されている");
      }else
      {
        m_Text = MAIDTEXT("カーソルが入った");
      }
    }

    virtual void MouseOut( const POINT2DI& pos )
    {
      if( IsButtonDown() )
      {
        m_Text = MAIDTEXT("押されているまま出た");
      }else
      {
        m_Text = MAIDTEXT("通常");
      }
    }

    virtual void MouseDown( const POINT2DI& pos, int Button )
    {
      m_Text = MAIDTEXT("押されている");
    }

    virtual void MouseUp( const POINT2DI& pos, int Button )
    {
      m_Text = MAIDTEXT("通常");
    }

private:
  Font m_hFont;
  String  m_Text;
};

class MyApp : public IGameThread
{
protected:
  bool SelectDevice( const std::vector<Graphics::IDeviceList::INFO>& GraphicsDevice, DEFAULTCONFIG& conf )
  {
     return true;
  }

  void Initialize()
  {
    const Mouse& m = GetMouse();
    const Keybord& k = GetKeybord();

    m_Render.Initialize();
    m_Manager.Initialize( &m, &k );

    {
      GUIButton* pButton = new GUIButton;

      pButton->SetListner( SPGUIBUTTONLISTNER( new Listner(m_Render) ) );
      pButton->SetID( 1 );
      pButton->SetPosition( POINT2DI(100,100) );

      m_Manager.RegisterParts( SPGUIPARTS(pButton) );
    }

    {
      GUIButton* pButton = new GUIButton;

      pButton->SetListner( SPGUIBUTTONLISTNER( new Listner(m_Render) ) );
      pButton->SetID( 2 );
      pButton->SetPosition( POINT2DI(400,100) );

      m_Manager.RegisterParts( SPGUIPARTS(pButton) );
    }
  }

  void UpdateFrame()
  {
    const Mouse& m = GetMouse();
    const Keybord& k = GetKeybord();
    GraphicsCore& core = GetGraphicsCore();

    if( m_Render.IsInitializing() ) { return ; }

    m_Manager.UpdateFrame();
  }

  void UpdateDraw()
  {
    //  定期的に描画するとこ

    m_Render.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(1,0,0,0) );

    if( m_Render.IsInitializing() ) { return ; }

    m_Render.Begin();

    RenderTargetScreen rt;
    DepthStencil ds;
    m_Manager.UpdateDraw(rt,ds);

/*

    //  矩形を普通に描画
		m_Render.Blt( POINT2DI(400,300), m_Texture, rc, center, 1.0f );

    //  横に伸ばして、縦に縮めて、不透明度５０％
		m_Render.BltS( POINT2DI(400,100), m_Texture, rc, center, 0.5f, SIZE2DF(1.5f,0.8f) );

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
*/
    m_Render.End();
  }

  void Finalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  GraphicsRender  m_Render;
  GUIManager      m_Manager;
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

