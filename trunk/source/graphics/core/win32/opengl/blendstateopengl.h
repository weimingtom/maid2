#ifndef maid2_graphics_core_win32_openl_blendstateopengl_h
#define maid2_graphics_core_win32_openl_blendstateopengl_h


#include"../../../../config/define.h"
#include"../../iblendstate.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class BlendStateOpenGL
    : public IBlendState
  {
  public:
    BlendStateOpenGL( const BLENDSTATEPARAM& param ) 
      :IBlendState(param)
      {}
  };

}}

#endif