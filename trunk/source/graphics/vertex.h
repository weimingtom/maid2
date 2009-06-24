#ifndef maid2_graphics_vertex_h
#define maid2_graphics_vertex_h

/*!
    @file
    @brief  CPU側で書き換え可能な頂点バッファ
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"graphicscore.h"

namespace Maid
{
  class Vertex
      :public IVertex
      ,private GlobalPointer<GraphicsCore>
  {
  public:
    Vertex();
    virtual ~Vertex();

    void Create( size_t Length );
    void Create( const void* pData, size_t Length );
    void Delete();
  };	
}

#endif