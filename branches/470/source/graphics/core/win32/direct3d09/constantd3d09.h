#ifndef maid2_graphics_core_constantd3d09_h
#define maid2_graphics_core_constantd3d09_h


#include"../../../../config/define.h"

#include"../../iconstant.h"
#include"common.h"

#include<vector>

namespace Maid { namespace Graphics {

  class ConstantD3D09 : public IConstant
  {
  public:
    ConstantD3D09( const CREATEBUFFERPARAM& param )
      :IConstant(param)
      ,Buffer(param.Length)
    {}

    std::vector<unt08>  Buffer;
  };

}}

#endif