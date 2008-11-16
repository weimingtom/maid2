
#include"texturebuffermemory.h"



namespace Maid
{
	/*!
	 	@class	TextureBufferMemory texturebuffermemory.h
	 	@brief	テクスチャバッファ オンメモリ Ver.
	 */

TextureBufferMemory::TextureBufferMemory()
{

}

TextureBufferMemory::~TextureBufferMemory()
{
	Destroy();
}

SIZE2DI TextureBufferMemory::GetSize() const
{
	MAID_ASSERT( m_Surface.empty(), "まだ作成されていません" );
	return m_Surface[0].GetSize();
}

PIXELFORMAT TextureBufferMemory::GetPixelFormat() const
{
	MAID_ASSERT( m_Surface.empty(), "まだ作成されていません" );
	return m_Surface[0].GetPixelFormat();
}

int TextureBufferMemory::GetSurfaceLevel() const
{
	MAID_ASSERT( m_Surface.empty(), "まだ作成されていません" );
	return (int)m_Surface.size();
}

SurfaceBuffer& TextureBufferMemory::GetSurface( int Level )
{
	return GetSurfaceEx( Level );
}


void TextureBufferMemory::Create( const SIZE2DI& size, PIXELFORMAT fmt )
{
  Create( size, fmt, 1 );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! テクスチャの作成
/*!
 	@param	size	[i ]	サーフェスの大きさ
 	@param	fmt		[i ]	ピクセルフォーマット
 	@param	level	[i ]	作るミップマップの枚数(デフォルトで1)
 */
void TextureBufferMemory::Create( const SIZE2DI& size, PIXELFORMAT fmt, int level )
{
	MAID_ASSERT( level==0, "最低でも1にしてください" );
	Destroy();

	m_Surface.resize(level);

	int w = size.w;
	int h = size.h;

	for( int i=0; i<level; ++i )
	{
		m_Surface[i].Create( SIZE2DI(w,h), fmt );

		w /= 2;
		h /= 2;
	}
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! サーフェスの破棄
/*!
 */
void TextureBufferMemory::Destroy()
{
	m_Surface.clear();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 内部サーフェスの取得
/*!
 */
SurfaceBufferMemory& TextureBufferMemory::GetSurfaceEx( int Level )
{
	MAID_ASSERT( m_Surface.empty(), "まだ作成されていません" );
	MAID_ASSERT( !(Level<(int)m_Surface.size()), "範囲を超えています" );

	return m_Surface[Level];
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! このサーフェスは反転してる設定を行う
/*!
	@param	IsReverse	[i ]	反転してるなら true
 */
void TextureBufferMemory::SetReverseSurface( bool IsReverse )
{
	MAID_ASSERT( m_Surface.empty(), "まだ作成されていません" );

	for( int i=0; i<(int)m_Surface.size(); ++i )
	{
		m_Surface[i].SetReverseSurface(IsReverse);

	}
}


}


