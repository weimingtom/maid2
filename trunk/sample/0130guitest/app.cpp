#include"app.h"

#include"buttonsample.h"


#include"../../source/graphics/rendertargetscreen.h"

using namespace Maid;



bool MyApp::SelectDevice( const std::vector<Graphics::IDeviceList::INFO>& GraphicsDevice, DEFAULTCONFIG& conf )
{
   return true;
}

void MyApp::Initialize()
{
  const Mouse& m = GetMouse();
  const Keybord& k = GetKeybord();

  m_Render.Initialize();
  m_Manager.Initialize( MakeReciveMessageFunction(&MyApp::ReciveMessage,this), &m, &k );

  {
    boost::shared_ptr<ButtonSample> pButton( new ButtonSample(m_Render) );

    pButton->SetPosition( POINT2DI(100,100) );

    m_Manager.RegisterParts( 1, pButton );
  }

  {
    boost::shared_ptr<ButtonSample> pButton( new ButtonSample(m_Render) );

    pButton->SetPosition( POINT2DI(400,100) );

    m_Manager.RegisterParts( 2, pButton );
  }

  m_hFont.Create( SIZE2DI(8,16), true );

}

void MyApp::UpdateFrame()
{
  const Mouse& m = GetMouse();
  const Keybord& k = GetKeybord();
  GraphicsCore& core = GetGraphicsCore();

  if( m_Render.IsInitializing() ) { return ; }

  m_Manager.UpdateFrame();
}

void MyApp::UpdateDraw()
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

void MyApp::Finalize()
{
  // OnExitGame() を呼んだあとの処理

}

void MyApp::ReciveMessage( IGUIParts::ID id, const IGUIParam& param )
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



