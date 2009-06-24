#ifndef maid2_graphics_core_samplerstated3d09_h
#define maid2_graphics_core_samplerstated3d09_h


#include"../../../../config/define.h"
#include"../../isamplerstate.h"

#include"common.h"

namespace Maid { namespace Graphics {

  struct SAMPLERSTATE
  {
    SAMPLERSTATE(){}
    SAMPLERSTATE( D3DSAMPLERSTATETYPE t, DWORD v ):Type(t),Value(v){}
    D3DSAMPLERSTATETYPE  Type;
    DWORD                Value;
  };
  typedef std::vector<SAMPLERSTATE> SAMPLERSTATELIST;

  class SamplerStateD3D09 : public ISamplerState
  {
  public:
    SamplerStateD3D09( const SAMPLERSTATEPARAM& param, const SAMPLERSTATELIST& list )
      :ISamplerState(param)
      ,State(list)
    {
    }

    SAMPLERSTATELIST State;
  };

}}

#endif