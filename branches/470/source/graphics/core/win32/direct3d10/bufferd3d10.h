#ifndef maid2_graphics_core_win32_direct3d10_bufferd3d10_h
#define maid2_graphics_core_win32_direct3d10_bufferd3d10_h


#include"../../../../config/define.h"

#include"../../ibuffer.h"
#include"common.h"

namespace Maid { namespace Graphics {

  class BufferD3D10 
    : public IBuffer
  {
  public:
    BufferD3D10( const CREATEBUFFERPARAM& param, const SPD3D10BUFFER& p )
      :IBuffer(param), pBuffer(p){}

    SPD3D10BUFFER  pBuffer;
  };

}}

#endif