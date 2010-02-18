#ifndef maid2_graphics_index_h
#define maid2_graphics_index_h

/*!
    @file
    @brief  頂点インデックスを扱うクラス
 */


#include"../config/define.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"graphicscore.h"

namespace Maid
{
  class Index
      :public IIndex
      ,public GlobalPointer<GraphicsCore>
  {
  public:
    Index();
    virtual ~Index();

    void Create( const void* pData, size_t Size );
    void Delete();

    size_t  GetSize() const;
  };	
}


#endif