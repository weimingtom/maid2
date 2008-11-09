#ifndef maid2_auxiliary_mathematics_size_h
#define maid2_auxiliary_mathematics_size_h

#include"../../config/define.h"
#include"size2d.h"
#include"size3d.h"

namespace Maid
{
  typedef SIZE2D_TEMPLATE<int>      SIZE2DI;
  typedef SIZE2D_TEMPLATE<float>    SIZE2DF;
  typedef SIZE2D_TEMPLATE<double>   SIZE2DD;

  typedef SIZE3D_TEMPLATE<int>      SIZE3DI;
  typedef SIZE3D_TEMPLATE<float>    SIZE3DF;
  typedef SIZE3D_TEMPLATE<double>   SIZE3DD;
}


#endif
