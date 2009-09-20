#ifndef maid2_graphics_core_win32_direct3d11_samplerstated3d11_h
#define maid2_graphics_core_win32_direct3d11_samplerstated3d11_h


#include"../../../../config/define.h"
#include"../../isamplerstate.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class SamplerStateD3D11 : public ISamplerState
  {
  public:
    SamplerStateD3D11( const SAMPLERSTATEPARAM& param, const SPD3D11SAMPLERSTATE& p )
      :ISamplerState(param)
      ,pState(p)
    {
    }

    SPD3D11SAMPLERSTATE pState;
  };

}}

#endif