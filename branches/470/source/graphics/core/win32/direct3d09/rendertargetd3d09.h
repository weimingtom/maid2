#ifndef maid2_graphics_core_rendertargetd3d09_h
#define maid2_graphics_core_rendertargetd3d09_h


#include"../../../../config/define.h"
#include"../../irendertarget.h"
#include"texture2dd3d09.h"
#include"common.h"

namespace Maid { namespace Graphics {

  class RenderTargetD3D09 : public IRenderTarget
  {
  public:
    RenderTargetD3D09( const SPRESOURCE& pRes, const CREATERENDERTARGETPARAM& param )
      :IRenderTarget(pRes,param)
    {}

  };

}}

#endif