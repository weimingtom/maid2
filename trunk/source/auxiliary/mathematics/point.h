#ifndef maid2_auxiliary_mathematics_point3_h
#define maid2_auxiliary_mathematics_point3_h

#include"../../config/define.h"
#include"point2d.h"
#include"point3d.h"
#include"point4d.h"

namespace Maid
{
  typedef POINT2D_TEMPLATE<int>     POINT2DI;
  typedef POINT2D_TEMPLATE<float>   POINT2DF;
  typedef POINT2D_TEMPLATE<double>  POINT2DD;

  typedef POINT3D_TEMPLATE<int>     POINT3DI;
  typedef POINT3D_TEMPLATE<float>   POINT3DF;
  typedef POINT3D_TEMPLATE<double>  POINT3DD;

  typedef POINT4D_TEMPLATE<int>     POINT4DI;
  typedef POINT4D_TEMPLATE<float>   POINT4DF;
  typedef POINT4D_TEMPLATE<double>  POINT4DD;
}

#endif