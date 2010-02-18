/*!
	  @file
	  @brief DXGI 定義
 */

#ifndef maid2_graphics_core_win32_dxgitypedef_h
#define maid2_graphics_core_win32_dxgitypedef_h

#include"../../../config/define.h"
#include"../../../framework/win32/com_ptr.h"
#include"../../pixelformat.h"
#include"../createparam.h"
#include"../iinputlayout.h"
#include<dxgi.h>

namespace Maid { namespace Graphics {

  typedef com_ptr<IDXGIFactory>   SPDXGIFACTORY;
  typedef com_ptr<IDXGIFactory1>   SPDXGIFACTORY1;
  typedef com_ptr<IDXGIAdapter>   SPDXGIADAPTER;
  typedef com_ptr<IDXGIAdapter1>  SPDXGIADAPTER1;
  typedef com_ptr<IDXGISwapChain> SPDXGISWAPCHAIN;
  typedef com_ptr<IDXGIOutput>    SPDXGIOUTPUT;
  typedef com_ptr<IDXGISurface>   SPDXGISURFACE;


  /*!
      @fn D3DFORMAT PIXELFORMATtoD3DFORMAT( PIXELFORMAT eFormat )
   
      PIXELFORMAT を Direct3D10 で使用されている
  \n	ピクセルフォーマットに変換します
   
      @param	eFormat	[i ]	変換したいフォーマット
   
      @return	変換後のフォーマット
  \n		    	判別できなかったら D3DFMT_UNKNOWN
   */
  inline DXGI_FORMAT PIXELFORMATtoDXGI_FORMAT( PIXELFORMAT eFormat )
  {
    switch( eFormat )
    {
    case PIXELFORMAT_A08B08G08R08I:		{ return DXGI_FORMAT_R8G8B8A8_UNORM;	}break;
    case PIXELFORMAT_A02B10G10R10I:		{ return DXGI_FORMAT_R10G10B10A2_UNORM;	}break;
    case PIXELFORMAT_A16B16G16R16I:		{ return DXGI_FORMAT_R16G16B16A16_UNORM;	}break;
    case PIXELFORMAT_A16B16G16R16F:		{ return DXGI_FORMAT_R16G16B16A16_FLOAT;	}break;
    case PIXELFORMAT_A32B32G32R32F:		{ return DXGI_FORMAT_R32G32B32A32_FLOAT;	}break;

    case PIXELFORMAT_DXT1:				{ return DXGI_FORMAT_BC1_UNORM;		}break;
    case PIXELFORMAT_DXT2:				{ return DXGI_FORMAT_BC2_UNORM;		}break;
    case PIXELFORMAT_DXT3:				{ return DXGI_FORMAT_BC3_UNORM;		}break;
    case PIXELFORMAT_DXT4:				{ return DXGI_FORMAT_BC4_UNORM;		}break;
    case PIXELFORMAT_DXT5:				{ return DXGI_FORMAT_BC5_UNORM;		}break;

    case PIXELFORMAT_D16I:     { return DXGI_FORMAT_D16_UNORM;	  }break;
    case PIXELFORMAT_D24IS8:   { return DXGI_FORMAT_D24_UNORM_S8_UINT;  }break;
    }

    MAID_ASSERT( true, "識別できないPIXELFORMATです " << eFormat );

    return DXGI_FORMAT_UNKNOWN;
  }

  /*!
    @fn PIXELFORMAT D3DFORMATtoPIXELFORMAT( D3DFORMAT eFormat )
   
    DirectGraphic9 で使用されているピクセルフォーマットを
    PIXELFORMAT に変換します
   
    @param	Format	[i ]	変換したい頂点フォーマット
   
    @return	変換後のフォーマット
  \n			判別できなかったら PIXELFORMAT_NONE
   */
  inline PIXELFORMAT DXGI_FORMATtoPIXELFORMAT( DXGI_FORMAT Format )
  {
    switch( Format )
    {
    case DXGI_FORMAT_R8G8B8A8_UNORM:		{ return PIXELFORMAT_A08B08G08R08I;	}break;
    case DXGI_FORMAT_R10G10B10A2_UNORM:	{ return PIXELFORMAT_A02B10G10R10I;	}break;
    case DXGI_FORMAT_R16G16B16A16_UNORM:{ return PIXELFORMAT_A16B16G16R16I;	}break;
    case DXGI_FORMAT_R16G16B16A16_FLOAT:{ return PIXELFORMAT_A16B16G16R16F;	}break;
    case DXGI_FORMAT_R32G32B32A32_FLOAT:{ return PIXELFORMAT_A32B32G32R32F;	}break;

    case DXGI_FORMAT_BC1_UNORM:		    	{ return PIXELFORMAT_DXT1;	}break;
    case DXGI_FORMAT_BC2_UNORM:			    { return PIXELFORMAT_DXT2;	}break;
    case DXGI_FORMAT_BC3_UNORM:			    { return PIXELFORMAT_DXT3;	}break;
    case DXGI_FORMAT_BC4_UNORM:			    { return PIXELFORMAT_DXT4;	}break;
    case DXGI_FORMAT_BC5_UNORM:			    { return PIXELFORMAT_DXT5;	}break;

    case DXGI_FORMAT_D16_UNORM:			    { return PIXELFORMAT_D16I;	}break;
    case DXGI_FORMAT_D24_UNORM_S8_UINT:	{ return PIXELFORMAT_D24IS8;	}break;
    }

    return PIXELFORMAT_NONE;
  }


  inline SWAPCHAINFORMAT::SCANLINEORDER DXGI_MODE_SCANLINE_ORDERtoSWAPCHAINFORMATSCANLINEORDER( DXGI_MODE_SCANLINE_ORDER type )
  {
    SWAPCHAINFORMAT::SCANLINEORDER ret = SWAPCHAINFORMAT::SCANLINEORDER_PROGRESSIVE;
    switch( type )
    {
    case DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED:		    { ret = SWAPCHAINFORMAT::SCANLINEORDER_UNSPECIFIED;	}break;
    case DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE:		    { ret = SWAPCHAINFORMAT::SCANLINEORDER_PROGRESSIVE;	}break;
    case DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST:	{ ret = SWAPCHAINFORMAT::SCANLINEORDER_INTERLACE_U;	}break;
    case DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST:	{ ret = SWAPCHAINFORMAT::SCANLINEORDER_INTERLACE_L;	}break;
    }
    return ret;
  }

  inline DXGI_MODE_SCANLINE_ORDER SWAPCHAINFORMATSCANLINEORDERtoDXGI_MODE_SCANLINE_ORDER( SWAPCHAINFORMAT::SCANLINEORDER type )
  {
    DXGI_MODE_SCANLINE_ORDER ret = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
    switch( type )
    {
    case SWAPCHAINFORMAT::SCANLINEORDER_UNSPECIFIED:	{ ret = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;	      }break;
    case SWAPCHAINFORMAT::SCANLINEORDER_PROGRESSIVE:	{ ret = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;	      }break;
    case SWAPCHAINFORMAT::SCANLINEORDER_INTERLACE_U:	{ ret = DXGI_MODE_SCANLINE_ORDER_UPPER_FIELD_FIRST;	}break;
    case SWAPCHAINFORMAT::SCANLINEORDER_INTERLACE_L:	{ ret = DXGI_MODE_SCANLINE_ORDER_LOWER_FIELD_FIRST;	}break;
    }
    return ret;
  }

  inline SWAPCHAINFORMAT::SCALING DXGI_MODE_SCALINGtoDISPLAYMODESCALING( DXGI_MODE_SCALING type )
  {
    SWAPCHAINFORMAT::SCALING ret = SWAPCHAINFORMAT::SCALING_STRETCHED;
    switch( type )
    {
    case DXGI_MODE_SCALING_UNSPECIFIED:	{ ret = SWAPCHAINFORMAT::SCALING_UNSPECIFIED;	}break;
    case DXGI_MODE_SCALING_CENTERED:		{ ret = SWAPCHAINFORMAT::SCALING_CENTERED;	}break;
    case DXGI_MODE_SCALING_STRETCHED:	  { ret = SWAPCHAINFORMAT::SCALING_STRETCHED;	}break;
    }
    return ret;
  }

  inline DXGI_MODE_SCALING SWAPCHAINFORMATSCALINGtoDXGI_MODE_SCALING( SWAPCHAINFORMAT::SCALING type )
  {
    DXGI_MODE_SCALING ret = DXGI_MODE_SCALING_STRETCHED;
    switch( type )
    {
    case SWAPCHAINFORMAT::SCALING_UNSPECIFIED:{ ret = DXGI_MODE_SCALING_UNSPECIFIED;	}break;
    case SWAPCHAINFORMAT::SCALING_CENTERED:	  { ret = DXGI_MODE_SCALING_CENTERED;	    }break;
    case SWAPCHAINFORMAT::SCALING_STRETCHED:	{ ret = DXGI_MODE_SCALING_STRETCHED;	  }break;
    }
    return ret;
  }


  inline DXGI_FORMAT INPUT_ELEMENTTYPEtoDXGI_FORMAT( INPUT_ELEMENT::TYPE t )
  {
    DXGI_FORMAT ret;

    switch( t )
    {
    case INPUT_ELEMENT::TYPE_FLOAT1: { ret = DXGI_FORMAT_R32_FLOAT; }break;
    case INPUT_ELEMENT::TYPE_FLOAT2: { ret = DXGI_FORMAT_R32G32_FLOAT; }break;
    case INPUT_ELEMENT::TYPE_FLOAT3: { ret = DXGI_FORMAT_R32G32B32_FLOAT; }break;
    case INPUT_ELEMENT::TYPE_FLOAT4: { ret = DXGI_FORMAT_R32G32B32A32_FLOAT; }break;
    case INPUT_ELEMENT::TYPE_COLOR:  { ret = DXGI_FORMAT_R8G8B8A8_UNORM; }break;
    }
    return ret;
  }

}}


#endif