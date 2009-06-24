#ifndef maid2_graphics_rendertargettexture_h
#define maid2_graphics_rendertargettexture_h

/*!
    @file
    @brief  テクスチャに使える RenderTarget
 */


#include"../config/define.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"texture2dbase.h"
#include"rendertargetbase.h"

namespace Maid
{
  class GraphicsCore;
  class RenderTargetTexture
    :public Texture2DBase
    ,public RenderTargetBase
    ,private GlobalPointer<GraphicsCore>
  {
  public:
    void Create( const SIZE2DI& size, PIXELFORMAT format );
    void Destroy();

    void GenerateMips();
  };
	
}

#endif