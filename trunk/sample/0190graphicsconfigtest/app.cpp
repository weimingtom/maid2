#include"app.h"
#include"startupdialog.h"

using namespace Maid;

  enum SAMPLEID
  {
    ID_RELOADBUTTON,
    ID_TEXTUREQUALITY,
    ID_MIPMAP,

    ID_SCREEN640x480,
    ID_SCREEN800x600,
    ID_SCREEN1024x768,

    ID_FILTERPOINT,
    ID_FILTERBILINEAR,

    ID_CONFIGWINDOW,
  };

 MyApp::MyApp()
    :m_Render(m_Command)
  {

  }

bool MyApp::SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf )
{
  const RETURNCODE ret = BeginStartupInfo( DeviceList, conf );
  if( ret==APPEXIT ) { return false; }
  return true;
}

void MyApp::Initialize()
{
  const Mouse& m = GetMouse();
  const Keybord& k = GetKeybord();
  Graphics2DRender* pRender = &m_Render;

  m_Manager.SetReviceFunction( MakeReciveMessageFunction(&MyApp::ReciveMessage,this) );

  m_Command.Initialize();
  m_Render.Initialize();

  m_FontM.Create( SIZE2DI(8,16), true );

  m_FrameCount = 0;
  m_DrawCount = 0;
  m_FPS = 0.0f;

  m_Window.Initialize( pRender );
  m_Window.SetPosition( POINT2DI(0,0) );

  {
    m_ReloadButton.Initialize( pRender, SIZE2DI(80,18), MAIDTEXT("リロード") );
    m_ReloadButton.SetPosition( POINT2DI(5,35) );

    m_Window.AddChildParts( ID_RELOADBUTTON, m_ReloadButton );
  }


  {
    m_Screen640x480.Initialize( pRender, SIZE2DI(55,18), MAIDTEXT("VGA") );
    m_Screen640x480.SetPosition( POINT2DI(5,60) );

    m_Window.AddChildParts( ID_SCREEN640x480, m_Screen640x480 );

    m_Screen800x600.Initialize( pRender, SIZE2DI(55,18), MAIDTEXT("SVGA") );
    m_Screen800x600.SetPosition( POINT2DI(65,60) );

    m_Window.AddChildParts( ID_SCREEN800x600, m_Screen800x600 );

    m_Screen1024x768.Initialize( pRender, SIZE2DI(55,18), MAIDTEXT("XGA") );
    m_Screen1024x768.SetPosition( POINT2DI(125,60) );

    m_Window.AddChildParts( ID_SCREEN1024x768, m_Screen1024x768 );
  }
  {
    m_TextureQuality.Initialize( pRender, MAIDTEXT("quality:"), SIZE2DI(160,10), SIZE2DI(10,18) );
    m_TextureQuality.SetPosition( POINT2DI(15,90) );
    m_TextureQuality.SetValueBegin(0);
    m_TextureQuality.SetValueEnd(15);
    m_TextureQuality.SetBarLength(160);

    m_Window.AddChildParts( ID_TEXTUREQUALITY, m_TextureQuality );
  }

  {
    m_MipMap.Initialize( pRender, MAIDTEXT("mipmap"), SIZE2DI(75,18) );
    m_MipMap.SetPosition( POINT2DI(5,110) );
    m_MipMap.SetCheck(true);

    m_Window.AddChildParts( ID_MIPMAP, m_MipMap );
  }

  {
    m_FilterPoint.Initialize( pRender, MAIDTEXT("point"), SIZE2DI(85,18) );
    m_FilterPoint.SetPosition( POINT2DI(5,135) );
    m_FilterPoint.SetGroup( m_FilterGroup );

    m_Window.AddChildParts( ID_FILTERPOINT, m_FilterPoint );


    m_FilterBilinear.Initialize( pRender, MAIDTEXT("bilinear"), SIZE2DI(85,18) );
    m_FilterBilinear.SetPosition( POINT2DI(95,135) );
    m_FilterBilinear.SetGroup( m_FilterGroup );

    m_FilterBilinear.SetCheck();

    m_Window.AddChildParts( ID_FILTERBILINEAR, m_FilterBilinear );
  }

  m_Manager.RegisterParts( ID_CONFIGWINDOW, m_Window );

  m_IsShowPanel = false;

  m_TextureFilter = Graphics2DRender::TEXTUREFILTER_LINEAR;
  ReloadImage();
}

void MyApp::UpdateFrame()
{
  const Mouse& m = GetMouse();
  const Keybord& k = GetKeybord();
  const TextInput& t = GetTextInput();
  GraphicsCore& core = GetGraphicsCore();

  ++m_FrameCount;

  if( m_Render.IsInitializing() ) { return ; }

  if( m_IsShowPanel )
  {
    m_Manager.UpdateFrame( &m, &k, &t );
  }

  if( k.IsIn(Keybord::BUTTON_SPACE) ) { m_IsShowPanel = !m_IsShowPanel; }

  ++m_PixivTanAngle;
  m_PixivTanAngle %= 360;
}

void MyApp::UpdateDraw()
{
  ++m_DrawCount;
  if( 60<=m_FrameCount )
  {
    m_FPS = float(m_DrawCount) / float(m_FrameCount) * 60.0f;
    m_FrameCount = 0;
    m_DrawCount = 0;

  }

  if( m_Render.IsInitializing() ) { return ; }

  const RenderTargetBase& rt = GetGraphicsCore().GetBackBuffer();
  const IDepthStencil& ds = GetGraphicsCore().GetDepthStencil();

  m_Command.SetRenderTarget( rt );
  m_Command.ClearRenderTarget( Maid::COLOR_A32B32G32R32F(0,0,0,0) );

  m_Command.Begin();



  if( !IsImageLoading() )
  {

    m_Render.SetFilterState( m_TextureFilter );

    {
      m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_NONE );
      const RECT2DI  rc( POINT2DI(0,0), m_BG.GetImageSize() );
      const POINT2DI center(rc.w/2,rc.h/2);
		  m_Render.Blt( POINT2DI(400,300), m_BG, rc, center, 1.0f );
    }

    {
      const RECT2DI  rc( POINT2DI(0,0), m_PixivTan.GetImageSize() );
      const POINT2DI center(rc.w/2,760);

      m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_SCREEN );
      m_Render.BltSR( POINT2DI(600,600), m_PixivTan, rc, center, 1.0f, SIZE2DF(0.8f,0.8f), DEGtoRAD(-m_PixivTanAngle), Maid::VECTOR3DF(0,1,0) );
    }
    {
      m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_ALPHA );
      const RECT2DI  rc( POINT2DI(0,0), m_ITChan.GetImageSize() );
      const POINT2DI center(rc.w/2,413);
	    m_Render.Blt( POINT2DI(200,600), m_ITChan, rc, center, 1.0f );
	    m_Render.BltS( POINT2DI(50,600), m_ITChan, rc, center, 1.0f, SIZE2DF(0.3f,0.3f) );
    }

  }else
  {
    m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_ALPHA );
    m_Render.BltText( POINT2DI(0,  16), m_FontM, MAIDTEXT("Loading"), COLOR_R32G32B32A32F(1,1,1,1) );
  }

  m_Render.SetFilterState( Graphics2DRender::TEXTUREFILTER_LINEAR );

  if( m_IsShowPanel )
  {
    m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_ALPHA );
    m_Manager.UpdateDraw(rt,ds);
  }

  {
    //  FPS表示 
    m_Render.SetBlendState( Graphics2DRender::BLENDSTATE_ALPHA );
    m_Render.BltText( POINT2DI(0,  0), m_FontM, String::PrintFormat( "fps:%f",m_FPS), COLOR_R32G32B32A32F(0,0,1,1) );
  }

  m_Command.End();

}

void MyApp::Finalize()
{

}



void MyApp::ReciveMessage( IGUIParts::ID id, const IGUIParam& param )
{
  switch( param.Message )
  {
  case IGUIParam::MESSAGE_BUTTON_CLICK:
    {
      switch( id )
      {
      case ID_RELOADBUTTON:
        {
          ReloadImage();
        }break;

      case ID_SCREEN640x480:
        {
          SetScreenSize( SIZE2DI(640,480) );
        }break;

      case ID_SCREEN800x600:
        {
          SetScreenSize( SIZE2DI(800,600) );
        }break;

      case ID_SCREEN1024x768:
        {
          SetScreenSize( SIZE2DI(1024,768) );
        }break;

      }
    }break;

  case IGUIParam::MESSAGE_SLIDER_CHANGEVALUE:
    {
      switch( id )
      {
      case ID_TEXTUREQUALITY:
        {
          const GUIMESSAGE_SLIDER_CHANGEVALUE& p = static_cast<const GUIMESSAGE_SLIDER_CHANGEVALUE&>(param);

          GetGraphicsCore().SetTextureQuality( p.Value );
        }break;
      }
    }break;

  case IGUIParam::MESSAGE_CHECKBOX_SETCHECK:
    {
      switch( id )
      {
      case ID_MIPMAP:
        {
          const GUIMESSAGE_CHECKBOX_SETCHECK& p = static_cast<const GUIMESSAGE_CHECKBOX_SETCHECK&>(param);

          GetGraphicsCore().SetTextureMipMap( p.NewState );
        }break;
      }
    }break;

  case IGUIParam::MESSAGE_RADIOBUTTON_SETCHECK:
    {
      switch( id )
      {
      case ID_FILTERPOINT:
        {
          m_TextureFilter = Graphics2DRender::TEXTUREFILTER_POINT;
        }break;

      case ID_FILTERBILINEAR:
        {
          m_TextureFilter = Graphics2DRender::TEXTUREFILTER_LINEAR;
        }break;
      }
    }break;
  }
}


void MyApp::ReloadImage()
{
  m_BG.LoadFile( MAIDTEXT("nc4758.png") );

  m_PixivTan.LoadFile( MAIDTEXT("nc1429.png") );
  m_ITChan.LoadFile( MAIDTEXT("nc3258.png") );

  m_PixivTanAngle = 0;

  m_State = STATE_LOADING;
}

bool MyApp::IsImageLoading()
{
  if( m_State==STATE_EXECUTING ) { return false; }

  return  m_BG.IsLoading()
    ||    m_ITChan.IsLoading()
    ||    m_PixivTan.IsLoading()
    ;
}
