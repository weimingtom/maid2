#ifndef maid2_graphics_core_win32_direct3d11_rendertargetd3d11_h
#define maid2_graphics_core_win32_direct3d11_rendertargetd3d11_h


#include"../../../../config/define.h"
#include"../../irendertarget.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class RenderTargetD3D11 : public IRenderTarget
  {
  public:
    RenderTargetD3D11( const SPRESOURCE& pRes, const CREATERENDERTARGETPARAM& param, const SPD3D11RENDERTARGETVIEW& p )
      :IRenderTarget(pRes,param)
      ,pView(p)
    {}

    SPD3D11RENDERTARGETVIEW pView;
  };

}}

#endif