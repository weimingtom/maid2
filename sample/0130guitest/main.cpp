#include"../../source/framework/gameframework.h"
#include"../../source/framework/gui.h"
#include"../../source/auxiliary/collision.h"
#include"../../source/graphics/graphicsrender.h"
#include"../../source/graphics/font.h"

#include"../../source/graphics/texture2d.h"
#include"../../source/graphics/rendertargetscreen.h"
#include"../../source/graphics/depthstencil.h"

using namespace Maid;
/*
  class GUIButton : public IGUIParts
  {
  public:
    GUIButton();

    bool IsButtonDown() const { return m_IsButtonDown; }

  protected:
    virtual void LocalSendMessage( const IGUIParam& Param );

    virtual void OnMouseIn( const POINT2DI& pos ){}
    virtual void OnMouseOut( const POINT2DI& pos ){}

    virtual void OnMouseClick( const POINT2DI& pos ){}

  private:
    bool  m_IsMouseIn;
    bool  m_IsButtonDown;
*/

class Button : public GUIButton
{
  GraphicsRender&  m_Render;
  SIZE2DI m_Size;
  COLOR_R32G32B32A32F m_Color;
public:
    Button( GraphicsRender& r )
      :m_Render( r )
    {

    }

protected:

    virtual void OnInitialize( const Mouse* pMouse, const Keybord* pKey )
    {
      m_hFont.Create( SIZE2DI(16,32), true );
      m_Size = SIZE2DI(100,100);
      m_Text = MAIDTEXT("通常");
      m_Color= COLOR_R32G32B32A32F(1,0,0,1);
    }
    virtual void OnFinalize(){}

    virtual bool LocalIsMouseIn( const POINT2DI& pos ) const
    {
      return Collision<int>::IsPointToRect( pos, RECT2DI(POINT2DI(0,0),m_Size) );
    }

    virtual void LocalUpdateFrame(){}
    virtual void LocalUpdateDraw( RenderTargetBase& Target, IDepthStencil& Depth, const VECTOR2DI& offset  )
    {
      const POINT2DI pos = POINT2DI(0,0)+offset;
      m_Render.Fill( pos, m_Color, m_Size, POINT2DI(0,0) );
      m_Render.BltText( pos, m_hFont, m_Text, COLOR_R32G32B32A32F(1,1,1,1) );
    }

    virtual void OnMouseIn( const POINT2DI& pos )
    {
      if( IsButtonDown() )
      {
        m_Text = MAIDTEXT("押されている");
      }else
      {
        m_Text = MAIDTEXT("カーソルが入った");
      }
    }

    virtual void OnMouseOut( const POINT2DI& pos )
    {
      if( IsButtonDown() )
      {
        m_Text = MAIDTEXT("押されているまま出た");
      }else
      {
        m_Text = MAIDTEXT("通常");
      }
    }

    virtual void OnMouseDown( const POINT2DI& pos, int Button )
    {
      m_Text = MAIDTEXT("押されている");
    }

    virtual void OnMouseUp( const POINT2DI& pos, int Button )
    {
      if( IsMouseIn() )
      {
        m_Text = MAIDTEXT("カーソルが入った");
      }else
      {
        m_Text = MAIDTEXT("通常");
      }
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
      Button* pButton = new Button(m_Render);

      pButton->SetPosition( POINT2DI(100,100) );

      m_Manager.RegisterParts( 1, SPGUIPARTS(pButton) );
    }

    {
      Button* pButton = new Button(m_Render);

      pButton->SetPosition( POINT2DI(400,100) );

      m_Manager.RegisterParts( 2, SPGUIPARTS(pButton) );
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

