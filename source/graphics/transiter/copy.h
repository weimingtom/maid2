#ifndef maid2_graphics_transiter_copy_h
#define maid2_graphics_transiter_copy_h

#include"../../config/define.h"
#include"../../auxiliary/mathematics/rect.h"
#include"../../auxiliary/mathematics/point.h"
#include"../surface.h"


namespace Maid
{
  namespace Transiter
  {
    void Copy( const Surface& Src, Surface& Dst );
    void Copy( const Surface& Src, const RECT2DI& rect, Surface& Dst, const POINT2DI& pos );
  }
}

#endif
