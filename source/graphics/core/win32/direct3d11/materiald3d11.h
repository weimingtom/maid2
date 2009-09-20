#ifndef maid2_graphics_core_win32_direct3d11_materiald3d11_h
#define maid2_graphics_core_win32_direct3d11_materiald3d11_h


#include"../../../../config/define.h"
#include"../../imaterial.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class MaterialD3D11
    : public IMaterial
  {
  public:
    MaterialD3D11( const SPRESOURCE& pRes, const CREATEMATERIALPARAM& param, const SPD3D11SHADERRESOURCEVIEW& p )
      :IMaterial(pRes,param),pView(p){}
    SPD3D11SHADERRESOURCEVIEW pView;
  };

}}

#endif