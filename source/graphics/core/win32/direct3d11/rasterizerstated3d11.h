#ifndef maid2_graphics_core_win32_direct3d11_rasterizerstated3d11_h
#define maid2_graphics_core_win32_direct3d11_rasterizerstated3d11_h


#include"../../../../config/define.h"
#include"../../irasterizerstate.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class RasterizerStateD3D11 
    : public IRasterizerState
  {
  public:
    RasterizerStateD3D11( const RASTERIZERSTATEPARAM& param, const SPD3D11RASTERIZERSTATE& p ) 
      :IRasterizerState(param)
      ,pState(p){}
    SPD3D11RASTERIZERSTATE pState;
  };

}}

#endif