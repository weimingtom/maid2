/*!
    @file
    @brief	ピクセルシェーダーバッファ D3D9 Ver.
 */

#ifndef graphic_core_driver_win32_direct3d09_pixelshaderbufferd3d09_h
#define graphic_core_driver_win32_direct3d09_pixelshaderbufferd3d09_h



#include"../../../../../config/define.h"
#include"../../../../../config/typedef.h"

#include"../../ipixelshaderbuffer.h"

#include"d3d09include.h"


namespace Maid
{
	class PixelShaderBufferD3D09 : public IPixelShaderBuffer
	{
	public:
		virtual void Setup( const SPD3D09DEVICE& pDevice )=0;
	};

	class PixelShaderBufferD3D09HARD : public PixelShaderBufferD3D09
	{
	public:
    PixelShaderBufferD3D09HARD( IDirect3DPixelShader9* p );
		virtual void Setup( const SPD3D09DEVICE& pDevice );
    SPD3D09PIXELSHADERBUFFER  pBuffer;
	};

	class PixelShaderBufferD3D09_NORMAL : public PixelShaderBufferD3D09
	{
	public:
    virtual void Setup( const SPD3D09DEVICE& pDevice );

  private:
	};

}

#endif