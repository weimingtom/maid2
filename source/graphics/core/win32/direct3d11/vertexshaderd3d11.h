#ifndef maid2_graphics_core_win32_direct3d11_vertexshaderd3d11_h
#define maid2_graphics_core_win32_direct3d11_vertexshaderd3d11_h


#include"../../../../config/define.h"
#include"../../ivertexshader.h"
#include"common.h"


namespace Maid { namespace Graphics {

  class VertexShaderD3D11 
    : public IVertexShader
  {
  public:
    VertexShaderD3D11( const SPD3D11VERTEXSHADER& p )
      :pShader(p){}
    SPD3D11VERTEXSHADER pShader;
  };

}}

#endif