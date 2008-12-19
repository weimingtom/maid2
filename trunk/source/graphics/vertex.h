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
#include"core/core.h"

namespace Maid
{
  class Vertex
      :public IVertex
      ,public GlobalPointer<Graphics::Core>
  {
  public:
    Vertex();
    virtual ~Vertex();

    void Create( size_t Length, unt32 Format );
    void Create( const void* pData, size_t Length, unt32 Format );
    void Delete();

  private:
    Vertex( const Vertex& rha ){} //  そのうち書く
    Vertex& operator = ( const Vertex& rha ){} //  そのうち書く

  };	
}

#endif