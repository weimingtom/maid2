#ifndef maid2_graphics_shaderconstant_h
#define maid2_graphics_shaderconstant_h


/*!
    @file
    @brief  シェーダー定数
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"graphicscore.h"

namespace Maid
{
  class ShaderConstant
    :public IConstant
    ,private GlobalPointer<GraphicsCore>
  {
  public:
    ShaderConstant();
    virtual ~ShaderConstant();

    void Create( size_t Length );
    void Delete();


  };
}


#endif
