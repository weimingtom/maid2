#ifndef maid2_graphics_transiter_fill_h
#define maid2_graphics_transiter_fill_h

#include"../../config/define.h"
#include"../../auxiliary/mathematics/rect.h"
#include"../surface.h"


namespace Maid
{
  namespace Transiter
  {
    void Fill( const COLOR_R32G32B32A32F& Color, Surface& Dst  );
    void Fill( const COLOR_R32G32B32A32F& Color, Surface& Dst, const RECT2DI& rect );
  }
}

#endif
