#include"textradiobutton.h"

using namespace Maid;



void TextRadioButton::Initialize( Maid::Graphics2DRender* r, const Maid::String& Text, const Maid::SIZE2DI& size )
{
  m_pRender = r;
  m_hFont.Create( SIZE2DI(8,16), true );
  m_Size = size;
  m_Text = Text;

}

void TextRadioButton::OnInitialize( ID id, const IGUIParts& Parent )
{
}

void TextRadioButton::OnFinalize()
{
}

bool TextRadioButton::LocalIsCollision( const POINT2DI& pos ) const
{
  return Collision<int>::IsPointToRect( pos, RECT2DI(POINT2DI(0,0),m_Size) );
}

void TextRadioButton::OnUpdateFrame()
{
}

void TextRadioButton::OnUpdateDraw( const  RenderTargetBase& Target, const IDepthStencil& Depth, const POINT2DI& pos  )
{
  String str;
  const bool in = IsMouseIn();
  const bool down = IsButtonDown();

  COLOR_R32G32B32A32F color(0,0,0,1);

  if( in )
  {
    color = COLOR_R32G32B32A32F(1,1,1,1);
  }

  if( IsCheck() )
  {
    str += MAIDTEXT("◎"); 
  }
  else            { str += MAIDTEXT("　"); }
  str += m_Text;

  m_pRender->Fill( pos, COLOR_R32G32B32A32F(0.5f,0.5f,0.5f,1), m_Size, POINT2DI(0,0) );
  m_pRender->BltText( pos, m_hFont, str, color );
}

void TextRadioButton::OnMouseMove( const POINT2DI& pos )
{
}

void TextRadioButton::OnMouseOut( const POINT2DI& pos )
{
}

void TextRadioButton::OnMouseIn( const POINT2DI& pos )
{
}

void TextRadioButton::OnCheck()
{
}
