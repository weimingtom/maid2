#include"radiobuttonsample.h"

using namespace Maid;



void RadioButtonSample::Initialize( Maid::GraphicsRender* r )
{
  m_pRender = r;
  m_hFont.Create( SIZE2DI(8,16), true );
  m_Size = SIZE2DI(100,100);
}

GUIButton::MESSAGERETURN RadioButtonSample::OnInitialize()
{
  m_hFont.Create( SIZE2DI(8,16), true );
  m_Size = SIZE2DI(100,100);
  return MESSAGERETURN_SENDPARENT;
}

GUIButton::MESSAGERETURN RadioButtonSample::OnFinalize()
{
  return MESSAGERETURN_SENDPARENT;
}

bool RadioButtonSample::LocalIsCollision( const POINT2DI& pos ) const
{
  return Collision<int>::IsPointToRect( pos, RECT2DI(POINT2DI(0,0),m_Size) );
}

void RadioButtonSample::OnUpdateFrame()
{
}

void RadioButtonSample::OnUpdateDraw( const  RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos  )
{
  String str = MAIDTEXT("ラジオ:");
  const bool in = IsMouseIn();
  const bool down = IsButtonDown();

  if( in )
  {
    if( down ) { str += MAIDTEXT("押し中"); }
    else { str += MAIDTEXT("入った"); }
  }else
  {
    if( down ) { str += MAIDTEXT("押し中外"); }
    else { str += MAIDTEXT("通常"); }
  }

  if( IsCheck() )
  {
    str += MAIDTEXT(":check");
  }else
  {
    str += MAIDTEXT(":free");
  }

  m_pRender->Fill( pos, COLOR_R32G32B32A32F(1,0,0,1), m_Size, POINT2DI(0,0) );
  m_pRender->BltText( pos, m_hFont, str, COLOR_R32G32B32A32F(1,1,1,1) );
}

GUIButton::MESSAGERETURN RadioButtonSample::OnMouseIn( const POINT2DI& pos )
{
  return MESSAGERETURN_SENDPARENT;
}

GUIButton::MESSAGERETURN RadioButtonSample::OnMouseOut( const POINT2DI& pos )
{
  return MESSAGERETURN_SENDPARENT;
}

GUIButton::MESSAGERETURN RadioButtonSample::OnMouseDown( const POINT2DI& pos, int Button )
{
  return MESSAGERETURN_SENDPARENT;
}

GUIButton::MESSAGERETURN RadioButtonSample::OnMouseUp( const POINT2DI& pos, int Button )
{
  return MESSAGERETURN_SENDPARENT;
}

GUIButton::MESSAGERETURN RadioButtonSample::OnCheck()
{
  return MESSAGERETURN_SENDPARENT;
}
