#ifndef maid2_graphics_texture2dmemory_h
#define maid2_graphics_texture2dmemory_h

/*!
    @file
    @brief  システムメモリから書き込む用の2Dテクスチャ
 */


#include"../config/define.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"graphicscore.h"

namespace Maid
{
  class Texture2DMemory
    :public ITexture2D
    ,private GlobalPointer<GraphicsCore>
  {
  public:
    Texture2DMemory();
    Texture2DMemory( const Graphics::SPTEXTURE2D& pTex );

  private:

  };


}

#endif