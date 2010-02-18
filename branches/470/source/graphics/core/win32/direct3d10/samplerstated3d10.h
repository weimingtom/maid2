#ifndef maid2_graphics_core_win32_direct3d10_samplerstated3d10_h
#define maid2_graphics_core_win32_direct3d10_samplerstated3d10_h


#include"../../../../config/define.h"
#include"../../isamplerstate.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class SamplerStateD3D10 : public ISamplerState
  {
  public:
    SamplerStateD3D10( const SAMPLERSTATEPARAM& param, const SPD3D10SAMPLERSTATE& p )
      :ISamplerState(param)
      ,pState(p)
    {
    }

    SPD3D10SAMPLERSTATE pState;
  };

}}

#endif