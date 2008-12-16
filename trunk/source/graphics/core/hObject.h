/*!
    @file
    @brief デバイスが扱うオブジェクトを定義する
 */

#ifndef maid2_graphics_core_hObject_h
#define maid2_graphics_core_hObject_h


#include"../../config/define.h"
#include"../../auxiliary/string.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Graphics {

  struct hObject
  {
      typedef uintptr_t INSTANCEID;
      enum
      {
        ID_NONE = ~0  //  無効なオブジェクトＩＤの場合
      };
      const INSTANCEID ID;

      hObject():ID(ID_NONE){}
      hObject( INSTANCEID id ):ID(id){}
      hObject( const hObject& obj ):ID(obj.ID){}

      hObject& operator=(const hObject& rha )
      {
        const_cast<uintptr_t&>(ID) = rha.ID;

        return *this;
      }
  };
    /*
       こうゆう定義を行うマクロ
        struct child : public parent
        {
          child( INSTANCEID id ):parent(id){}
        };
    */
#define OBJECTDEFINE(parent,child)  struct child : public parent { child(){} child(hObject::INSTANCEID id):parent(id){} };

    OBJECTDEFINE(hObject,hResource);

    OBJECTDEFINE(hResource,hBuffer);
    OBJECTDEFINE(hResource,hVertex);
    OBJECTDEFINE(hResource,hIndex);
    OBJECTDEFINE(hResource,hTexture2D);
    OBJECTDEFINE(hResource,hConstant);

    OBJECTDEFINE(hObject,hRenderTarget);
    OBJECTDEFINE(hObject,hDepthStencil);
    OBJECTDEFINE(hObject,hMaterial);

    OBJECTDEFINE(hObject,hSamplerState);
    OBJECTDEFINE(hObject,hRasterizerState);
    OBJECTDEFINE(hObject,hBlendState);

    OBJECTDEFINE(hObject,hInputLayout);
    OBJECTDEFINE(hObject,hVertexShader);
    OBJECTDEFINE(hObject,hHullShader);
    OBJECTDEFINE(hObject,hDomainShader);
    OBJECTDEFINE(hObject,hGeometryShader);
    OBJECTDEFINE(hObject,hPixelShader);


    #undef OBJECTDEFINE //  ここ以外ではいらないでしょう



    struct ObjectDesc
    {
      enum TYPE
      {
        TYPE_UNKWON,  //  不明

        TYPE_RESOURCEVERTEX,
        TYPE_RESOURCEINDEX,
        TYPE_RESOURCETEXTURE2D,
        TYPE_RESOURCECONSTANT,

        TYPE_RENDERTARGET,
        TYPE_DEPTHSTENCIL,
        TYPE_MATERIAL,

        TYPE_SAMPLERSTATE,
        TYPE_RASTERIZESTATE,
        TYPE_BLENDSTATE,

        TYPE_INPUTLAYOUT,
        TYPE_VERTEXSHADER,
        TYPE_HULLSHADER,
        TYPE_DOMAINSHADER,
        TYPE_GEOMETRYSHADER,
        TYPE_PIXELSHADER,
      };

      ObjectDesc( TYPE t ):Type(t){}
      const TYPE  Type;
    };

  typedef	boost::shared_ptr<ObjectDesc>	SPOBJECTDESC;

}}

#endif