#ifndef maid2_graphics_core_win32_direct3d10_inputlayoutd3d10_h
#define maid2_graphics_core_win32_direct3d10_inputlayoutd3d10_h


#include"../../../../config/define.h"
#include"../../iinputlayout.h"

#include"common.h"


namespace Maid { namespace Graphics {

  class InputLayoutD3D10 : public IInputLayout
  {
  public:
    InputLayoutD3D10( const SPD3D10INPUTLAYOUT& p )
      :pDecl(p){}
    SPD3D10INPUTLAYOUT  pDecl;
  };

}}

#endif