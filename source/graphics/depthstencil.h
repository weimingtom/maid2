#ifndef maid2_graphics_depthstencil_h
#define maid2_graphics_depthstencil_h

/*!
    @file
    @brief  DepthStencilState
 */


#include"../config/define.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"

namespace Maid
{
  class RenderTargetBase;
  class GraphicsCore;
  class DepthStencil
      :public IDepthStencil
      ,public GlobalPointer<GraphicsCore>
  {
  public:
    void CreateCompatible( const RenderTargetBase& tex, PIXELFORMAT format );
    void Create( const SIZE2DI& size, PIXELFORMAT format );
    void Destroy();
  };	
}

#endif