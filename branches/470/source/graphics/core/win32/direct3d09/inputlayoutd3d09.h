#ifndef maid2_graphics_core_inputlayoutd3d09_h
#define maid2_graphics_core_inputlayoutd3d09_h


#include"../../../../config/define.h"
#include"../../iinputlayout.h"

#include"common.h"


namespace Maid { namespace Graphics {

  class InputLayoutD3D09 : public IInputLayout
  {
  public:
    InputLayoutD3D09( IDirect3DVertexDeclaration9* p )
      :pDecl(p){}
    SPD3D09VERTEXDECLARATION  pDecl;
  };

}}

#endif