#include"textbutton.h"

using namespace Maid;




void TextButton::Initialize( Maid::Graphics2DRender* r, const SIZE2DI& size, const String& Text )
{
  m_pRender = r;
  m_hFont.Create( SIZE2DI(8,16), true );
  m_Size = size;
  m_Text = Text;
}


void TextButton::OnInitialize( ID id, const IGUIParts& Parent )
{
}

void TextButton::OnFinalize()
{
}

bool TextButton::LocalIsCollision( const POINT2DI& pos ) const
{
  return Collision<int>::IsPointToRect( pos, RECT2DI(POINT2DI(0,0),m_Size) );
}

void TextButton::OnUpdateFrame()
{
}

void TextButton::OnUpdateDraw( const  RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos  )
{
  const bool in = IsMouseIn();
  const bool down = IsButtonDown();

  VECTOR2DI off(0,0);
  COLOR_R32G32B32A32F color(0,0,0,1);

  if( in )
  {
    color = COLOR_R32G32B32A32F(1,1,1,1);
  }
  
  if( down )
  {
    off = VECTOR2DI(1,1);
  }

  m_pRender->Fill( pos+off, COLOR_R32G32B32A32F(0.5f,0.5f,0.5f,1), m_Size, POINT2DI(0,0) );
  m_pRender->BltText( pos+off, m_hFont, m_Text, color );
}

void TextButton::OnMouseMove( const POINT2DI& pos )
{
}

void TextButton::OnMouseOut( const POINT2DI& pos )
{
}

void TextButton::OnMouseIn( const POINT2DI& pos )
{
}