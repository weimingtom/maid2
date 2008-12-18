#ifndef maid2_graphics_core_win32_direct3d09_object_h
#define maid2_graphics_core_win32_direct3d09_object_h

#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include"../../hObject.h"

#include"common.h"


namespace Maid { namespace Graphics {

  class IOtherObject
  {
  public:
    virtual ~IOtherObject(){}
  };

  class PixelShaderD3D09 : public IOtherObject
  {
  public:
    virtual void Setup( const SPD3D09DEVICE& pDevice ) = 0;
  };

  class SamplerStateD3D09 : public IOtherObject
  {
  };

  class RasterizerStateD3D09 : public IOtherObject
  {
  };

  class BlendStateD3D09 : public IOtherObject
  {
  };

}}

#endif