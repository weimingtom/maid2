#ifndef maid2_graphics_core_win32_opengl_materialopengl_h
#define maid2_graphics_core_win32_opengl_materialopengl_h


#include"../../../../config/define.h"
#include"../../imaterial.h"

#include"common.h"

namespace Maid { namespace Graphics {

  class MaterialOpenGL
    : public IMaterial
  {
  public:
    MaterialOpenGL( const SPRESOURCE& pRes, const CREATEMATERIALPARAM& param )
      :IMaterial(pRes,param)
    {}
  };

}}

#endif