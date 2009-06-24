#ifndef maid2_graphics_core_vertexshaderd3d09_h
#define maid2_graphics_core_vertexshaderd3d09_h


#include"../../../../config/define.h"
#include"../../ivertexshader.h"
#include"common.h"


namespace Maid { namespace Graphics {

  class VertexShaderD3D09 : public IVertexShader
  {
  public:
    VertexShaderD3D09( IDirect3DVertexShader9* p, DWORD v):pShader(p),Version(v){}
    SPD3D09VERTEXSHADER pShader;
    DWORD  Version;
  };

}}

#endif