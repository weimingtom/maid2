#include"copy.h"
#include"common.h"


namespace Maid
{
  namespace Transiter
  {

//! タダ単にコピーする
/*!
    転送先は事前に作っておくこと

    @param  Src [i ]  転送元
    @param  Dst [ o]  転送先
 */
void Copy( const Surface& Src, Surface& Dst )
{
  RECT2DI rc(POINT2DI(0,0),Src.GetSize());

  Copy( Src, rc, Dst, POINT2DI(0,0) );
}

//! タダ単にコピーする
/*!
    転送先は事前に作っておくこと

    @param  Src     [i ]  転送元
    @param  SrcRect [i ]  転送領域
    @param  Dst     [ o]  転送先
    @param  pos     [i ]  転送開始座標
 */
void Copy( const Surface& Src, const RECT2DI& rect, Surface& Dst, const POINT2DI& pos )
{
  const RECT2DI src_rc = Clip(Src,rect);
  const RECT2DI dst_rc = Clip( src_rc, Dst, pos );

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


  }
}

