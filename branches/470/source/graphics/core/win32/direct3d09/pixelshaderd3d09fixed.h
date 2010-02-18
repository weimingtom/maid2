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
    PS_FIXED( int no );
    virtual void Setup( const SPD3D09DEVICE& pDevice );

  private:
    const int m_No;
	};

}}

#endif