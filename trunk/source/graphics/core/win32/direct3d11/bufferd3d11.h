#ifndef maid2_graphics_core_win32_direct3d11_bufferd3d11_h
#define maid2_graphics_core_win32_direct3d11_bufferd3d11_h


#include"../../../../config/define.h"

#include"../../ibuffer.h"
#include"common.h"

namespace Maid { namespace Graphics {

  class BufferD3D11 
    : public IBuffer
  {
  public:
    BufferD3D11( const CREATEBUFFERPARAM& param, const SPD3D11BUFFER& p )
      :IBuffer(param), pBuffer(p){}

    SPD3D11BUFFER  pBuffer;
  };

}}

#endif