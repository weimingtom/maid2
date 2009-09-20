#ifndef maid2_graphics_core_win32_direct3d11_blendstated3d11_h
#define maid2_graphics_core_win32_direct3d11_blendstated3d11_h


#include"../../../../config/define.h"
#include"../../iblendstate.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class BlendStateD3D11
    : public IBlendState
  {
  public:
    BlendStateD3D11( const BLENDSTATEPARAM& param, const SPD3D11BLENDSTATE& v ) 
      :IBlendState(param)
      ,pState(v){}
    SPD3D11BLENDSTATE pState;
  };

}}

#endif