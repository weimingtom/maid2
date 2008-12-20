#ifndef graphic_core_driver_win32_direct3d09_pixelshaderd3dfixed_h
#define graphic_core_driver_win32_direct3d09_pixelshaderd3dfixed_h

#include"../../../../config/define.h"
#include"common.h"
#include"pixelshaderd3d09.h"

namespace Maid { namespace Graphics {

	class PS_SHADERCODE : public PixelShaderD3D09
  {
	public:
    PS_SHADERCODE( IDirect3DPixelShader9* p ):pPixelShader(p){}
    virtual void Setup( const SPD3D09DEVICE& pDevice )
    {
			pDevice->SetPixelShader( pPixelShader.get() );
    }

    SPD3D09PIXELSHADER  pPixelShader;
  };




  //	頂点色をそのまま流す
	class PS_001 : public PixelShaderD3D09
	{
	public:
    virtual void Setup( const SPD3D09DEVICE& pDevice )
		{
			pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_SELECTARG1 );
			pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
			pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
			pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,	D3DTOP_DISABLE );

			pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,	D3DTOP_SELECTARG1 );
			pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
			pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,	D3DTOP_DISABLE );
		}
	};

	/////////////////////////////////////////////

	//	テクスチャと頂点色の合成
	class PS_002 : public PixelShaderD3D09
	{
	public:
    virtual void Setup( const SPD3D09DEVICE& pDevice )
		{
			pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
			pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
			pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,	D3DTOP_DISABLE );

			pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
			pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,	D3DTOP_DISABLE );
  	}
	};
}}

#endif