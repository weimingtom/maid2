#ifndef maid2_auxiliary_mathematics_vector_h
#define maid2_auxiliary_mathematics_vector_h

#include"../../config/define.h"
#include"vector2d.h"
#include"vector3d.h"
#include"vector4d.h"

namespace Maid
{
  typedef VECTOR2D_TEMPLATE<int>    VECTOR2DI;
  typedef VECTOR2D_TEMPLATE<float>  VECTOR2DF;
  typedef VECTOR2D_TEMPLATE<double> VECTOR2DD;

  typedef VECTOR3D_TEMPLATE<int>    VECTOR3DI;
  typedef VECTOR3D_TEMPLATE<float>  VECTOR3DF;
  typedef VECTOR3D_TEMPLATE<double> VECTOR3DD;

  typedef VECTOR4D_TEMPLATE<int>    VECTOR4DI;
  typedef VECTOR4D_TEMPLATE<float>  VECTOR4DF;
  typedef VECTOR4D_TEMPLATE<double> VECTOR4DD;
}

#endif