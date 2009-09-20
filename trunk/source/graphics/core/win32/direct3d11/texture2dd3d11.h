#ifndef maid2_graphics_core_win32_direct3d11_texture2dd3d11_h
#define maid2_graphics_core_win32_direct3d11_texture2dd3d11_h


#include"../../../../config/define.h"
#include"../../itexture2d.h"
#include"common.h"


namespace Maid { namespace Graphics {

  class Texture2DD3D11 
    : public ITexture2D
  {
  public:
    Texture2DD3D11( const CREATERETEXTURE2DPARAM& param, const SPD3D11TEXTURE2D& p )
      :ITexture2D(param), pTexture(p){}

    SPD3D11TEXTURE2D  pTexture;
  };

}}

#endif