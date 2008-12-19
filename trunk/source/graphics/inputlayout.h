#ifndef maid2_graphics_inputlayout_h
#define maid2_graphics_inputlayout_h

/*!
    @file
    @brief  InputLayout
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"

#include"graphicsobjecttemplate.h"
#include"core/core.h"

namespace Maid
{
  class InputLayout
      :public IInputLayout
      ,public GlobalPointer<Graphics::Core>
  {
  public:
    InputLayout();
    virtual ~InputLayout();

    void Create( const Graphics::IDevice::INPUT_ELEMENT* pElement, int Count,  const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength );
    void Delete();

  private:
    InputLayout( const InputLayout& rha ){} //  そのうち書く。そもそもコピー禁止でいいかもしれない
    InputLayout& operator = ( const InputLayout& rha ){} //  そのうち書く。そもそもコピー禁止でいいかもしれない

  };	
}

#endif