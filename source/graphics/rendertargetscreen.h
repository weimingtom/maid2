#ifndef maid2_graphics_rendertargetscreen_h
#define maid2_graphics_rendertargetscreen_h

/*!
    @file
    @brief  スクリーンが持っている RenderTarget
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"

#include"irendertarget.h"

namespace Maid
{
  class RenderTargetScreen : public IRenderTarget
  {
  public:
 
    void SetOBJECT( const Graphics::IDevice::RENDERTARGET& obj )
    {
      m_RenderTarget = obj;
    }
  };	
	
}