#include"graphics2drender.h"
#include"customvertextemplate.h"
#include"../auxiliary/mathematics/vector.h"
#include"../auxiliary/mathematics/matrix.h"

#include<algorithm>

namespace Maid
{


void Graphics2DRender::BltText( const POINT2DI& Base, const Font& f, const String& Text )
{
  BltText( Base, f, Text, COLOR_R32G32B32A32F(1,1,1,1) );
}


//! テキスト描画
/*!
    @param	Base    [i ]	描画開始座標
    @param	pFont   [i ]	描画に使うフォント
    @param	Text    [i ]	描画する文字列
    @param	Color   [i ]	描画する色の強さ
 */
void Graphics2DRender::BltText( const POINT2DI& Base, const Font& pFont, const String& Text, const COLOR_R32G32B32A32F& pow )
{
  BltText( Base, pFont, Text, pow, ~0);
}




//! テキスト描画
/*!
    @param	Base    [i ]	描画開始座標
    @param	pFont   [i ]	描画に使うフォント
    @param	Text    [i ]	描画する文字列
    @param	Color   [i ]	描画する色強さ
    @param	Len     [i ]	描画する文字数（Text.length() より大きいと全部)
 */
void Graphics2DRender::BltText( const POINT2DI& Base, const Font& pFont, const String& Text, const COLOR_R32G32B32A32F& pow, size_t Len )
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
    else if( ch=='\t' ) { }  //  タブは除外(座標を増やすだけなので、そうしてもいいかも)
    else if( ch=='\r' ) { }

//    const FontTexture& tex = m_FontManager.GetTexture( pFont, ch );
    const Texture2DBase& tex = GetFont( pFont, ch );


    { //  指定した強さを掛けるシェーダを使う
      const RECT2DI  rc(POINT2DI(0,0),tex.GetRealSize());
      const POINT2DI center(0,0);
      const COLOR_R32G32B32A32F col(pow.GetR(),
                                    pow.GetG(),
                                    pow.GetB(),
                                    pow.GetA());

      MODELINFO m;
      CreateVirtualScreenModel( pos, rc.GetSize(), center, m );
      Blt( m.Model, m.Translate, tex, rc, col );
    }

    if( String::IsHankaku(ch) ) { pos.x += pFont.GetSize().w; }
    else { pos.x += pFont.GetSize().w*2; }
  }

}

const Texture2DBase&	Graphics2DRender::GetFont( const Font& pFont, unt32 Code )
{
	GAIZIMAP::iterator ite = m_GaiziMap.find(Code);

	if( ite!=m_GaiziMap.end() ) { return ite->second; }

	return m_FontManager.GetTexture( pFont, Code );

}



}