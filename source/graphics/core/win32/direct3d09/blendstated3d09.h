#ifndef maid2_graphics_core_blendstated3d09_h
#define maid2_graphics_core_blendstated3d09_h


#include"../../../../config/define.h"
#include"../../iblendstate.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class BlendStateD3D09 : public IBlendState
  {
  public:
    BlendStateD3D09( const BLENDSTATEPARAM& param, const RENDERSTATELIST& v ) 
      :IBlendState(param)
      ,RenderState(v){}
    RENDERSTATELIST RenderState;
  };

}}

#endif