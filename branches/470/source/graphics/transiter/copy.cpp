#include"copy.h"
#include"common.h"


namespace Maid
{
  namespace Transiter
  {


void CopyDynamic( const Surface& Src, const RECT2DI& src_rc, Surface& Dst, const RECT2DI& dst_rc )
{
  for( int y=0; y<dst_rc.h; ++y )
  {
    for( int x=0; x<dst_rc.w; ++x )
    {
      const POINT2DI src_pos(src_rc.x+x, src_rc.y+y);
      const POINT2DI dst_pos(dst_rc.x+x, dst_rc.y+y);

      const COLOR_R32G32B32A32F color = Src.GetPixel( src_pos );
      Dst.SetPixel( dst_pos, color );
    }
  }
}

template<class SRCPIXEL, class DSTPIXEL>
void CopyInteger( const Surface& Src, const RECT2DI& SrcRect, const SRCPIXEL& SrcPixel, Surface& Dst, const RECT2DI& DstRect, const DSTPIXEL& DstPixel )
{
  const int SrcPitch = Src.GetPitch();
  const PIXELFORMAT SrcFormat = Src.GetPixelFormat();
  const void* pSrc = Src.GetLinePTR(SrcRect.y);

  const int DstPitch = Dst.GetPitch();
  const PIXELFORMAT DstFormat = Dst.GetPixelFormat();
  const void* pDst = Dst.GetLinePTR(DstRect.y);

  const int SrcPixelSize = GetPixelBPP(SrcFormat) / 8;
  const int DstPixelSize = GetPixelBPP(DstFormat) / 8;

  const int width  = std::min( SrcRect.w, DstRect.w );
  const int height = std::min( SrcRect.h, DstRect.h );

  for( int y=0; y<height; ++y )
  {
    const unt08* s = ((unt08*)pSrc) + SrcPitch*y;
    const unt08* d = ((unt08*)pDst) + DstPitch*y;
    for( int x=0; x<width; ++x )
    {
      const SRCPIXEL& src = *(SRCPIXEL*)s;
            DSTPIXEL& dst = *(DSTPIXEL*)d;

      dst.SetA( src.GetA() );
      dst.SetR( src.GetR() );
      dst.SetG( src.GetG() );
      dst.SetB( src.GetB() );

      s += SrcPixelSize;
      d += DstPixelSize;
    }
  }
}


//! タダ単にコピーする
/*!
    転送先は事前に作っておくこと

    @param  Src [i ]  転送元
    @param  Dst [ o]  転送先
 */
void Copy( const Surface& Src, Surface& Dst )
{
  RECT2DI src_rc(POINT2DI(0,0),Src.GetSize());
  RECT2DI dst_rc(POINT2DI(0,0),Dst.GetSize());

  Copy( Src, src_rc, Dst, dst_rc );
}

//! タダ単にコピーする
/*!
    転送先は事前に作っておくこと

    @param  Src     [i ]  転送元
    @param  SrcRect [i ]  転送領域
    @param  Dst     [ o]  転送先
    @param  DstRect [i ]  転送先領域
 */
void Copy( const Surface& Src, const RECT2DI& SrcRect, Surface& Dst, const RECT2DI& DstRect )
{
  const RECT2DI src_rc = Clip(Src,SrcRect);
  const RECT2DI dst_rc = Clip(Dst,DstRect);

  const PIXELFORMAT SrcFormat = Src.GetPixelFormat();
  const PIXELFORMAT DstFormat = Dst.GetPixelFormat();

  //  転送先と転送元のフォーマットから
  //  uint->uint テンプレートを駆使した静的フォーマット変換転送
  //  それ以外は動的フォーマット変換転送

#define PIXELCOPY( dstpixel ) \
switch( SrcFormat )  \
{                         \
case PIXELFORMAT_R08G08B08I:    { CopyInteger( Src, SrcRect, COLOR_R08G08B08I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_A08R08G08B08I: { CopyInteger( Src, SrcRect, COLOR_A08R08G08B08I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_A08B08G08R08I: { CopyInteger( Src, SrcRect, COLOR_A08B08G08R08I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_X08R08G08B08I: { CopyInteger( Src, SrcRect, COLOR_X08R08G08B08I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_A02R10G10B10I: { CopyInteger( Src, SrcRect, COLOR_A02R10G10B10I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_A02B10G10R10I: { CopyInteger( Src, SrcRect, COLOR_A02B10G10R10I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_R05G06B05I:    { CopyInteger( Src, SrcRect, COLOR_R05G06B05I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_X01R05G05B05I: { CopyInteger( Src, SrcRect, COLOR_X01R05G05B05I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_A01R05G05B05I: { CopyInteger( Src, SrcRect, COLOR_A01R05G05B05I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_A01B05G05R05I: { CopyInteger( Src, SrcRect, COLOR_A01B05G05R05I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_A04R04G04B04I: { CopyInteger( Src, SrcRect, COLOR_A04R04G04B04I(), Dst, DstRect, dstpixel() ); }break;  \
default: { CopyDynamic( Src, SrcRect, Dst, DstRect ); }break; \
}\

  switch( DstFormat )
  {
  case PIXELFORMAT_R08G08B08I:    { PIXELCOPY( COLOR_R08G08B08I ); }break;
  case PIXELFORMAT_B08G08R08I:    { PIXELCOPY( COLOR_B08G08R08I ); }break;
  case PIXELFORMAT_A08R08G08B08I: { PIXELCOPY( COLOR_A08R08G08B08I ); }break;
  case PIXELFORMAT_A08B08G08R08I: { PIXELCOPY( COLOR_A08B08G08R08I ); }break;
  case PIXELFORMAT_X08R08G08B08I: { PIXELCOPY( COLOR_X08R08G08B08I ); }break;
  case PIXELFORMAT_A02R10G10B10I: { PIXELCOPY( COLOR_A02R10G10B10I ); }break;
  case PIXELFORMAT_A02B10G10R10I: { PIXELCOPY( COLOR_A02B10G10R10I ); }break;
  case PIXELFORMAT_R05G06B05I:    { PIXELCOPY( COLOR_R05G06B05I    ); }break;
  case PIXELFORMAT_X01R05G05B05I: { PIXELCOPY( COLOR_X01R05G05B05I ); }break;
  case PIXELFORMAT_A01R05G05B05I: { PIXELCOPY( COLOR_A01R05G05B05I ); }break;
  case PIXELFORMAT_A01B05G05R05I: { PIXELCOPY( COLOR_A01B05G05R05I ); }break;
  case PIXELFORMAT_A04R04G04B04I: { PIXELCOPY( COLOR_A04R04G04B04I ); }break;
  default:
    {
      CopyDynamic( Src, SrcRect, Dst, DstRect );
    }break;
  }

#undef PIXELCOPY

/*
  ↑のコードは↓と同等のことをやってます

  for( int y=0; y<dst_rc.h; ++y )
  {
    for( int x=0; x<dst_rc.w; ++x )
    {
      const POINT2DI src_pos(src_rc.x+x, src_rc.y+y);
      const POINT2DI dst_pos(dst_rc.x+x, dst_rc.y+y);

      const COLOR_R32G32B32A32F color = Src.GetPixel( src_pos );
      Dst.SetPixel( dst_pos, color );
    }
  }
*/
}









void CopyAndSetAlphaDynamic( const Surface& Src, const Surface& Alpha, const RECT2DI& src_rc, Surface& Dst, const RECT2DI& dst_rc )
{
  for( int y=0; y<dst_rc.h; ++y )
  {
    for( int x=0; x<dst_rc.w; ++x )
    {
      const POINT2DI src_pos(src_rc.x+x, src_rc.y+y);
      const POINT2DI dst_pos(dst_rc.x+x, dst_rc.y+y);

            COLOR_R32G32B32A32F color = Src.GetPixel( src_pos );
      const float alpha = BITCONVERT08Ito32F(*(unt08*)Alpha.GetPixelPTR(src_pos));
      color.SetA( alpha );

      Dst.SetPixel( dst_pos, color );
    }
  }
}

template<class SRCPIXEL, class DSTPIXEL>
void CopyAndSetAlphaInteger( const Surface& Src, const Surface& Alpha, const RECT2DI& SrcRect, const SRCPIXEL& SrcPixel, Surface& Dst, const RECT2DI& DstRect, const DSTPIXEL& DstPixel )
{
  const int SrcPitch = Src.GetPitch();
  const PIXELFORMAT SrcFormat = Src.GetPixelFormat();
  const void* pSrc = Src.GetLinePTR(SrcRect.y);

  const int AlpPitch = Alpha.GetPitch();
  const void* pAlp = Alpha.GetLinePTR(SrcRect.y);

  const int DstPitch = Dst.GetPitch();
  const PIXELFORMAT DstFormat = Dst.GetPixelFormat();
  const void* pDst = Dst.GetLinePTR(DstRect.y);


  const int SrcPixelSize = GetPixelBPP(SrcFormat) / 8;
  const int DstPixelSize = GetPixelBPP(DstFormat) / 8;

  const int width  = std::min( SrcRect.w, DstRect.w );
  const int height = std::min( SrcRect.h, DstRect.h );

  for( int y=0; y<height; ++y )
  {
    const unt08* s = ((unt08*)pSrc) + SrcPitch*y;
    const unt08* a = ((unt08*)pAlp) + AlpPitch*y;
    const unt08* d = ((unt08*)pDst) + DstPitch*y;
    for( int x=0; x<width; ++x )
    {
      const SRCPIXEL& src = *(SRCPIXEL*)s;
      const unt08     alp = *a;
            DSTPIXEL& dst = *(DSTPIXEL*)d;

      dst.SetA( BITCONVERT08Ito16I(alp) );
      dst.SetR( src.GetR() );
      dst.SetG( src.GetG() );
      dst.SetB( src.GetB() );

      s += SrcPixelSize;
      d += DstPixelSize;
      a += 1;
    }
  }
}

void CopyAndSetAlpha( const Surface& Src, const Surface& Alpha, Surface& Dst )
{
  RECT2DI src_rc(POINT2DI(0,0),Src.GetSize());
  RECT2DI dst_rc(POINT2DI(0,0),Dst.GetSize());

  CopyAndSetAlpha( Src, Alpha, src_rc, Dst, dst_rc );
}

void CopyAndSetAlpha( const Surface& Src, const Surface& Alpha, const RECT2DI& SrcRect, Surface& Dst, const RECT2DI& DstRect )
{
  MAID_ASSERT( Alpha.GetPixelFormat()!=PIXELFORMAT_P08X08R08G08B08I && Alpha.GetPixelFormat()!=PIXELFORMAT_P08A08R08G08B08I,
    "アルファ画像はインデックス付でお願いします" );

  const RECT2DI src_rc = Clip(Src,SrcRect);
  const RECT2DI dst_rc = Clip(Dst,DstRect);

  const PIXELFORMAT SrcFormat = Src.GetPixelFormat();
  const PIXELFORMAT DstFormat = Dst.GetPixelFormat();

  //  転送先と転送元のフォーマットから
  //  uint->uint テンプレートを駆使した静的フォーマット変換転送
  //  それ以外は動的フォーマット変換転送

#define PIXELCOPY( dstpixel ) \
switch( SrcFormat )  \
{                         \
case PIXELFORMAT_R08G08B08I:    { CopyAndSetAlphaInteger( Src, Alpha, SrcRect, COLOR_R08G08B08I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_A08R08G08B08I: { CopyAndSetAlphaInteger( Src, Alpha, SrcRect, COLOR_A08R08G08B08I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_A08B08G08R08I: { CopyAndSetAlphaInteger( Src, Alpha, SrcRect, COLOR_A08B08G08R08I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_X08R08G08B08I: { CopyAndSetAlphaInteger( Src, Alpha, SrcRect, COLOR_X08R08G08B08I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_A02R10G10B10I: { CopyAndSetAlphaInteger( Src, Alpha, SrcRect, COLOR_A02R10G10B10I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_A02B10G10R10I: { CopyAndSetAlphaInteger( Src, Alpha, SrcRect, COLOR_A02B10G10R10I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_R05G06B05I:    { CopyAndSetAlphaInteger( Src, Alpha, SrcRect, COLOR_R05G06B05I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_X01R05G05B05I: { CopyAndSetAlphaInteger( Src, Alpha, SrcRect, COLOR_X01R05G05B05I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_A01R05G05B05I: { CopyAndSetAlphaInteger( Src, Alpha, SrcRect, COLOR_A01R05G05B05I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_A01B05G05R05I: { CopyAndSetAlphaInteger( Src, Alpha, SrcRect, COLOR_A01B05G05R05I(), Dst, DstRect, dstpixel() ); }break;  \
case PIXELFORMAT_A04R04G04B04I: { CopyAndSetAlphaInteger( Src, Alpha, SrcRect, COLOR_A04R04G04B04I(), Dst, DstRect, dstpixel() ); }break;  \
default: { CopyAndSetAlphaDynamic( Src, Alpha, SrcRect, Dst, DstRect ); }break; \
}\

  switch( DstFormat )
  {
  case PIXELFORMAT_R08G08B08I:    { PIXELCOPY( COLOR_R08G08B08I ); }break;
  case PIXELFORMAT_B08G08R08I:    { PIXELCOPY( COLOR_B08G08R08I ); }break;
  case PIXELFORMAT_A08R08G08B08I: { PIXELCOPY( COLOR_A08R08G08B08I ); }break;
  case PIXELFORMAT_A08B08G08R08I: { PIXELCOPY( COLOR_A08B08G08R08I ); }break;
  case PIXELFORMAT_X08R08G08B08I: { PIXELCOPY( COLOR_X08R08G08B08I ); }break;
  case PIXELFORMAT_A02R10G10B10I: { PIXELCOPY( COLOR_A02R10G10B10I ); }break;
  case PIXELFORMAT_A02B10G10R10I: { PIXELCOPY( COLOR_A02B10G10R10I ); }break;
  case PIXELFORMAT_R05G06B05I:    { PIXELCOPY( COLOR_R05G06B05I    ); }break;
  case PIXELFORMAT_X01R05G05B05I: { PIXELCOPY( COLOR_X01R05G05B05I ); }break;
  case PIXELFORMAT_A01R05G05B05I: { PIXELCOPY( COLOR_A01R05G05B05I ); }break;
  case PIXELFORMAT_A01B05G05R05I: { PIXELCOPY( COLOR_A01B05G05R05I ); }break;
  case PIXELFORMAT_A04R04G04B04I: { PIXELCOPY( COLOR_A04R04G04B04I ); }break;
  default:
    {
      CopyAndSetAlphaDynamic( Src, Alpha, SrcRect, Dst, DstRect );
    }break;
  }

#undef PIXELCOPY

}



  }
}

