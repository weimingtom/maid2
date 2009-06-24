﻿#ifndef maid2_graphics_transiter_copy_h
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
    void Copy( const Surface& Src, const RECT2DI& SrcRect, Surface& Dst, const RECT2DI& DstRect );
  }
}

#endif
