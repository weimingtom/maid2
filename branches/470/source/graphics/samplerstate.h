#ifndef maid2_graphics_samplerstate_h
#define maid2_graphics_samplerstate_h

/*!
    @file
    @brief  SamplerState
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"graphicscore.h"

namespace Maid
{
  class SamplerState
      :public ISamplerState
      ,private GlobalPointer<GraphicsCore>
  {
  public:
    SamplerState();
    virtual ~SamplerState();

    void Create( const Graphics::SAMPLERSTATEPARAM& state );
    void Delete();
  };	
}

#endif