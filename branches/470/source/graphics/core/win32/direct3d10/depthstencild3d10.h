#ifndef maid2_graphics_core_win32_direct3d10_depthstencild3d10_h
#define maid2_graphics_core_win32_direct3d10_depthstencild3d10_h


#include"../../../../config/define.h"
#include"../../idepthstencil.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class DepthStencilD3D10 
    : public IDepthStencil
  {
  public:
    DepthStencilD3D10( const SPRESOURCE& pRes, const CREATEDEPTHSTENCILPARAM& param, const SPD3D10DEPTHSTENCILVIEW& p )
      :IDepthStencil(pRes,param),pView(p){}
    SPD3D10DEPTHSTENCILVIEW pView;
  };

}}

#endif