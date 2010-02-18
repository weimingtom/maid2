#ifndef maid2_graphics_core_pixelshaderd3d09_h
#define maid2_graphics_core_pixelshaderd3d09_h


#include"../../../../config/define.h"
#include"../../ipixelshader.h"
#include"common.h"


namespace Maid { namespace Graphics {

  class PixelShaderD3D09 : public IPixelShader
  {
  public:
    virtual void Setup( const SPD3D09DEVICE& pDevice )=0;
  };

}}

#endif