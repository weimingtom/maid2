#ifndef maid2_graphics_core_win32_direct3d10_vertexshaderd3d10_h
#define maid2_graphics_core_win32_direct3d10_vertexshaderd3d10_h


#include"../../../../config/define.h"
#include"../../ivertexshader.h"
#include"common.h"


namespace Maid { namespace Graphics {

  class VertexShaderD3D10 
    : public IVertexShader
  {
  public:
    VertexShaderD3D10( const SPD3D10VERTEXSHADER& p )
      :pShader(p){}
    SPD3D10VERTEXSHADER pShader;
  };

}}

#endif