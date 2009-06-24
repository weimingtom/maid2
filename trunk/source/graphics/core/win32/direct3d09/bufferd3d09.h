#ifndef maid2_graphics_core_bufferD3D09_h
#define maid2_graphics_core_bufferD3D09_h


#include"../../../../config/define.h"

#include"../../ibuffer.h"
#include"common.h"

namespace Maid { namespace Graphics {

  class IBufferD3D09 : public IBuffer
  {
  public:
    SPD3D09UNKNOWN  pObject;
  };

}}

#endif