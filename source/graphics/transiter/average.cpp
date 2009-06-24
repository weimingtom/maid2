#include"average.h"
#include"common.h"

#include"../color.h"

//  平均で色を作る

namespace Maid
{
  namespace Transiter
  {

static COLOR_R32G32B32A32F GetLineTotal( const Surface& Src, float BeginX, float EndX, int y )
{
	int	BeginI;
	float BeginF;
	FloatDiv( BeginX, BeginI, BeginF );

	int	EndI;
	float EndF;
	FloatDiv( EndX, EndI, EndF );

  float channel[4]={0,0,0,0};	// 0:a  1:r  2:g  3:b

  if( BeginF<1.0f)
  { //  一番左の色
    const COLOR_R32G32B32A32F c = Src.GetPixel( POINT2DI(BeginI,y) );
    channel[0] = c.GetA() * (1.0f-BeginF);
    channel[1] = c.GetR() * (1.0f-BeginF);
    channel[2] = c.GetG() * (1.0f-BeginF);
    channel[3] = c.GetB() * (1.0f-BeginF);
  }
  { //  途中の色
    for( int32 i=BeginI+1; i<EndI; ++i )
    {
      const COLOR_R32G32B32A32F c = Src.GetPixel( POINT2DI(i,y) );
      channel[0] += c.GetA();
      channel[1] += c.GetR();
      channel[2] += c.GetG();
      channel[3] += c.GetB();
    }
  }
  if( 0<EndF )
  { //  一番右の色
    const COLOR_R32G32B32A32F c = Src.GetPixel( POINT2DI(EndI,y) );
    channel[0] += c.GetA() * EndF;
    channel[1] += c.GetR() * EndF;
    channel[2] += c.GetG() * EndF;
    channel[3] += c.GetB() * EndF;
  }

	return COLOR_R32G32B32A32F( channel[1], channel[2], channel[3], channel[0] );
}


//! 範囲内の平均で合成する
/*!
    縮小転送しかサポートしてません

    @param  Src [i ]  転送元
    @param  Dst [ o]  転送先
 */
void Average( const Surface& Src, Surface& Dst )
{
  RECT2DI src_rc(POINT2DI(0,0),Src.GetSize());
  RECT2DI dst_rc(POINT2DI(0,0),Dst.GetSize());

  Average( Src, src_rc, Dst, dst_rc );
}


//! 範囲内の平均で合成する
/*!
    縮小転送しかサポートしてません

    @param  Src     [i ]  転送元
    @param  SrcRect [i ]  転送領域
    @param  Dst     [ o]  転送先
    @param  DstRect [i ]  転送領域
 */
void Average( const Surface& Src, const RECT2DI& SrcRect, Surface& Dst, const RECT2DI& DstRect )
{
  const RECT2DI src_rc = Clip(Src,SrcRect);
  const RECT2DI dst_rc = Clip(Dst,DstRect);

  const float scale_w = float(src_rc.w) / float(dst_rc.w);
  const float scale_h = float(src_rc.h) / float(dst_rc.h);

  MAID_ASSERT( scale_w<1.0f || scale_h<1.0f, "拡大はできません" );

  for( int y=0; y<dst_rc.h; ++y )
  {
    for( int x=0; x<dst_rc.w; ++x )
    {
      const float src_x_begin = float(src_rc.x) + (x * scale_w);
      const float src_x_end   = src_x_begin + scale_w;
      const float src_y_begin = float(src_rc.y) + (y * scale_h);
      const float src_y_end   = src_y_begin + scale_h;

	    int	BeginI;
	    float BeginF;
	    FloatDiv( src_y_begin, BeginI, BeginF );

	    int	EndI;
	    float EndF;
	    FloatDiv( src_y_end, EndI, EndF );

      float channel[4]={0,0,0,0};	// 0:a  1:r  2:g  3:b

      if( BeginF<1.0f)
      {//  一番上の色
        const COLOR_R32G32B32A32F c = GetLineTotal( Src, src_x_begin, src_x_end, BeginI );
        channel[0] = c.GetA() * (1.0f-BeginF);
        channel[1] = c.GetR() * (1.0f-BeginF);
        channel[2] = c.GetG() * (1.0f-BeginF);
        channel[3] = c.GetB() * (1.0f-BeginF);
      }
      {//  途中の色
        for( int32 i=BeginI+1; i<EndI; ++i )
        {
          const COLOR_R32G32B32A32F c = GetLineTotal( Src, src_x_begin, src_x_end, i );
          channel[0] += c.GetA();
          channel[1] += c.GetR();
          channel[2] += c.GetG();
          channel[3] += c.GetB();
        }
      }
      if( 0<EndF ) //  値が 64.0 とかちょうどだと、このピクセルがないことがあるので、そうゆうときは無視する
      {//  一番下の色
        const COLOR_R32G32B32A32F c = GetLineTotal( Src, src_x_begin, src_x_end, EndI );
        channel[0] += c.GetA() * EndF;
        channel[1] += c.GetR() * EndF;
        channel[2] += c.GetG() * EndF;
        channel[3] += c.GetB() * EndF;
      }

			const float Plane = (src_x_end-src_x_begin) * (src_y_end-src_y_begin);
			for( int i=0; i<NUMELEMENTS(channel); ++i )
			{
				channel[i] /=  Plane;

				if( 1.0f < channel[i] ) { channel[i] = 1.0f; }
			}

    	const COLOR_R32G32B32A32F c( channel[1], channel[2], channel[3], channel[0] );

      const POINT2DI dst_pos(dst_rc.x+x, dst_rc.y+y);
      Dst.SetPixel( dst_pos, c);

    }
  }
}


  }
}

