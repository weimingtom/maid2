#ifndef maid2_graphics_core_win32_direct3d10_blendstated3d10_h
#define maid2_graphics_core_win32_direct3d10_blendstated3d10_h


#include"../../../../config/define.h"
#include"../../iblendstate.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class BlendStateD3D10
    : public IBlendState
  {
  public:
    BlendStateD3D10( const BLENDSTATEPARAM& param, const SPD3D10BLENDSTATE& v ) 
      :IBlendState(param)
      ,pState(v){}
    SPD3D10BLENDSTATE pState;
  };

}}

#endif