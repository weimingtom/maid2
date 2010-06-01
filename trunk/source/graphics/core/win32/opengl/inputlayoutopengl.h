#ifndef maid2_graphics_core_win32_opengl_inputlayoutopengl_h
#define maid2_graphics_core_win32_opengl_inputlayoutopengl_h


#include"../../../../config/define.h"
#include"../../iinputlayout.h"

#include"common.h"


namespace Maid { namespace Graphics {

  class InputLayoutOpenGL
    : public IInputLayout
  {
  public:
    InputLayoutOpenGL( const std::vector<INPUT_ELEMENT>& e )
      :Element(e)
    {}
    const std::vector<INPUT_ELEMENT>  Element;
  };

}}

#endif