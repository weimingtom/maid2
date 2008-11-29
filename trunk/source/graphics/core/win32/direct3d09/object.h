#ifndef maid2_graphics_core_win32_direct3d09_object_h
#define maid2_graphics_core_win32_direct3d09_object_h

#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include"../../idevice.h"

#include"d3d09include.h"


namespace Maid { namespace Graphics {

  struct IObjectInfo
  {
    virtual ~IObjectInfo(){}

    enum TYPE
    {
      TYPE_INDEX,
      TYPE_VERTEX,
      TYPE_TEXTURE2D,
      TYPE_RENDERTARGET,
      TYPE_DEPTHSTENCIL,
      TYPE_VERTEXSHADER,
      TYPE_PIXELSHADER,
    };

    IObjectInfo( TYPE t ) : Type(t){}

    const TYPE Type;
  };

  struct IndexBufferInfo : public IObjectInfo
  {
    IndexBufferInfo():IObjectInfo(TYPE_INDEX){}
    SPD3D09INDEXBUFFER pIndex;
  };

  struct VertexBufferInfo : public IObjectInfo
  {
    IndexBufferInfo():IObjectInfo(TYPE_VERTEX){}
    SPD3D09VERTEXBUFFER pIndex;
  };

  struct TextureInfo : public IObjectInfo
  {
    IndexBufferInfo():IObjectInfo(TYPE_TEXTURE2D){}
    SPD3D09TEXTURE pIndex;
  };

  struct RenderTargetInfo : public IObjectInfo
  {
    IndexBufferInfo():IObjectInfo(TYPE_RENDERTARGET){}
    SPD3D09SURFACE pResource;
  };

  struct DepthStencilInfo : public IObjectInfo
  {
    IndexBufferInfo():IObjectInfo(TYPE_DEPTHSTENCIL){}
    SPD3D09SURFACE pResource;
  };

/*
  //  D3D9はテクスチャしか使えないのでいらない
  struct ShaderMaterialInfo : public IObjectInfo
  {
    SPD3D09SURFACE pResource;
  };
*/

  struct PixelShaderInfo : public IObjectInfo
  {
    PixelShaderInfo():IObjectInfo(TYPE_PIXELSHADER){}
    SPD3D09PIXELSHADER pShader;
  };

  struct VertexShaderInfo : public IObjectInfo
  {
    VertexShaderInfo():IObjectInfo(TYPE_VERTEXSHADER){}
    SPD3D09VERTEXSHADER pShader;
  };


  typedef boost::shared_ptr<IObjectInfo>  SPOBJECTINFO;
}}

#endif