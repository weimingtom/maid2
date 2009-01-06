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
#include"graphicscore.h"
#include"rendertargetbase.h"

namespace Maid
{
  class RenderTargetScreen
    : public RenderTargetBase
    , public GlobalPointer<GraphicsCore>
  {
  public:
		void Initialize();
  };	
	
}

#endif