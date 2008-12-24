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
#include"graphicscore.h"

namespace Maid
{
  class DepthStencilState
      :public IDepthStencilState
      ,public GlobalPointer<GraphicsCore>
  {
  public:
    DepthStencilState();
    virtual ~DepthStencilState();

    void Create( const Graphics::DEPTHSTENCILSTATEPARAM& state );
    void Delete();

  };	
}

#endif