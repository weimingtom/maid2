#ifndef maid2_graphics_core_depthstencild3d09_h
#define maid2_graphics_core_depthstencild3d09_h


#include"../../../../config/define.h"
#include"../../idepthstencil.h"


namespace Maid { namespace Graphics {

  class DepthStencilD3D09 
    : public IDepthStencil
  {
  public:
    DepthStencilD3D09( const SPRESOURCE& pRes, const CREATEDEPTHSTENCILPARAM& param );
  };

}}

#endif