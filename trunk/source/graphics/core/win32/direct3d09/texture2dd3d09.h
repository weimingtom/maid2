#ifndef maid2_graphics_core_texture2dd3d09_h
#define maid2_graphics_core_texture2dd3d09_h


#include"../../../../config/define.h"
#include"../../itexture2d.h"
#include"common.h"
#include"id3d09object.h"

namespace Maid { namespace Graphics {

  class Texture2DD3D09 
    : public ITexture2D
    , public ID3D09Object
  {
  public:
    Texture2DD3D09( const CREATERETEXTURE2DPARAM& param, const SPD3D09TEXTURE& pTex );
    Texture2DD3D09( const CREATERETEXTURE2DPARAM& param, const SPD3D09TEXTURE& pTex, const SPD3D09SURFACE& pSurf );
    Texture2DD3D09( const CREATERETEXTURE2DPARAM& param, const SPD3D09SURFACE& pSurf, bool sw );

    SPD3D09TEXTURE  pTexture;
    SPD3D09SURFACE  pSurface;

    const bool      IsSwapChainSurface;

  protected:
    void  Escape();
    void  Restore( DeviceD3D09& Device  );

  };

}}

#endif