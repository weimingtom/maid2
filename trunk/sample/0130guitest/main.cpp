#include"../../source/framework/gameframework.h"
#include"../../source/framework/gui.h"
#include"../../source/auxiliary/collision.h"
#include"../../source/graphics/graphicsrender.h"
#include"../../source/graphics/font.h"

#include"../../source/graphics/texture2d.h"
#include"../../source/graphics/rendertargetscreen.h"
#include"../../source/graphics/depthstencil.h"

using namespace Maid;



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
      m_hFont.Create( SIZE2DI(8,16), true );
      m_Size = SIZE2DI(100,100);
      m_Text = MAIDTEXT("通常");
      m_Color= COLOR_R32G32B32A32F(1,0,0,1);
    }
    virtual void OnFinalize(){}

    virtual bool LocalIsMouseIn( const POINT2DI& pos ) const
    {
      return Collision<int>::IsPointToRect( pos, RECT2DI(POINT2DI(0,0),m_Size) );
    }

    virtual void OnUpdateFrame(){}
    virtual void OnUpdateDraw( RenderTargetBase& Target, IDepthStencil& Depth, const VECTOR2DI& offset  )
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
    m_Manager.Initialize( MakeReciveMessageFunction(&MyApp::ReciveMessage,this), &m, &k );
 
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

    m_hFont.Create( SIZE2DI(8,16), true );

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


    POINT2DI pos(0,0);
    for( std::list<String>::iterator ite=m_MessageLog.begin();
                                    ite!=m_MessageLog.end();
                                    ++ite )
    {
      m_Render.BltText( pos, m_hFont, (*ite), COLOR_R32G32B32A32F(1,1,1,1) );
      pos.y += m_hFont.GetSize().h;
    }
    m_Render.End();
  }

  void Finalize()
  {
    // OnExitGame() を呼んだあとの処理

  }

private:
  void ReciveMessage( IGUIParts::ID id, const IGUIParam& param )
  {
    String str;
    {
      char buf[256];
      sprintf( buf, "ID:%02d", id );
      str = String::ConvertSJIStoMAID(buf) + MAIDTEXT(" MESS:") + CreateMessageLog(param);
    }

    m_MessageLog.push_back(str);

    if( m_MessageLog.size()>30 ) { m_MessageLog.pop_front(); }
  }

private:
  GraphicsRender  m_Render;
  GUIManager      m_Manager;

  Font m_hFont;
  std::list<String> m_MessageLog;
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

