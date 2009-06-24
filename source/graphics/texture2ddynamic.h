#ifndef maid2_graphics_texture2ddynamic_h
#define maid2_graphics_texture2ddynamic_h

/*!
    @file
    @brief  システムメモリから書き込む用の2Dテクスチャ
 */


#include"../config/define.h"
#include"../auxiliary/globalpointer.h"

#include"texture2dbase.h"

namespace Maid
{
  class Texture2DDynamic
    :public Texture2DBase
    ,private GlobalPointer<GraphicsCore>
  {
  public:
    void Create( const SIZE2DI& Size, PIXELFORMAT fmt );

  private:

  };


}

#endif