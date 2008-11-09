#ifndef maid2_auxiliary_mathematics_rect_h
#define maid2_auxiliary_mathematics_rect_h

#include"../../config/define.h"
#include"rect2d.h"
#include"rect3d.h"

namespace Maid
{
  typedef RECT2D_TEMPLATE<int>      RECT2DI;
  typedef RECT2D_TEMPLATE<float>    RECT2DF;
  typedef RECT2D_TEMPLATE<double>   RECT2DD;

  typedef RECT3D_TEMPLATE<int>      RECT3DI;
  typedef RECT3D_TEMPLATE<float>    RECT3DF;
  typedef RECT3D_TEMPLATE<double>   RECT3DD;
}

#endif