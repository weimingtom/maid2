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

  class VertexShaderD3D09 : public IOtherObject
  {
  public:
    VertexShaderD3D09( IDirect3DVertexShader9* p, DWORD v):pShader(p),Version(v){}
    SPD3D09VERTEXSHADER pShader;
    DWORD  Version;
  };


  class PixelShaderD3D09 : public IOtherObject
  {
  public:
    virtual void Setup( const SPD3D09DEVICE& pDevice ) = 0;
  };

  class SamplerStateD3D09 : public IOtherObject
  {
  public:
  };

  struct RENDERSTATE
  {
    RENDERSTATE(){}
    RENDERSTATE( D3DRENDERSTATETYPE t, DWORD v ):Type(t),Value(v){}
    D3DRENDERSTATETYPE  Type;
    DWORD               Value;
  };
  typedef std::vector<RENDERSTATE> RENDERSTATELIST;

  class RasterizerStateD3D09 : public IOtherObject
  {
  public:
    RasterizerStateD3D09( const RENDERSTATELIST& list ):RenderState(list){}
    RENDERSTATELIST  RenderState;
  };

  class BlendStateD3D09 : public IOtherObject
  {
  public:
    BlendStateD3D09( const RENDERSTATELIST& list ):RenderState(list){}
    RENDERSTATELIST  RenderState;
  };

}}

#endif