/*!
 	@file
 	@brief	テクスチャバッファオンメモリ Ver.
 */

#ifndef maid2_graphic_core_texturebuffermemory_h
#define maid2_graphic_core_texturebuffermemory_h

#include"../../../config/define.h"

#include<vector>

#include"itexturebuffer.h"
#include"surfacebuffermemory.h"



namespace Maid
{
	class TextureBufferMemory : public ITextureBuffer
	{
	public:
		TextureBufferMemory();
		~TextureBufferMemory();

		virtual SIZE2DI GetSize() const;
		virtual PIXELFORMAT GetPixelFormat() const;
		virtual int GetSurfaceLevel() const;
		virtual ISurfaceBuffer& GetSurface( int Level );

		void Create( const SIZE2DI& size, PIXELFORMAT fmt );
		void Create( const SIZE2DI& size, PIXELFORMAT fmt, int level );
		void Destroy();

		SurfaceBufferMemory& GetSurfaceEx( int Level );
		void SetReverseSurface( bool IsReverse );

	private:
		std::vector<SurfaceBufferMemory>	m_Surface;
	};
}

#endif

