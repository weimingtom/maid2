#ifndef maid2_graphics_rendertarget_h
#define maid2_graphics_rendertarget_h

#include"../config/define.h"
#include"../auxiliary/globalpointer.h"

#include"rendertargetbase.h"
#include"graphicsobjecttemplate.h"

namespace Maid
{
  class GraphicsCore;
  class RenderTarget
    : public RenderTargetBase
    , private GlobalPointer<GraphicsCore>
  {
  public:

    void Create( const SIZE2DI& size, PIXELFORMAT format );
    void Create( const SIZE2DI& virtualsize, const SIZE2DI& realsize, PIXELFORMAT format );
    void Destroy();
 
  };
	
}

#endif