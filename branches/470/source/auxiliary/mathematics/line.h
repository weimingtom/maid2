#ifndef maid2_auxiliary_mathematics_line_h
#define maid2_auxiliary_mathematics_line_h

#include"../../config/define.h"
#include"line2d.h"
#include"line3d.h"

namespace Maid
{
  typedef LINE2D_TEMPLATE<int>      LINE2DI;
  typedef LINE2D_TEMPLATE<float>    LINE2DF;
  typedef LINE2D_TEMPLATE<double>   LINE2DD;

  typedef LINE3D_TEMPLATE<int>      LINE3DI;
  typedef LINE3D_TEMPLATE<float>    LINE3DF;
  typedef LINE3D_TEMPLATE<double>   LINE3DD;
}

#endif