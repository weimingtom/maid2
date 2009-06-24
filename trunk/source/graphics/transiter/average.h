#ifndef maid2_graphics_transiter_average_h
#define maid2_graphics_transiter_average_h

#include"../../config/define.h"
#include"../../auxiliary/mathematics/rect.h"
#include"../surface.h"


namespace Maid
{
  namespace Transiter
  {
    void Average( const Surface& Src, Surface& Dst );

    void Average( const Surface& Src, const RECT2DI& SrcRect, Surface& Dst, const RECT2DI& DstRect );
  }
}

#endif
