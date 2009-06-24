#include"buttonsample.h"

using namespace Maid;




void ButtonSample::Initialize( Maid::Graphics2DRender* r, const SIZE2DI& size )
{
  m_pRender = r;
  m_hFont.Create( SIZE2DI(8,16), true );
  m_Size = size;
}


void ButtonSample::OnInitialize( ID id, const IGUIParts& Parent )
{
}

void ButtonSample::OnFinalize()
{
}

bool ButtonSample::LocalIsCollision( const POINT2DI& pos ) const
{
  return Collision<int>::IsPointToRect( pos, RECT2DI(POINT2DI(0,0),m_Size) );
}

void ButtonSample::OnUpdateFrame()
{
}

void ButtonSample::OnUpdateDraw( const  RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos  )
{
  String str = MAIDTEXT("ボタン:");
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

  m_pRender->Fill( pos, COLOR_R32G32B32A32F(1,0,0,1), m_Size, POINT2DI(0,0) );
  m_pRender->BltText( pos, m_hFont, str, COLOR_R32G32B32A32F(1,1,1,1) );
}

void ButtonSample::OnMouseMove( const POINT2DI& pos )
{
}

void ButtonSample::OnMouseOut( const POINT2DI& pos )
{
}

void ButtonSample::OnMouseIn( const POINT2DI& pos )
{
}