#include"fill.h"
#include"common.h"


namespace Maid
{
  namespace Transiter
  {

//! タダ単にコピーする
/*!
    転送先は事前に作っておくこと

    @param  Color [i ]  塗りつぶす色
    @param  Dst   [ o]  転送先
 */
void Fill( const COLOR_R32G32B32A32F& Color, Surface& Dst )
{
  RECT2DI rc(POINT2DI(0,0),Dst.GetSize());
  Fill( Color, Dst, rc );
}

//! タダ単にコピーする
/*!
    転送先は事前に作っておくこと

    @param  Color [i ]  塗りつぶす色
    @param  Dst   [ o]  転送先
    @param  rect  [ o]  塗りつぶし範囲
 */
void Fill( const COLOR_R32G32B32A32F& Color, Surface& Dst, const RECT2DI& rect )
{
  const RECT2DI rc = Clip( Dst, rect );

  for( int y=0; y<rc.h; ++y )
  {
    for( int x=0; x<rc.w; ++x )
    {
      const POINT2DI pos( rc.x+x, rc.y+y );

      Dst.SetPixel( pos, Color );
    }
  }
}


  }
}

