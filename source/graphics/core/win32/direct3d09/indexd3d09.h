#ifndef maid2_graphics_core_indexd3d09_h
#define maid2_graphics_core_indexd3d09_h


#include"../../../../config/define.h"

#include"../../ibuffer.h"
#include"common.h"

namespace Maid { namespace Graphics {

  class IndexD3D09 : public IBuffer
  {
  public:
    IndexD3D09( const CREATEBUFFERPARAM& param, IDirect3DIndexBuffer9* p ):IBuffer(param),pObject(p){}
    SPD3D09INDEXBUFFER  pObject;
  };

}}

#endif