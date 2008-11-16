/*!
    @file
    @brief	バーテックスシェーダーバッファ
 */

#ifndef maid2_graphic_core_vertexshaderbuffer_h
#define maid2_graphic_core_vertexshaderbuffer_h



#include"../../../config/define.h"


#include<boost/smart_ptr.hpp>

namespace Maid
{
  class VertexShaderBuffer
  {
  public:
    virtual ~VertexShaderBuffer(){}
  };

  /*!
    @brief		VertexShaderBuffer クラスの共有ポインタ
   */
  typedef boost::shared_ptr<VertexShaderBuffer> SPVERTEXSHADERBUFFER;	

}

#endif