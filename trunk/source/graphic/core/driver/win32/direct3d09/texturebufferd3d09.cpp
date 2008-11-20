#include"texturebufferd3d09.h"

namespace Maid
{

TextureBufferD3D09::TextureBufferD3D09( IDirect3DTexture9* p )
{
	pTexture.reset( p );

	const int count = pTexture->GetLevelCount();

	MAID_ASSERT( count==0, "サーフェスがありません" );

	for( int i=0; i<count; ++i )
	{
		IDirect3DSurface9* pp;
		pTexture->GetSurfaceLevel( i, &pp );

		Surface.push_back( SurfaceBufferD3D09( pp ) );
	}

	Size	= Surface[0].GetSize();
	Format= Surface[0].GetPixelFormat();
}


SIZE2DI TextureBufferD3D09::GetSize() const
{
	MAID_ASSERT( pTexture.get()==NULL, "初期化されていません" );
	return Size;
}

PIXELFORMAT TextureBufferD3D09::GetPixelFormat() const
{
	MAID_ASSERT( pTexture.get()==NULL, "初期化されていません" );
	return Format;
}

int TextureBufferD3D09::GetSurfaceLevel() const
{
	MAID_ASSERT( pTexture.get()==NULL, "初期化されていません" );
	return (int)Surface.size();
}

ISurfaceBuffer& TextureBufferD3D09::GetSurface( int Level )
{
	MAID_ASSERT( pTexture.get()==NULL, "初期化されていません" );
	MAID_ASSERT( !(Level<(int)Surface.size()), "範囲を超えています" );

	return Surface[Level];
}

}
