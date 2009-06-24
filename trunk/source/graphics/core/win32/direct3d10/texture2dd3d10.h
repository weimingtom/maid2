#ifndef maid2_graphics_core_win32_direct3d10_texture2dd3d10_h
#define maid2_graphics_core_win32_direct3d10_texture2dd3d10_h


#include"../../../../config/define.h"
#include"../../itexture2d.h"
#include"common.h"


namespace Maid { namespace Graphics {

  class Texture2DD3D10 
    : public ITexture2D
  {
  public:
    Texture2DD3D10( const CREATERETEXTURE2DPARAM& param, const SPD3D10TEXTURE2D& p )
      :ITexture2D(param), pTexture(p){}

    SPD3D10TEXTURE2D  pTexture;
  };

}}

#endif