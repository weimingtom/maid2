#ifndef maid2_graphics_core_win32_direct3d11_pixelshaderd3d11_h
#define maid2_graphics_core_win32_direct3d11_pixelshaderd3d11_h


#include"../../../../config/define.h"
#include"../../ipixelshader.h"
#include"common.h"


namespace Maid { namespace Graphics {

  class PixelShaderD3D11 : public IPixelShader
  {
  public:
    PixelShaderD3D11( const SPD3D11PIXELSHADER& p )
      :pShader(p){}

    SPD3D11PIXELSHADER  pShader;
  };

}}

#endif