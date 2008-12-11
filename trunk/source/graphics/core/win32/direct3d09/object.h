#ifndef maid2_graphics_core_win32_direct3d09_object_h
#define maid2_graphics_core_win32_direct3d09_object_h

#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include"../../iobject.h"

#include"d3d09include.h"


namespace Maid { namespace Graphics {

  class SamplerState : public ISamplerState
  {
  };

  class RasterizerState : public IRasterizerState
  {
  };
  class BlendState : public IBlendState
  {
  };

}}

#endif