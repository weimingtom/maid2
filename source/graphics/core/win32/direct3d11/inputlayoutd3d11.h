#ifndef maid2_graphics_core_win32_direct3d11_inputlayoutd3d11_h
#define maid2_graphics_core_win32_direct3d11_inputlayoutd3d11_h


#include"../../../../config/define.h"
#include"../../iinputlayout.h"

#include"common.h"


namespace Maid { namespace Graphics {

  class InputLayoutD3D11 : public IInputLayout
  {
  public:
    InputLayoutD3D11( const SPD3D11INPUTLAYOUT& p )
      :pDecl(p){}
    SPD3D11INPUTLAYOUT  pDecl;
  };

}}

#endif