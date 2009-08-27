

#include"app.h"

#include"startupdialog.h"

using namespace Maid;


  enum SAMPLEID
  {
    SAMPLEID_BUTTON=0,

    SAMPLEID_CHECKBOX,

    SAMPLEID_RADIO0,
    SAMPLEID_RADIO1,
    SAMPLEID_RADIO2,

    SAMPLEID_SLIDER,
    SAMPLEID_SLIDER20,
    SAMPLEID_SLIDER20b,

    SAMPLEID_WINDOW,
    SAMPLEID_WINDOWBUTTON,

    SAMPLEID_TEXTBOX,

    SAMPLEID_CONTEXTMENU,

    SAMPLEID_COMBOBOX,
  };


MyApp::MyApp()
  :m_Render(m_Command)
{

}

bool MyApp::SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
{
  const RETURNCODE ret = BeginStartupInfo( DeviceList, conf );
  return ret==GAMESTART;
}

void MyApp::Initialize()
{
  const Mouse& m = GetMouse();
  const Keybord& k = GetKeybord();

  m_Frame = 0;

  m_Command.Initialize();
  m_Render.Initialize();

  //  GUIになにかイベントがあったときにコールバックされる関数の設定
  m_Manager.SetReviceFunction( MakeReciveMessageFunction(&MyApp::ReciveMessage,this) );


  //  これ、サンプルなので GraphicsRender を渡したりしてるけど
  //  どうせならGUIクラスを GlobalPointer<GraphicsRender> で継承して使うのがいいと思う
  //  GlobalPointer<MyApp> とやるのなら音とかも使えるようになるし、相当ベンリかも

  Graphics2DRender* pRender = &m_Render;


  //  各種ＧＵＩパーツの初期化＆m_Managerへの登録

  {
    m_Button.Initialize( pRender, SIZE2DI(100,100) );
    m_Button.SetPosition( POINT2DI(250,100) );

    m_Manager.RegisterParts( SAMPLEID_BUTTON, m_Button );
  }
  {
    m_CheckBox.Initialize( pRender );
    m_CheckBox.SetPosition( POINT2DI(250,250) );

    m_Manager.RegisterParts( SAMPLEID_CHECKBOX, m_CheckBox );
  }
  {
    const SAMPLEID id[] = {SAMPLEID_RADIO0,SAMPLEID_RADIO1,SAMPLEID_RADIO2 };

    for( int i=0; i<NUMELEMENTS(m_RadioButton); ++i )
    {
      RadioButtonSample& b = m_RadioButton[i];

      b.Initialize( pRender );
      b.SetPosition( POINT2DI(450,10+60*i) );
      b.SetGroup( m_RadioGroup );

      m_Manager.RegisterParts( id[i], b );
    }
  }



  {
    m_Slider.Initialize( pRender );
    m_Slider.SetPosition( POINT2DI(200,380) );
//    m_Slider.SetValueBegin(-100);
//    m_Slider.SetValueEnd(100);
    m_Slider.SetValueBegin(10);
    m_Slider.SetValueEnd(-10);
    m_Slider.SetBarLength(200);

    m_Manager.RegisterParts( SAMPLEID_SLIDER, m_Slider );
  }


  {
    m_Slider20.Initialize( pRender );
    m_Slider20.SetPosition( POINT2DI(200,430) );
    m_Slider20.SetValueBegin(-50);
    m_Slider20.SetValueEnd(50);
    m_Slider20.SetRotate( DEGtoRAD(20.0f) );  //  ２０度傾く
    m_Slider20.SetBarLength(200);

    m_Manager.RegisterParts( SAMPLEID_SLIDER20, m_Slider20 );
  }
  {
    m_Slider20b.Initialize( pRender );
    m_Slider20b.SetPosition( POINT2DI(200,530) );
    m_Slider20b.SetValueBegin(-50);
    m_Slider20b.SetValueEnd(50);
    m_Slider20b.SetBarLength(200);
    m_Slider20b.SetButtonLength(20);

    m_Manager.RegisterParts( SAMPLEID_SLIDER20b, m_Slider20b );
  } 

  {
    m_WindowButton.Initialize( pRender, SIZE2DI(100,20) );
    m_WindowButton.SetPosition( POINT2DI(0,50) );

    m_Window.Initialize( pRender );
    m_Window.SetPosition( POINT2DI(0,0) );

    m_Window.AddChildParts( SAMPLEID_WINDOWBUTTON, m_WindowButton );

    m_Manager.RegisterParts( SAMPLEID_WINDOW, m_Window );
    m_Window.SetZOrder(IGUIParts::ZORDER_BACK);
  }


  {
    m_TextBox.Initialize( pRender );
    m_TextBox.SetPosition( POINT2DI(0,570) );
    m_TextBox.SetTextLengthByte( 10 );
    m_Manager.RegisterParts( SAMPLEID_TEXTBOX, m_TextBox );

  }

  {
    const SIZE2DI size(50,25);
    for( int i=0; i<NUMELEMENTS(m_ContextSubMenu1); ++i )
    {
      m_ContextSubMenu1[i].Initialize( pRender, String::PrintFormat("sub1-%d",i), size );
    }
    for( int i=0; i<NUMELEMENTS(m_ContextSubMenu2); ++i )
    {
      m_ContextSubMenu2[i].Initialize( pRender, String::PrintFormat("sub2-%d",i), size );
    }
    for( int i=0; i<NUMELEMENTS(m_ContextSubMenu3); ++i )
    {
      m_ContextSubMenu3[i].Initialize( pRender, String::PrintFormat("sub3-%d",i), size );
    }

    m_ContextSubMenu1[0].Insert( 100, m_ContextSubMenu2[0] );
    m_ContextSubMenu1[0].Insert( 101, m_ContextSubMenu2[1] );
    m_ContextSubMenu1[0].Insert( 102, m_ContextSubMenu2[2] );

    m_ContextSubMenu2[1].Insert( 200, m_ContextSubMenu3[0] );
    m_ContextSubMenu2[1].Insert( 201, m_ContextSubMenu3[1] );
    m_ContextSubMenu2[1].Insert( 202, m_ContextSubMenu3[2] );

    m_ContextMenu.Insert( 0, m_ContextSubMenu1[0] );
    m_ContextMenu.Insert( 1, m_ContextSubMenu1[1] );
    m_ContextMenu.Insert( 2, m_ContextSubMenu1[2] );

    //  割り込み用なので、設定はしない
    //  m_Manager.RegisterParts( SAMPLEID_CONTEXTMENU, m_ContextMenu );
  }

  {
    m_ComboBox.Initialize( pRender );

    for( int i=0; i<NUMELEMENTS(m_ComboBoxElement); ++i )
    {
      m_ComboBoxElement[i].Initialize( pRender, String::PrintFormat("項目%02d",i) );
      m_ComboBox.Insert( i, m_ComboBoxElement[i] );

    }

    m_ComboBox.SetPosition( POINT2DI(500,350) );
    m_ComboBox.SetSelectListMax( 3 );
    m_Manager.RegisterParts( SAMPLEID_COMBOBOX, m_ComboBox );
  }


  m_hFont.Create( SIZE2DI(8,16), true );
}

void MyApp::UpdateFrame()
{
  const Mouse& m = GetMouse();
  const Keybord& k = GetKeybord();
  const TextInput& t = GetTextInput();
  GraphicsCore& core = GetGraphicsCore();

  if( m_Render.IsInitializing() ) { return ; }

  ++m_Frame;
  m_Manager.UpdateFrame( &m, &k, &t );
}

void MyApp::UpdateDraw()
{
  //  定期的に描画するとこ

  m_Command.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(1,0,0,0) );

  if( m_Render.IsInitializing() ) { return ; }

  const RenderTargetBase& rt = GetGraphicsCore().GetBackBuffer();
  const IDepthStencil& ds = GetGraphicsCore().GetDepthStencil();

  m_Command.Begin();

  m_Manager.UpdateDraw(rt,ds );

  {
    m_Render.BltText( POINT2DI(0,0), m_hFont, MAIDTEXT(" time :id:mess"), COLOR_R32G32B32A32F(1,1,1,1) );

    POINT2DI pos(0,m_hFont.GetSize().h);
    for( std::list<String>::iterator ite=m_MessageLog.begin();
                                    ite!=m_MessageLog.end();
                                    ++ite )
    {
      m_Render.BltText( pos, m_hFont, (*ite), COLOR_R32G32B32A32F(1,1,1,1) );
      pos.y += m_hFont.GetSize().h;
    }
  }
  m_Command.End();
}

void MyApp::Finalize()
{
  // OnExitGame() を呼んだあとの処理

}

void MyApp::ReciveMessage( IGUIParts::ID id, const IGUIParam& param )
{
  { //  GUI内で起きたメッセージはここで処理する
    switch( param.Message )
    {
    case IGUIParam::MESSAGE_BUTTON_CLICK:
      {
        switch( id )
        {
        case SAMPLEID_WINDOWBUTTON:
          {
              const POINT2DI pos( GetMouse().GetX(), GetMouse().GetY() );
              m_Manager.BeginInterruptParts( pos, SAMPLEID_CONTEXTMENU, m_ContextMenu );
          }break;

        }

      }break;
    }
  }

  {
    //  ログを足す
    String str;
    {
      char buf[256];
      sprintf( buf, "%06d %02d", m_Frame, id );
      str = String::ConvertSJIStoMAID(buf) + MAIDTEXT(" ") + CreateMessageLog(param);
    }

    m_MessageLog.push_back(str);

    if( m_MessageLog.size()>30 ) { m_MessageLog.pop_front(); }
  }
}



