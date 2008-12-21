#ifndef maid2_graphics_core_texture2dd3d09_h
#define maid2_graphics_core_texture2dd3d09_h


#include"../../../../config/define.h"
#include"../../itexture2d.h"
#include"common.h"

namespace Maid { namespace Graphics {

  class Texture2DD3D09 : public ITexture2D
  {
  public:
    Texture2DD3D09( const CREATERETEXTURE2DPARAM& param, IDirect3DTexture9* pTex )
      :ITexture2D(param), pTexture(pTex){}
    Texture2DD3D09( const CREATERETEXTURE2DPARAM& param, IDirect3DSurface9* pSurf )
      :ITexture2D(param), pSurface(pSurf){}
    Texture2DD3D09( const CREATERETEXTURE2DPARAM& param, const SPD3D09SURFACE& pSurf )
      :ITexture2D(param), pSurface(pSurf){}

    SPD3D09TEXTURE  pTexture; //  どちらか一方だけ存在している
    SPD3D09SURFACE  pSurface;
  };

}}

#endif