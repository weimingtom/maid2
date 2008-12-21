#ifndef maid2_graphics_depthstencilstate_h
#define maid2_graphics_depthstencilstate_h

/*!
    @file
    @brief  DepthStencilState
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"core/core.h"

namespace Maid
{
  class DepthStencilState
      :public IDepthStencilState
      ,public GlobalPointer<Graphics::Core>
  {
  public:
    DepthStencilState();
    virtual ~DepthStencilState();

    void Create( const Graphics::DEPTHSTENCILSTATEPARAM& state );
    void Delete();

  private:
    DepthStencilState( const DepthStencilState& rha ){} //  そのうち書く。
    DepthStencilState& operator = ( const DepthStencilState& rha ){} //  そのうち書く。

  };	
}

#endif