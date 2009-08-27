#include"font.h"

#include"transiter.h"

#pragma COMPILERMSG("TODO:任意段階のアンチエイリアスがかけれるようにする")	

namespace Maid
{
Font::FILTERMAP  Font::s_FilterMap;
static const int s_SCALE = 4;

Font::Font()
  :m_FontSize(0,0)
  ,m_IsAntiAlias(false)
  ,m_Color(1,1,1,1)
  ,m_Filter(FILTER_NONE)
{

}

//! フォントが使える状態にあるか？
/*!
    @return 使えるなら true, 使えないなら false
 */
bool  Font::IsCreated() const
{
  return m_pFont.get()!=NULL;
}

//! フォントを作成する
/*!
    フォント名はデフォルトです。
    色は白。
    フィルタなしです。
    
    @param size         [i ]  フォントの大きさ
    @param IsAntiAlias  [i ]  アンチエイリアスをかけるか
 */
void Font::Create( const SIZE2DI& size, bool IsAntiAlias )
{
  Create( size, IsAntiAlias, COLOR_R32G32B32A32F(1,1,1,1) );
}

void Font::Create( const SIZE2DI& size, bool IsAntiAlias, const COLOR_R32G32B32A32F& Color )
{
  Create( size, IsAntiAlias, Color, FILTER_NONE );
}

void Font::Create( const SIZE2DI& size, bool IsAntiAlias, const COLOR_R32G32B32A32F& Color, unt FilterNo )
{
  const String name = GlobalPointer<GraphicsCore>::Get()->GetFontDevice()->GetDefaultFontName();
  Create( name, size, IsAntiAlias, Color, FilterNo );
}


//! フォントを作成する
/*!
    @param Name         [i ]  作成するフォント名
    @param size         [i ]  フォントの大きさ
    @param IsAntiAlias  [i ]  アンチエイリアスをかけるか
    @param Color        [i ]  ラスタライズしたときの色
    @param Filter       [i ]  おこなうフィルタ処理
 */
void Font::Create( const String& Name, const SIZE2DI& size, bool IsAntiAlias, const COLOR_R32G32B32A32F& Color, unt FilterNo )
{
  SIZE2DI createsize(size);

  //  アンチエイリアスをかける場合、４倍に広げてから、縮小でかけるので
  //  内部的にはs_SCALE倍の大きさで持っておく
  if( IsAntiAlias )
  {
    createsize.w *= s_SCALE;
    createsize.h *= s_SCALE;
  }

  m_pFont = GlobalPointer<GraphicsCore>::Get()->GetFontDevice()->CreateFont( Name, createsize );

  m_FontName = Name;
  m_FontSize = size;
  m_IsAntiAlias = IsAntiAlias;
  m_Color  = Color;
  m_Filter = FilterNo;
}

//! フォントの破棄
/*!
 */
void Font::Destroy()
{
  m_pFont.reset();
}


//! 作成しているフォント名の取得
/*!
 */
const String& Font::GetName()   const
{
  return m_FontName;
}

//! 作成しているフォントサイズの取得
/*!
 */
const SIZE2DI& Font::GetSize()   const
{
  return m_FontSize;
}

//! アンチエイリアスがかかっているか？
/*!
 */
bool Font::IsAntiAlias()const
{
  return m_IsAntiAlias;
}

//! テキスト色の取得
/*!
 */
const COLOR_R32G32B32A32F& Font::GetColor() const
{
  return m_Color;
}

//! かけるフィルタの取得
/*!
 */
unt Font::GetFilterNo() const
{
  return m_Filter;
}


//! ラスタライズする
/*!
    @param code  [i ]  ラスタライズする文字コード（MSTRING)
    @param Dst   [ o]  ラスタライズ先
 */
void Font::Rasterize( unt32 code, SurfaceInstance& Dst )const
{
  SPSURFACEINSTANCE pMem( new SurfaceInstance );

  GlobalPointer<GraphicsCore>::Get()->GetFontDevice()->Rasterize( m_pFont, code, m_Color, *pMem );

  {
    FILTERMAP::const_iterator ite = s_FilterMap.find(m_Filter);
    if( ite!=s_FilterMap.end() )
    {
      const FONTRASTERIZEFILTERLIST& Filter = ite->second;
      for( size_t i=0; i<Filter.size(); ++i )
      {
        Filter[i]->Convert( pMem );
      }
    }
  }

  if( m_IsAntiAlias )
  {
    const SIZE2DI size(pMem->GetSize().w/s_SCALE,pMem->GetSize().h/s_SCALE);

    Dst.Create( size, pMem->GetPixelFormat() );
    Transiter::Average( *pMem, Dst );
  }else
  {
    Dst.Create( pMem->GetSize(), pMem->GetPixelFormat() );
    Transiter::Copy( *pMem, Dst );
  }
}


//! 指定した文字列の矩形サイズを計算する
/*!
    @param text [i ]  計算したい文字列
 */
SIZE2DI Font::CalcTextSize( const String& text ) const
{
  int TextLineLength =0;	//	１行の最大文字数(1byteCode が +1 2byteCode は+2)
  int	CountCF=0;	// '\n' が何個あるか？
  int	NowStrCount=0;	//	現在行の文字数(1byteCode が +1 2byteCode は+2)

  for( int i=0; i<(int)text.length(); ++i )
  {
    const unt32 c = text[i];

    if     ( c=='\r' ) { continue; }
    else if( c=='\t' ) { continue; }
    else if( c=='\n' ) 
    {
      TextLineLength = std::max( TextLineLength, NowStrCount );
      CountCF++; 
      NowStrCount = 0;
    }
    else if( String::IsHankaku(c) ) { NowStrCount += 1; }
    else { NowStrCount += 2; }
  }

  TextLineLength = std::max( TextLineLength, NowStrCount );

  const int w = GetSize().w * TextLineLength;
  const int h = GetSize().h * (CountCF+1);

  return SIZE2DI(w,h);
}

//! ラスタライズフィルタの設定
/*!
    @param No     [i ]  設定番号
    @param Filter [i ]  フィルタ
 */
void Font::SetFilter( unt No, const FONTRASTERIZEFILTERLIST& Filter )
{
  s_FilterMap[No] = Filter;
}

}
