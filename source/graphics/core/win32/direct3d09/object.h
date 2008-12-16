#ifndef maid2_graphics_core_win32_direct3d09_object_h
#define maid2_graphics_core_win32_direct3d09_object_h

#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include"../../hObject.h"

#include"common.h"


namespace Maid { namespace Graphics {

  struct IOtherObject
  {

  };

  struct SamplerStateD3D09 : public IOtherObject
  {
  };

  struct RasterizerStateD3D09 : public IOtherObject
  {
  };

  struct BlendStateD3D09 : public IOtherObject
  {
  };

}}

#endif