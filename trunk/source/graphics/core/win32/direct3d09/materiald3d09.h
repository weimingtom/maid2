#ifndef maid2_graphics_core_materiald3d09_h
#define maid2_graphics_core_materiald3d09_h


#include"../../../../config/define.h"
#include"../../imaterial.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class MaterialD3D09 : public IMaterial
  {
  public:
    MaterialD3D09( const SPRESOURCE& pRes, const CREATEMATERIALPARAM& param ):IMaterial(pRes,param){}
  };

}}

#endif