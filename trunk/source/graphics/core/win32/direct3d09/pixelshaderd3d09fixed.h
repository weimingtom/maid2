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




  
	class PS_FIXED : public PixelShaderD3D09
	{
	public:
    PS_FIXED( int no ):m_No(no){}

    virtual void Setup( const SPD3D09DEVICE& pDevice )
		{
      switch( m_No )
      {
      case 0:
        { //	頂点色をそのまま流す
			    pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_SELECTARG1 );
			    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
			    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
			    pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,	D3DTOP_DISABLE );

			    pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,	D3DTOP_SELECTARG1 );
			    pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
			    pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			    pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,	D3DTOP_DISABLE );
        }break;

      case 10:
        {//	テクスチャと頂点色の合成
			    pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
			    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
			    pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
			    pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,	D3DTOP_DISABLE );

			    pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
			    pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
			    pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
			    pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,	D3DTOP_DISABLE );
        }break;
      }
		}

  private:
    const int m_No;
	};

}}

#endif