#ifndef maid2_auxiliary_mathematics_matrix_h
#define maid2_auxiliary_mathematics_matrix_h

#include"../../config/define.h"
#include"matrix4d.h"

namespace Maid
{
  typedef MATRIX4D_TEMPLATE<int>    MATRIX4DI;
  typedef MATRIX4D_TEMPLATE<float>  MATRIX4DF;
  typedef MATRIX4D_TEMPLATE<double> MATRIX4DD;
}

#endif