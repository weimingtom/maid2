﻿#ifndef graphic_core_driver_win32_direct3d11_common_h
#define graphic_core_driver_win32_direct3d11_common_h

/*!	
    @file
    @brief	このディレクトリの共通ヘッダ
*/


#include"../../../../config/define.h"
#include"../../../../config/typedef.h"

#include"../../../../auxiliary/macro.h"
#include"../../../../auxiliary/debug/assert.h"
#include"../../../../auxiliary/debug/warning.h"

#include"../../../../framework/win32/com_ptr.h"

#include"../../../pixelformat.h"
#include"../../idevice.h"
#include"../dxgitypedef.h"

#include<d3d11.h>
#include<d3dx11.h>
#include<string>

namespace Maid { namespace Graphics {

  typedef com_ptr<IUnknown>           SPD3D11UNKNOWN;
  typedef com_ptr<ID3D11Device>       SPD3D11DEVICE;

  typedef com_ptr<ID3D11DeviceContext>       SPD3D11DEVICECONTEXT;

  typedef com_ptr<ID3D11Buffer>       SPD3D11BUFFER;
  typedef com_ptr<ID3D11Texture2D>    SPD3D11TEXTURE2D;
  typedef com_ptr<ID3D11DepthStencilView>    SPD3D11DEPTHSTENCILVIEW;
  typedef com_ptr<ID3D11RenderTargetView>    SPD3D11RENDERTARGETVIEW;
  typedef com_ptr<ID3D11ShaderResourceView>  SPD3D11SHADERRESOURCEVIEW;

  typedef com_ptr<ID3D11InputLayout >   SPD3D11INPUTLAYOUT;
  typedef com_ptr<ID3D11SamplerState>   SPD3D11SAMPLERSTATE;
  typedef com_ptr<ID3D11VertexShader>   SPD3D11VERTEXSHADER;
  typedef com_ptr<ID3D11PixelShader>    SPD3D11PIXELSHADER;
  typedef com_ptr<ID3D11BlendState>     SPD3D11BLENDSTATE;
  typedef com_ptr<ID3D11DepthStencilState>  SPD3D11DEPTHSTENCILSTATE;
  typedef com_ptr<ID3D11RasterizerState>    SPD3D11RASTERIZERSTATE;




  inline D3D11_USAGE RESOURCEUSAGEtoD3D11_USAGE( RESOURCEUSAGE use )
  {
    D3D11_USAGE ret;
    switch( use )
    {
    case RESOURCEUSAGE_DEFAULT: { ret = D3D11_USAGE_DEFAULT; }break;
    case RESOURCEUSAGE_IMMUTABLE: { ret = D3D11_USAGE_IMMUTABLE; }break;
    case RESOURCEUSAGE_DYNAMIC: { ret = D3D11_USAGE_DYNAMIC; }break;
    case RESOURCEUSAGE_STAGING: { ret = D3D11_USAGE_STAGING; }break;
    case RESOURCEUSAGE_GPUWRITE: { ret = D3D11_USAGE_DEFAULT; }break;

    }
    return ret;
  }

  inline D3D11_RTV_DIMENSION RENDERTARGETDIMENSIONtoD3D11_RTV_DIMENSION( CREATERENDERTARGETPARAM::DIMENSION src )
  {
    D3D11_RTV_DIMENSION ret;
    switch( src )
    {
    case CREATERENDERTARGETPARAM::DIMENSION_BUFFER:           { ret = D3D11_RTV_DIMENSION_BUFFER; }break;
    case CREATERENDERTARGETPARAM::DIMENSION_TEXTURE1D:        { ret = D3D11_RTV_DIMENSION_TEXTURE1D; }break;
    case CREATERENDERTARGETPARAM::DIMENSION_TEXTURE1DARRAY:   { ret = D3D11_RTV_DIMENSION_TEXTURE1DARRAY;   }break;
    case CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2D:        { ret = D3D11_RTV_DIMENSION_TEXTURE2D;        }break;
    case CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2DARRAY:   { ret = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;   }break;
    case CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2DMS:      { ret = D3D11_RTV_DIMENSION_TEXTURE2DMS;      }break;
    case CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2DMSARRAY: { ret = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY; }break;
    case CREATERENDERTARGETPARAM::DIMENSION_TEXTURE3D:        { ret = D3D11_RTV_DIMENSION_TEXTURE3D;        }break;
    }
    return ret;
  }

  inline CREATERENDERTARGETPARAM::DIMENSION D3D11_RTV_DIMENSIONtoRENDERTARGETDIMENSION( D3D11_RTV_DIMENSION src )
  {
    CREATERENDERTARGETPARAM::DIMENSION ret;
    switch( src )
    {
    case D3D11_RTV_DIMENSION_BUFFER:           { ret = CREATERENDERTARGETPARAM::DIMENSION_BUFFER;           }break;
    case D3D11_RTV_DIMENSION_TEXTURE1D:        { ret = CREATERENDERTARGETPARAM::DIMENSION_TEXTURE1D;        }break;
    case D3D11_RTV_DIMENSION_TEXTURE1DARRAY:   { ret = CREATERENDERTARGETPARAM::DIMENSION_TEXTURE1DARRAY;   }break;
    case D3D11_RTV_DIMENSION_TEXTURE2D:        { ret = CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2D;        }break;
    case D3D11_RTV_DIMENSION_TEXTURE2DARRAY:   { ret = CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2DARRAY;   }break;
    case D3D11_RTV_DIMENSION_TEXTURE2DMS:      { ret = CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2DMS;      }break;
    case D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY: { ret = CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2DMSARRAY; }break;
    case D3D11_RTV_DIMENSION_TEXTURE3D:        { ret = CREATERENDERTARGETPARAM::DIMENSION_TEXTURE3D;        }break;
    }
    return ret;
  }




  inline D3D11_DSV_DIMENSION DEPTHSTENCILDIMENSIONtoD3D11_DSV_DIMENSION( CREATEDEPTHSTENCILPARAM::DIMENSION src )
  {
    D3D11_DSV_DIMENSION ret;
    switch( src )
    {
    case CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE1D:        { ret = D3D11_DSV_DIMENSION_TEXTURE1D; }break;
    case CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE1DARRAY:   { ret = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;   }break;
    case CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE2D:        { ret = D3D11_DSV_DIMENSION_TEXTURE2D;        }break;
    case CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE2DARRAY:   { ret = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;   }break;
    case CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE2DMS:      { ret = D3D11_DSV_DIMENSION_TEXTURE2DMS;      }break;
    case CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE2DMSARRAY: { ret = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY; }break;
    }
    return ret;
  }


  inline CREATEDEPTHSTENCILPARAM::DIMENSION D3D11_DSV_DIMENSIONtoDEPTHSTENCILDIMENSION( D3D11_DSV_DIMENSION src )
  {
    CREATEDEPTHSTENCILPARAM::DIMENSION ret;
    switch( src )
    {
    case D3D11_DSV_DIMENSION_TEXTURE1D:        { ret = CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE1D;        }break;
    case D3D11_DSV_DIMENSION_TEXTURE1DARRAY:   { ret = CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE1DARRAY;   }break;
    case D3D11_DSV_DIMENSION_TEXTURE2D:        { ret = CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE2D;        }break;
    case D3D11_DSV_DIMENSION_TEXTURE2DARRAY:   { ret = CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE2DARRAY;   }break;
    case D3D11_DSV_DIMENSION_TEXTURE2DMS:      { ret = CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE2DMS;      }break;
    case D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY: { ret = CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE2DMSARRAY; }break;
    }
    return ret;
  }



  inline D3D11_SRV_DIMENSION MATERIALDIMENSIONtoD3D11_SRV_DIMENSION( CREATEMATERIALPARAM::DIMENSION src )
  {
    D3D11_SRV_DIMENSION ret;
    switch( src )
    {
    case CREATEMATERIALPARAM::DIMENSION_BUFFER:           { ret = D3D11_SRV_DIMENSION_BUFFER; }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE1D:        { ret = D3D11_SRV_DIMENSION_TEXTURE1D; }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE1DARRAY:   { ret = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;   }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE2D:        { ret = D3D11_SRV_DIMENSION_TEXTURE2D;        }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE2DARRAY:   { ret = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;   }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE2DMS:      { ret = D3D11_SRV_DIMENSION_TEXTURE2DMS;      }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE2DMSARRAY: { ret = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY; }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURE3D:        { ret = D3D11_SRV_DIMENSION_TEXTURE3D;        }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURECUBE:      { ret = D3D11_SRV_DIMENSION_TEXTURECUBE;        }break;
    case CREATEMATERIALPARAM::DIMENSION_TEXTURECUBEARRAY: { ret = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;        }break;
    }
    return ret;
  }

  inline CREATEMATERIALPARAM::DIMENSION D3D11_SRV_DIMENSION_MATERIALDIMENSION( D3D11_SRV_DIMENSION src )
  {
    CREATEMATERIALPARAM::DIMENSION ret;
    switch( src )
    {
    case D3D11_SRV_DIMENSION_BUFFER:           { ret = CREATEMATERIALPARAM::DIMENSION_BUFFER;           }break;
    case D3D11_SRV_DIMENSION_TEXTURE1D:        { ret = CREATEMATERIALPARAM::DIMENSION_TEXTURE1D;        }break;
    case D3D11_SRV_DIMENSION_TEXTURE1DARRAY:   { ret = CREATEMATERIALPARAM::DIMENSION_TEXTURE1DARRAY;   }break;
    case D3D11_SRV_DIMENSION_TEXTURE2D:        { ret = CREATEMATERIALPARAM::DIMENSION_TEXTURE2D;        }break;
    case D3D11_SRV_DIMENSION_TEXTURE2DARRAY:   { ret = CREATEMATERIALPARAM::DIMENSION_TEXTURE2DARRAY;   }break;
    case D3D11_SRV_DIMENSION_TEXTURE2DMS:      { ret = CREATEMATERIALPARAM::DIMENSION_TEXTURE2DMS;      }break;
    case D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY: { ret = CREATEMATERIALPARAM::DIMENSION_TEXTURE2DMSARRAY; }break;
    case D3D11_SRV_DIMENSION_TEXTURE3D:        { ret = CREATEMATERIALPARAM::DIMENSION_TEXTURE3D;        }break;
    case D3D11_SRV_DIMENSION_TEXTURECUBE:      { ret = CREATEMATERIALPARAM::DIMENSION_TEXTURECUBE;      }break;
    case D3D11_SRV_DIMENSION_TEXTURECUBEARRAY: { ret = CREATEMATERIALPARAM::DIMENSION_TEXTURECUBEARRAY; }break;
    }
    return ret;
  }


  inline D3D11_COMPARISON_FUNC CMPTYPEtoD3D11_COMPARISON_FUNC( COMPARISON Type )
  {
    D3D11_COMPARISON_FUNC ret = D3D11_COMPARISON_LESS;

    switch( Type )
    {
    case COMPARISON_NEVER:       { return D3D11_COMPARISON_NEVER;         }break;
    case COMPARISON_LESS:        { return D3D11_COMPARISON_LESS;          }break;
    case COMPARISON_EQUAL:       { return D3D11_COMPARISON_EQUAL;         }break;
    case COMPARISON_LESSEQUAL:   { return D3D11_COMPARISON_LESS_EQUAL;	  }break;
    case COMPARISON_GREATER:     { return D3D11_COMPARISON_GREATER;       }break;
    case COMPARISON_NOTEQUAL:    { return D3D11_COMPARISON_NOT_EQUAL;     }break;
    case COMPARISON_GREATEREQUAL:{ return D3D11_COMPARISON_GREATER_EQUAL; }break;
    case COMPARISON_ALWAYS:      { return D3D11_COMPARISON_ALWAYS;        }break;
    }

    return ret;
  }


  inline D3D11_FILL_MODE FILLtoD3D11_FILL_MODE( RASTERIZERSTATEPARAM::FILL Type )
  {
    D3D11_FILL_MODE ret = D3D11_FILL_SOLID;

    switch( Type )
    {
    case RASTERIZERSTATEPARAM::FILL_WIREFRAME: { ret = D3D11_FILL_WIREFRAME; }break;
    case RASTERIZERSTATEPARAM::FILL_SOLID:     { ret = D3D11_FILL_SOLID; }break;
    }

    return ret;
  }

  inline D3D11_CULL_MODE CULLINGtoD3D11_CULL_MODE( RASTERIZERSTATEPARAM::CULLING Type )
  {
    D3D11_CULL_MODE ret = D3D11_CULL_NONE;

    switch( Type )
    {
    case RASTERIZERSTATEPARAM::CULLING_NONE:  { ret = D3D11_CULL_NONE; }break;
    case RASTERIZERSTATEPARAM::CULLING_LEFT:  { ret = D3D11_CULL_FRONT;}break;
    case RASTERIZERSTATEPARAM::CULLING_RIGHT: { ret = D3D11_CULL_BACK; }break;
    }

    return ret;
  }


  inline D3D11_BLEND TARGETBLENDtoD3D11_BLEND( BLENDSTATEPARAM::TARGET::BLEND b )
  {
    D3D11_BLEND ret = D3D11_BLEND_ZERO;

    switch( b )
    {
    case BLENDSTATEPARAM::TARGET::BLEND_ZERO:          { ret = D3D11_BLEND_ZERO; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_ONE:           { ret = D3D11_BLEND_ONE; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_SRC_COLOR:     { ret = D3D11_BLEND_SRC_COLOR; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_INV_SRC_COLOR: { ret = D3D11_BLEND_INV_SRC_COLOR; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_SRC_ALPHA:     { ret = D3D11_BLEND_SRC_ALPHA; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_INV_SRC_ALPHA: { ret = D3D11_BLEND_INV_SRC_ALPHA; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_DST_ALPHA:    { ret = D3D11_BLEND_DEST_ALPHA; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_INV_DST_ALPHA:{ ret = D3D11_BLEND_INV_DEST_ALPHA; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_DST_COLOR:    { ret = D3D11_BLEND_DEST_COLOR; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_INV_DST_COLOR:{ ret = D3D11_BLEND_INV_DEST_COLOR; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_SRC_ALPHA_SAT: { ret = D3D11_BLEND_SRC_ALPHA_SAT; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_BLEND_FACTOR:  { ret = D3D11_BLEND_BLEND_FACTOR; }break;
    case BLENDSTATEPARAM::TARGET::BLEND_INV_BLEND_FACTOR:{ ret = D3D11_BLEND_INV_BLEND_FACTOR; }break;
    }
    return ret;
  }

  inline D3D11_BLEND_OP TARGETOPERATIONtoD3D11_BLEND_OP( BLENDSTATEPARAM::TARGET::OPERATION op )
  {
    D3D11_BLEND_OP ret = D3D11_BLEND_OP_ADD;

    switch( op )
    {
    case BLENDSTATEPARAM::TARGET::OPERATION_ADD:         { ret = D3D11_BLEND_OP_ADD; }break;
    case BLENDSTATEPARAM::TARGET::OPERATION_SUBTRACT:    { ret = D3D11_BLEND_OP_SUBTRACT; }break;
    case BLENDSTATEPARAM::TARGET::OPERATION_REV_SUBTRACT:{ ret = D3D11_BLEND_OP_REV_SUBTRACT; }break;
    case BLENDSTATEPARAM::TARGET::OPERATION_MIN:         { ret = D3D11_BLEND_OP_MIN ; }break;
    case BLENDSTATEPARAM::TARGET::OPERATION_MAX:         { ret = D3D11_BLEND_OP_MAX; }break;
    }

    return ret;
  }



  inline BOOL booltoD3D11BOOL( bool b )
  {
    if( b ) { return TRUE; }
    return FALSE;
  }


  inline D3D11_TEXTURE_ADDRESS_MODE ADDRESStoD3D11_TEXTURE_ADDRESS_MODE( SAMPLERSTATEPARAM::ADDRESS type )
  {
    D3D11_TEXTURE_ADDRESS_MODE ret = D3D11_TEXTURE_ADDRESS_CLAMP;

    switch( type )
    {
    case SAMPLERSTATEPARAM::ADDRESS_WRAP:       { ret = D3D11_TEXTURE_ADDRESS_WRAP;     }break;
    case SAMPLERSTATEPARAM::ADDRESS_MIRROR:     { ret = D3D11_TEXTURE_ADDRESS_MIRROR;   }break;
    case SAMPLERSTATEPARAM::ADDRESS_CLAMP:      { ret = D3D11_TEXTURE_ADDRESS_CLAMP;    }break;
    case SAMPLERSTATEPARAM::ADDRESS_BORDER:     { ret = D3D11_TEXTURE_ADDRESS_BORDER;   }break;
    case SAMPLERSTATEPARAM::ADDRESS_MIRROR_ONCE:{ ret = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE; }break;
    }

    return ret;
  }

  inline D3D11_FILTER FILTERtoD3D11_FILTER( SAMPLERSTATEPARAM::FILTER min, SAMPLERSTATEPARAM::FILTER mag, SAMPLERSTATEPARAM::FILTER mip  )
  {
    D3D11_FILTER ret = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    if(  min==SAMPLERSTATEPARAM::FILTER_ANISOTROPIC
      || mag==SAMPLERSTATEPARAM::FILTER_ANISOTROPIC
      || mip==SAMPLERSTATEPARAM::FILTER_ANISOTROPIC
      )
    {
      ret = D3D11_FILTER_ANISOTROPIC;
    }
    else
    {
      if( min==SAMPLERSTATEPARAM::FILTER_POINT )
      {
        if( mag==SAMPLERSTATEPARAM::FILTER_POINT )
        {
          if( mip==SAMPLERSTATEPARAM::FILTER_POINT  ) { ret = D3D11_FILTER_MIN_MAG_MIP_POINT; }
          else if( mip==SAMPLERSTATEPARAM::FILTER_LINEAR ) { ret = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR; }
        }
        else if( mag==SAMPLERSTATEPARAM::FILTER_LINEAR )
        {
          if( mip==SAMPLERSTATEPARAM::FILTER_POINT  ) { ret = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT; }
          else if( mip==SAMPLERSTATEPARAM::FILTER_LINEAR ) { ret = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR; }
        }
      }
      else if( min==SAMPLERSTATEPARAM::FILTER_LINEAR )
      {
        if( mag==SAMPLERSTATEPARAM::FILTER_POINT )
        {
          if( mip==SAMPLERSTATEPARAM::FILTER_POINT  ) { ret = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT; }
          else if( mip==SAMPLERSTATEPARAM::FILTER_LINEAR ) { ret = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR; }
        }
        else if( mag==SAMPLERSTATEPARAM::FILTER_LINEAR )
        {
          if( mip==SAMPLERSTATEPARAM::FILTER_POINT  ) { ret = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT; }
          else if( mip==SAMPLERSTATEPARAM::FILTER_LINEAR ) { ret = D3D11_FILTER_MIN_MAG_MIP_LINEAR; }
        }
      }
    }

    return ret;
  }

}}

#endif