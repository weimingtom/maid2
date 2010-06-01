#ifndef maid2_graphics_core_win32_opengl_rendertargetopengl_h
#define maid2_graphics_core_win32_opengl_rendertargetopengl_h


#include"../../../../config/define.h"
#include"../../irendertarget.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class RenderTargetOpenGL 
    : public IRenderTarget
  {
  public:
    RenderTargetOpenGL( const SPRESOURCE& pRes, const CREATERENDERTARGETPARAM& param )
      :IRenderTarget(pRes,param)
    {}
  };

}}

#endif