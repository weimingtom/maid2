/*!
	@file
	@brief	頂点バッファベースクラス
 */

#ifndef maid2_graphic_core_vertexbuffer_h
#define maid2_graphic_core_vertexbuffer_h

#include"../../../config/define.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  /*!
      @class	VertexBuffer vertexbuffer.h
      @brief	頂点バッファの基本クラス
   */
  class VertexBuffer
  {
  public:
    virtual ~VertexBuffer(){}	//!<	デストラクタ
  };

  typedef boost::shared_ptr<VertexBuffer> SPVERTEXBUFFER;
}

#endif

