#ifndef maid2_graphics_core_win32_direct3d10_rasterizerstated3d10_h
#define maid2_graphics_core_win32_direct3d10_rasterizerstated3d10_h


#include"../../../../config/define.h"
#include"../../irasterizerstate.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class RasterizerStateD3D10 
    : public IRasterizerState
  {
  public:
    RasterizerStateD3D10( const RASTERIZERSTATEPARAM& param, const SPD3D10RASTERIZERSTATE& p ) 
      :IRasterizerState(param)
      ,pState(p){}
    SPD3D10RASTERIZERSTATE pState;
  };

}}

#endif