#ifndef maid2_graphics_core_win32_direct3d10_materiald3d10_h
#define maid2_graphics_core_win32_direct3d10_materiald3d10_h


#include"../../../../config/define.h"
#include"../../imaterial.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class MaterialD3D10
    : public IMaterial
  {
  public:
    MaterialD3D10( const SPRESOURCE& pRes, const CREATEMATERIALPARAM& param, const SPD3D10SHADERRESOURCEVIEW& p )
      :IMaterial(pRes,param),pView(p){}
    SPD3D10SHADERRESOURCEVIEW pView;
  };

}}

#endif