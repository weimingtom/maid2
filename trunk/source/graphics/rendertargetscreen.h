#ifndef maid2_graphics_rendertargetscreen_h
#define maid2_graphics_rendertargetscreen_h

/*!
    @file
    @brief  スクリーンが持っている RenderTarget
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"core/core.h"

namespace Maid
{

  class RenderTargetScreen : public IRenderTarget, public GlobalPointer<Graphics::Core>
  {
  public:
    RenderTargetScreen();
  };	
	
}

#endif