#ifndef maid2_graphics_core_rasterizerstated3d09_h
#define maid2_graphics_core_rasterizerstated3d09_h


#include"../../../../config/define.h"
#include"../../irasterizerstate.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class RasterizerStateD3D09 : public IRasterizerState
  {
  public:
    RasterizerStateD3D09( const RASTERIZERSTATEPARAM& param, const RENDERSTATELIST& v ) 
      :IRasterizerState(param)
      ,RenderState(v){}
    RENDERSTATELIST RenderState;
  };

}}

#endif