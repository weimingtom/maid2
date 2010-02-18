#ifndef maid2_graphics_core_win32_direct3d10_rendertargetd3d10_h
#define maid2_graphics_core_win32_direct3d10_rendertargetd3d10_h


#include"../../../../config/define.h"
#include"../../irendertarget.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class RenderTargetD3D10 : public IRenderTarget
  {
  public:
    RenderTargetD3D10( const SPRESOURCE& pRes, const CREATERENDERTARGETPARAM& param, const SPD3D10RENDERTARGETVIEW& p )
      :IRenderTarget(pRes,param)
      ,pView(p)
    {}

    SPD3D10RENDERTARGETVIEW pView;
  };

}}

#endif