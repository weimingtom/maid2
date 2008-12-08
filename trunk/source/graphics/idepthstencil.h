#ifndef maid2_graphics_idepthstencil_h
#define maid2_graphics_idepthstencil_h

/*!
    @file
    @brief  DepthStencil の基本クラス
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"

#include"core/idevice.h"

namespace Maid
{
  class IDepthStencil
  {
  public:
    virtual ~IDepthStencil(){}

    const Graphics::IDevice::DEPTHSTENCIL& GetID()const { return m_DepthStencil; }
  protected:
    Graphics::IDevice::DEPTHSTENCIL m_DepthStencil;
  };	
	
}