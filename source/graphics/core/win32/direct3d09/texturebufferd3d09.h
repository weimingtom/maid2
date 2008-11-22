/*!
 	@file
 	@brief	テクスチャバッファ D3D9 Ver
 */

#ifndef graphic_core_driver_win32_direct3d09_texturebufferd3d09_h
#define graphic_core_driver_win32_direct3d09_texturebufferd3d09_h

#include"../../../../../config/define.h"
#include"../../../../../config/typedef.h"

#include<vector>

#include"../../itexturebuffer.h"

#include"d3d09include.h"
#include"surfacebufferd3d09.h"


namespace Maid
{
	/*!
	 	@class	TextureBufferD3D09 itexturebufferd3d09.h
	 	@brief	テクスチャバッファ D3D9 Ver
	 */
	class TextureBufferD3D09 : public ITextureBuffer
	{
	public:
		TextureBufferD3D09( IDirect3DTexture9* p );

		virtual SIZE2DI GetSize() const;
		virtual PIXELFORMAT GetPixelFormat() const;
		virtual int GetSurfaceLevel() const;
		virtual ISurfaceBuffer& GetSurface( int Level );

		SPD3D09TEXTUREBUFFER					      pTexture;
		std::vector<SurfaceBufferD3D09>	Surface;

		SIZE2DI		Size;
		PIXELFORMAT	Format;
	};
}

#endif

