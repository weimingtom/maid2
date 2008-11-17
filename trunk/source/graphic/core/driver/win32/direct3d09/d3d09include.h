#ifndef graphic_core_driver_win32_direct3d09_direct3d09include_h
#define graphic_core_driver_win32_direct3d09_direct3d09include_h

/*!	
    @file
    @brief	Direct3D9 を扱うのに便利なマクロ
*/


#include"../../../../../config/define.h"
#include"../../../../../config/typedef.h"

#include<d3d9.h>

#include"../../../../../auxiliary/macro.h"
#include"../../../../../framework/win32/com_ptr.h"
#include"../../../../../auxiliary/debug/assert.h"

#include"../../../../customvertextemplate.h"
#include"../../../../pixelformat.h"
#include"../../videocard.h"



namespace Maid
{
  typedef com_ptr<IDirect3D9>         SPD3D;
//  typedef com_ptr<IDirect3DDevice9>   SPD3DDEVICE;
//  typedef com_ptr<IDirect3DSurface9>  SPD3DSURFACEBUFFER;
//  typedef com_ptr<IDirect3DTexture9>  SPD3DTEXTUREBUFFER;
//  typedef com_ptr<IDirect3DIndexBuffer9>        SPD3DINDEXBUFFER;
//  typedef com_ptr<IDirect3DVertexBuffer9>       SPD3DVERTEXBUFFER;
//  typedef com_ptr<IDirect3DVertexShader9>       SPD3DVERTEXSHADERBUFFER;
//  typedef com_ptr<IDirect3DVertexDeclaration9>  SPD3DVERTEXDECLARATION;
//  typedef com_ptr<IDirect3DPixelShader9>        SPD3DPIXELSHADERBUFFER;


  /*!
    @fn DWORD VERTEXFORMAT2FVF( VERTEXFORMAT Format )
   
    VERTEXFORMAT を Direct3D9 で使用されている
  \n	FVFに変換します
   
    @param	Format	[i ]	変換したい頂点フォーマット
   
    @return	変換後のフォーマット
  \n			判別できなかったら 0
   */
  inline DWORD VERTEXFORMAT2FVF( unt32 Format )
  {
    static const struct 
    {
	    DWORD d3d;
	    unt32 maid;
    } data[] =
    {
	    { D3DFVF_XYZ      ,VERTEXFORMAT_XYZ	    },
	    { D3DFVF_XYZRHW   ,VERTEXFORMAT_XYZRHW	},
	    { D3DFVF_XYZB1    ,VERTEXFORMAT_BLENDCOUNT1	},
	    { D3DFVF_XYZB2    ,VERTEXFORMAT_BLENDCOUNT2	},
	    { D3DFVF_XYZB3    ,VERTEXFORMAT_BLENDCOUNT3	},
	    { D3DFVF_XYZB4    ,VERTEXFORMAT_BLENDCOUNT4	},
	    { D3DFVF_XYZB5    ,VERTEXFORMAT_BLENDCOUNT5	},
	    { D3DFVF_XYZW     ,VERTEXFORMAT_XYZW		},

	    { D3DFVF_NORMAL   ,VERTEXFORMAT_NORMAL	},
	    { D3DFVF_PSIZE    ,VERTEXFORMAT_PSIZE		},
	    { D3DFVF_DIFFUSE  ,VERTEXFORMAT_DIFFUSE	},
	    { D3DFVF_SPECULAR ,VERTEXFORMAT_SPECULAR},

	    { D3DFVF_TEX1     ,VERTEXFORMAT_TEXTURECOUNT1},
	    { D3DFVF_TEX2     ,VERTEXFORMAT_TEXTURECOUNT2},
	    { D3DFVF_TEX3     ,VERTEXFORMAT_TEXTURECOUNT3},
	    { D3DFVF_TEX4     ,VERTEXFORMAT_TEXTURECOUNT4},
	    { D3DFVF_TEX5     ,VERTEXFORMAT_TEXTURECOUNT5},
	    { D3DFVF_TEX6     ,VERTEXFORMAT_TEXTURECOUNT6},
	    { D3DFVF_TEX7     ,VERTEXFORMAT_TEXTURECOUNT7},
	    { D3DFVF_TEX8     ,VERTEXFORMAT_TEXTURECOUNT8},
	    { D3DFVF_LASTBETA_UBYTE4   ,VERTEXFORMAT_LASTBETA_UBYTE4  },
	    { D3DFVF_LASTBETA_D3DCOLOR ,VERTEXFORMAT_LASTBETA_COLOR   },
    };
    DWORD ret = 0;

    for( int i=0; i<NUMELEMENTS(data); ++i )
    {
      if( IsFlag(Format,data[i].maid) ) { ret |= data[i].d3d; }
    }

    return ret;
  }

  /*!
      @fn D3DFORMAT PIXELFORMATtoD3DFORMAT( PIXELFORMAT eFormat )
   
      PIXELFORMAT を Direct3D9 で使用されている
  \n	ピクセルフォーマットに変換します
   
      @param	eFormat	[i ]	変換したいフォーマット
   
      @return	変換後のフォーマット
  \n		    	判別できなかったら D3DFMT_UNKNOWN
   */
  inline D3DFORMAT PIXELFORMATtoD3DFORMAT( PIXELFORMAT eFormat )
  {
    switch( eFormat )
    {
    case PIXELFORMAT_P08A08R08G08B08I:	{ return D3DFMT_P8;			}break;
    case PIXELFORMAT_P08X08R08G08B08I:	{ return D3DFMT_P8;			}break;

    case PIXELFORMAT_R05G06B05I:		{ return D3DFMT_R5G6B5;		}break;
    case PIXELFORMAT_A04R04G04B04I:	{ return D3DFMT_A4R4G4B4;	}break;
    case PIXELFORMAT_X01R05G05B05I:	{ return D3DFMT_X1R5G5B5;	}break;
    case PIXELFORMAT_A01R05G05B05I:	{ return D3DFMT_A1R5G5B5;	}break;

    case PIXELFORMAT_R08G08B08I:	  	{ return D3DFMT_R8G8B8;		}break;

    case PIXELFORMAT_A08R08G08B08I:		{ return D3DFMT_A8R8G8B8;	}break;
    case PIXELFORMAT_X08R08G08B08I:		{ return D3DFMT_X8R8G8B8;	}break;

    case PIXELFORMAT_A02R10G10B10I:		{ return D3DFMT_A2R10G10B10;	}break;
    case PIXELFORMAT_A02B10G10R10I:		{ return D3DFMT_A2B10G10R10;	}break;
    case PIXELFORMAT_A16B16G16R16I:		{ return D3DFMT_A16B16G16R16;	}break;
    case PIXELFORMAT_A16B16G16R16F:		{ return D3DFMT_A16B16G16R16F;	}break;
    case PIXELFORMAT_A32B32G32R32F:		{ return D3DFMT_A32B32G32R32F;	}break;

    case PIXELFORMAT_DXT1:				{ return D3DFMT_DXT1;		}break;
    case PIXELFORMAT_DXT2:				{ return D3DFMT_DXT2;		}break;
    case PIXELFORMAT_DXT3:				{ return D3DFMT_DXT3;		}break;
    case PIXELFORMAT_DXT4:				{ return D3DFMT_DXT4;		}break;
    case PIXELFORMAT_DXT5:				{ return D3DFMT_DXT5;		}break;

    case PIXELFORMAT_D16:   { return D3DFMT_D16;		}break;
    case PIXELFORMAT_D24X8:	{ return D3DFMT_D24X8;		}break;
    case PIXELFORMAT_D24S8:	{ return D3DFMT_D24S8;		}break;
    case PIXELFORMAT_NONE:	{ return D3DFMT_UNKNOWN;	}break;
    }

    MAID_ASSERT( true, "識別できないPIXELFORMATです %d" << eFormat );

    return D3DFMT_UNKNOWN;
  }

  /*!
    @fn PIXELFORMAT D3DFORMATtoPIXELFORMAT( D3DFORMAT eFormat )
   
    DirectGraphic9 で使用されているピクセルフォーマットを
    PIXELFORMAT に変換します
   
    @param	Format	[i ]	変換したい頂点フォーマット
   
    @return	変換後のフォーマット
  \n			判別できなかったら PIXELFORMAT_NONE
   */
  inline PIXELFORMAT D3DFORMATtoPIXELFORMAT( D3DFORMAT Format )
  {
    switch( eFormat )
    {
    case D3DFMT_P8:			{ return PIXELFORMAT_P08A08R08G08B08I;	}break;

    case D3DFMT_R5G6B5:   { return PIXELFORMAT_R05G06B05I;		}break;
    case D3DFMT_A4R4G4B4: { return PIXELFORMAT_A04R04G04B04I;		}break;
    case D3DFMT_X1R5G5B5: { return PIXELFORMAT_X01R05G05B05I;		}break;
    case D3DFMT_A1R5G5B5: { return PIXELFORMAT_A01R05G05B05I;		}break;

    case D3DFMT_R8G8B8:   { return PIXELFORMAT_R08G08B08I;		}break;

    case D3DFMT_A8R8G8B8:     { return PIXELFORMAT_A08R08G08B08I;		}break;
    case D3DFMT_X8R8G8B8:     { return PIXELFORMAT_X08R08G08B08I;		}break;
    case D3DFMT_A2R10G10B10:	{ return PIXELFORMAT_A02R10G10B10I;		}break;
    case D3DFMT_A2B10G10R10:	{ return PIXELFORMAT_A02B10G10R10I;		}break;
    case D3DFMT_A16B16G16R16:	{ return PIXELFORMAT_A16B16G16R16I;		}break;

    case D3DFMT_A16B16G16R16F:	{ return PIXELFORMAT_A16B16G16R16F;		}break;

    case D3DFMT_A32B32G32R32F:	{ return PIXELFORMAT_A32B32G32R32F;		}break;

    case D3DFMT_DXT1:		{ return PIXELFORMAT_DXT1;			}break;
    case D3DFMT_DXT2:		{ return PIXELFORMAT_DXT2;			}break;
    case D3DFMT_DXT3:		{ return PIXELFORMAT_DXT3;			}break;
    case D3DFMT_DXT4:		{ return PIXELFORMAT_DXT4;			}break;
    case D3DFMT_DXT5:		{ return PIXELFORMAT_DXT5;			}break;

    case D3DFMT_D16:		{ return PIXELFORMAT_D16;			}break;
    case D3DFMT_D24X8:		{ return PIXELFORMAT_D24X8;			}break;
    case D3DFMT_D24S8:		{ return PIXELFORMAT_D24S8;			}break;
    case D3DFMT_UNKNOWN:	{ return PIXELFORMAT_NONE;			}break;
    }

    MAID_ASSERT( true, "識別できないD3DFORMATです %d" << eFormat );

    return PIXELFORMAT_NONE;
  }

  inline D3DCMPFUNC CMPTYPEtoD3DCMPFUNC( VideoCard::CMPTYPE eType )
  {
    switch( eType )
    {
    case VideoCard::CMPTYPE_NEVER:       { return D3DCMP_NEVER;      }break;
    case VideoCard::CMPTYPE_LESS:        { return D3DCMP_LESS;       }break;
    case VideoCard::CMPTYPE_EQUAL:       { return D3DCMP_EQUAL;      }break;
    case VideoCard::CMPTYPE_LESSEQUAL:   { return D3DCMP_LESSEQUAL;	}break;
    case VideoCard::CMPTYPE_GREATER:     { return D3DCMP_GREATER;    }break;
    case VideoCard::CMPTYPE_NOTEQUAL:    { return D3DCMP_NOTEQUAL;   }break;
    case VideoCard::CMPTYPE_GREATEREQUAL:{ return D3DCMP_GREATEREQUAL;}break;
    case VideoCard::CMPTYPE_ALWAYS:      { return D3DCMP_ALWAYS;     }break;
    }

    MAID_ASSERT( true, "識別できないCMPTYPE " << eType );

    return D3DCMP_FORCE_DWORD;
  }

}

#endif
