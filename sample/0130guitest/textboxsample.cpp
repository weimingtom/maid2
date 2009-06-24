#include"textboxsample.h"

using namespace Maid;

TextBoxSample::TextBoxSample()
{

}

void TextBoxSample::Initialize( Maid::Graphics2DRender* r )
{
  m_pRender = r;
  m_hFont.Create( SIZE2DI(8,16), true );
}



void TextBoxSample::OnInitialize( ID id, const IGUIParts& Parent )
{

}

void TextBoxSample::OnFinalize()
{

}
const int BARSIZE_W = 200;
const int BARSIZE_H = 10;

bool TextBoxSample::LocalIsCollision( const POINT2DI& pos ) const
{
  const RECT2DI rc( POINT2DI(0,0), SIZE2DI(BARSIZE_W,BARSIZE_H) );

  return Collision<float>::IsPointToRect( pos, rc );
}

void TextBoxSample::OnUpdateFrame()
{

}

void TextBoxSample::OnUpdateDraw( const Maid::RenderTargetBase& Target, const Maid::IDepthStencil& Depth, const Maid::POINT2DI& pos  )
{
  String str = MAIDTEXT("text:");
  const bool in = IsMouseIn();
  const String Text = GetText();
  const int TextCursor = GetTextCursor();
  const String IMText = GetIMText();
  const int IMCursor = GetIMCursor();

  {
    char buf[256];
    sprintf( buf, "c:%02d: ", TextCursor );

    str += String::ConvertSJIStoMAID(buf) + Text;
  }

  if( IsIMOpen() )
  {
    char buf[256];
    sprintf( buf, "c:%02d: ", IMCursor );

    str += String::ConvertSJIStoMAID(buf) + IMText;
  }

  {
    const SIZE2DI size(BARSIZE_W,BARSIZE_H);
    const POINT2DI off(0,0);

    m_pRender->Fill( pos, COLOR_R32G32B32A32F(0,1,0,1), size, off );
  }

  m_pRender->BltText( pos, m_hFont, str, COLOR_R32G32B32A32F(1,1,1,1) );

}

