#ifndef maid2_graphics_depthstencilstate_h
#define maid2_graphics_depthstencilstate_h

/*!
    @file
    @brief  DepthStencilState
 */


#include"../config/define.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"graphicscore.h"
#include"rendertargettexture.h"

namespace Maid
{
  class DepthStencilState
      :public IDepthStencilState
      ,public GlobalPointer<GraphicsCore>
  {
  public:
    void Create( const Graphics::DEPTHSTENCILSTATEPARAM& param );
    void Delete();

  };	
}

#endif