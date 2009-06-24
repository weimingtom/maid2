#ifndef maid2_graphics_core_win32_direct3d10_pixelshaderd3d10_h
#define maid2_graphics_core_win32_direct3d10_pixelshaderd3d10_h


#include"../../../../config/define.h"
#include"../../ipixelshader.h"
#include"common.h"


namespace Maid { namespace Graphics {

  class PixelShaderD3D10 : public IPixelShader
  {
  public:
    PixelShaderD3D10( const SPD3D10PIXELSHADER& p )
      :pShader(p){}

    SPD3D10PIXELSHADER  pShader;
  };

}}

#endif