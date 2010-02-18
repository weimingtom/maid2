#ifndef maid2_graphics_core_vertexd3d09_h
#define maid2_graphics_core_vertexd3d09_h


#include"../../../../config/define.h"

#include"../../ibuffer.h"
#include"common.h"

namespace Maid { namespace Graphics {

  class VertexD3D09 : public IBuffer
  {
  public:
    VertexD3D09( const CREATEBUFFERPARAM& param, IDirect3DVertexBuffer9* p ):IBuffer(param),pBuffer(p){}
    SPD3D09VERTEXBUFFER  pBuffer;
  };

}}

#endif