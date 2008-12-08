#ifndef maid2_graphics_irendertarget_h
#define maid2_graphics_irendertarget_h

/*!
    @file
    @brief  RenderTarget の基本クラス
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"

#include"core/idevice.h"

namespace Maid
{
  class IRenderTarget
  {
  public:
    virtual ~IRenderTarget(){}

    const Graphics::IDevice::RENDERTARGET& GetID()const { return m_RenderTarget; }
  protected:
    Graphics::IDevice::RENDERTARGET m_RenderTarget;
  };	
	
}