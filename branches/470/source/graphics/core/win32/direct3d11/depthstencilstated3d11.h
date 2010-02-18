#ifndef maid2_graphics_core_win32_direct3d11_depthstencilstated3d11_h
#define maid2_graphics_core_win32_direct3d11_depthstencilstated3d11_h


#include"../../../../config/define.h"
#include"../../idepthstencilstate.h"

#include"common.h"


namespace Maid { namespace Graphics {

  class DepthStencilStateD3D11 
    : public IDepthStencilState
  {
  public:
    DepthStencilStateD3D11( const SPD3D11DEPTHSTENCILSTATE& p ) : pState(p){}
    SPD3D11DEPTHSTENCILSTATE pState;
  };


}}

#endif