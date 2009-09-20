#ifndef maid2_graphics_core_win32_direct3d11_depthstencild3d11_h
#define maid2_graphics_core_win32_direct3d11_depthstencild3d11_h


#include"../../../../config/define.h"
#include"../../idepthstencil.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class DepthStencilD3D11 
    : public IDepthStencil
  {
  public:
    DepthStencilD3D11( const SPRESOURCE& pRes, const CREATEDEPTHSTENCILPARAM& param, const SPD3D11DEPTHSTENCILVIEW& p )
      :IDepthStencil(pRes,param),pView(p){}
    SPD3D11DEPTHSTENCILVIEW pView;
  };

}}

#endif