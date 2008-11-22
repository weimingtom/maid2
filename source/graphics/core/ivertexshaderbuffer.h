/*!
    @file
    @brief	バーテックスシェーダーバッファ
 */

#ifndef maid2_graphic_core_ivertexshaderbuffer_h
#define maid2_graphic_core_ivertexshaderbuffer_h



#include"../../../config/define.h"


#include<boost/smart_ptr.hpp>

namespace Maid
{
  class IVertexShaderBuffer
  {
  public:
    virtual ~IVertexShaderBuffer(){}
  };

  /*!
    @brief		IVertexShaderBuffer クラスの共有ポインタ
   */
  typedef boost::shared_ptr<IVertexShaderBuffer> SPVERTEXSHADERBUFFER;	

}

#endif