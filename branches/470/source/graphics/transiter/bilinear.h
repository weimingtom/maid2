#ifndef maid2_graphics_transiter_bilinear_h
#define maid2_graphics_transiter_bilinear_h

#include"../../config/define.h"
#include"../../auxiliary/mathematics/rect.h"
#include"../surface.h"


namespace Maid
{
  namespace Transiter
  {
    void Bilinear( const Surface& Src, Surface& Dst );

    void Bilinear( const Surface& Src, const RECT2DI& SrcRect, Surface& Dst, const RECT2DI& DstRect );
  }
}

#endif
