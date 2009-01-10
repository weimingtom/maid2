#include"graphicsrender.h"
#include"customvertextemplate.h"
#include"../auxiliary/mathematics/vector.h"
#include"../auxiliary/mathematics/matrix.h"

#include<algorithm>

namespace Maid
{

void GraphicsRender::BltText( const POINT2DI& Base, const Font& pFont, const String& Text, const COLOR_R32G32B32A32F& Color )
{
  BltText( Base, pFont, Text, Color, ~0);
}

void GraphicsRender::BltText( const POINT2DI& Base, const Font& pFont, const String& Text, const COLOR_R32G32B32A32F& Color, size_t Len )
{
  POINT2DI pos = Base;

  const size_t loop = std::min(Len,Text.length());

  for( size_t i=0; i<loop; ++i )
  {
    const unt32 ch = Text[i];

    if( ch=='\n' )
    {
      pos.x  = Base.x;
      pos.y += pFont.GetSize().h;
      continue;
    }
    ef( ch=='\t' ) { }  //  タブは除外
    ef( ch=='\r' ) { }

    const FontTexture& tex = m_FontManager.GetTexture( pFont, ch );

    //  uvの計算
    const SIZE2DI& image = tex.GetFontSize();
    const SIZE2DI& create= tex.GetCreateSize();
    const SIZE2DI& texsize=tex.GetTextureSize();
    const RECT2DI rc( POINT2DI(0,0),image );

    const int x = rc.x * image.w / create.w;
    const int y = rc.y * image.h / create.h;
    const int w = rc.w * image.w / create.w;
    const int h = rc.h * image.h / create.h;

    const float ux = float(x) / float(texsize.w);
    const float vy = float(y) / float(texsize.h);
    const float uw = float(w) / float(texsize.w);
    const float vh = float(h) / float(texsize.h);

    MODELINFO m;
    CreateVirtualScreenModel( pos, rc.GetSize(), POINT2DI(0,0), m );


    CUSTOMVERTEX_SPRITE v[] =
    {
      CUSTOMVERTEX_SPRITE( POINT3DF(0,0,0), Color, POINT2DF(ux   ,vy   ) ),
      CUSTOMVERTEX_SPRITE( POINT3DF(0,0,0), Color, POINT2DF(ux+uw,vy   ) ),
      CUSTOMVERTEX_SPRITE( POINT3DF(0,0,0), Color, POINT2DF(ux   ,vy+vh) ),
      CUSTOMVERTEX_SPRITE( POINT3DF(0,0,0), Color, POINT2DF(ux+uw,vy+vh) ),
    };

    SetupVertex( m.Model, m.Translate, v, v[0].GetStructSize() );
    SetCommonVertex( v, sizeof(v), v[0].GetStructSize() );

    const IInputLayout&  layout = m_BltLayout;
    const IVertexShader& vs = m_BltVertexShader;
    const IPixelShader&  ps = m_FontPixelShader;
    const IMaterial&  material = tex;

    const Graphics::SPDRAWCOMMAND& pCommand = m_pDrawCommand;


    pCommand->SetPrimitiveTopology( Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

    pCommand->SetInputLayout( layout.Get() );
    pCommand->PSSetMaterial( 0, material.Get() );

    pCommand->VSSetShader( vs.Get() );
    pCommand->PSSetShader( ps.Get() );
    pCommand->Draw( 4, 0 );

    if( String::IsHankaku(ch) ) { pos.x += pFont.GetSize().w; }
    else { pos.x += pFont.GetSize().w*2; }
  }

}



}