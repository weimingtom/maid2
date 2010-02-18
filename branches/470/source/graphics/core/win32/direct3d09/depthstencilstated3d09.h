#ifndef maid2_graphics_core_depthstencilstated3d09_h
#define maid2_graphics_core_depthstencilstated3d09_h


#include"../../../../config/define.h"
#include"../../idepthstencilstate.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  class DepthStencilStateD3D09 : public IDepthStencilState
  {
  public:
    DepthStencilStateD3D09( const RENDERSTATELIST& v ) : RenderState(v){}
    RENDERSTATELIST RenderState;
  };


}}

#endif