#ifndef maid2_graphics_core_win32_direct3d10_depthstencilstated3d10_h
#define maid2_graphics_core_win32_direct3d10_depthstencilstated3d10_h


#include"../../../../config/define.h"
#include"../../idepthstencilstate.h"

#include"common.h"


namespace Maid { namespace Graphics {

  class DepthStencilStateD3D10 
    : public IDepthStencilState
  {
  public:
    DepthStencilStateD3D10( const SPD3D10DEPTHSTENCILSTATE& p ) : pState(p){}
    SPD3D10DEPTHSTENCILSTATE pState;
  };


}}

#endif