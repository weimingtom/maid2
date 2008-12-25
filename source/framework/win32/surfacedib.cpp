#include"surfacedib.h"

#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/exception.h"

#include"../../auxiliary/string.h"

namespace Maid
{

/*!
 	マスクビットの設定
 
 	@param	Header	[ o]	設定先
 	@param	Alpha	[i ]	αビット
 	@param	Red		[i ]	赤ビット
 	@param	Green	[i ]	緑ビット
 	@param	Blue	[i ]	青ビット
 */
inline void SetMask( BITMAPV4HEADER& Header, DWORD Alpha, DWORD Red, DWORD Green, DWORD Blue )
{
	Header.bV4AlphaMask = Alpha;
	Header.bV4RedMask   = Red;
	Header.bV4GreenMask = Green;
	Header.bV4BlueMask  = Blue;
}

/*!
 	@class	SurfaceDIB surfacedib.h
 	@brief	描画平面クラス DIB Ver.
 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
SurfaceDIB::SurfaceDIB() : m_hDIB(NULL), m_pPlane(NULL)
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
SurfaceDIB::~SurfaceDIB()
{
	Destroy();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! サーフェスの横幅の取得
/*!
 	引数などは ISurface::GetSize() を参照すること
 */
SIZE2DI SurfaceDIB::GetSize() const
{
	MAID_ASSERT( m_hDIB==NULL, "まだ作成されていません" );
	return m_Size;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! サーフェスのピクセルフォーマットの取得
/*!
 	引数などは ISurface::GetPixelFormat() を参照すること
 */
PIXELFORMAT SurfaceDIB::GetPixelFormat() const
{
	MAID_ASSERT( m_hDIB==NULL, "まだ作成されていません" );
	return m_Format;
}


int   SurfaceDIB::GetPitch() const
{
	MAID_ASSERT( m_hDIB==NULL, "まだ作成されていません" );
	return m_Pitch;
}

bool  SurfaceDIB::IsEmpty() const
{
  return m_hDIB==NULL;
}



void SurfaceDIB::Create( const SIZE2DI& size, PIXELFORMAT fmt )
{
  MAID_ASSERT( fmt!=PIXELFORMAT_X08R08G08B08I, "PIXELFORMAT_X08R08G08B08I以外受け付けません" );

  Destroy();

	//	DIBの横幅は４の倍数でなければ作成できない

  const int pitch = (size.w*GetPixelBPP(fmt)/8+3)&~3;
  const int w = pitch / (GetPixelBPP(fmt)/8);

	BITMAPINFOHEADER dib;

	ZERO( &dib, sizeof(dib) );
	dib.biSize = sizeof(dib);
	dib.biWidth = w;
	dib.biHeight = -(int)size.h;
	dib.biPlanes = 1;
	dib.biBitCount = GetPixelBPP(fmt);

/*
	BITMAPV4HEADER Header;

	ZeroMemory(&Header, sizeof(Header));
	Header.bV4Size = sizeof(BITMAPV4HEADER);
	Header.bV4Width = w;
	Header.bV4Height = -short(Height);
	Header.bV4Planes = 1;
	Header.bV4BitCount = GetPixelBPP(eFormat);
	Header.bV4V4Compression = BI_BITFIELDS;
	Header.bV4SizeImage = 0;
	Header.bV4XPelsPerMeter = 0;
	Header.bV4YPelsPerMeter = 0;
	Header.bV4ClrUsed = 0;
	Header.bV4ClrImportant = 0;

	//	ピクセルフォーマットにあったマスクを生成する
	switch( eFormat )
	{
	case PIXELFORMAT_ARGB8888:  { SetMask( Header, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF ); }break;
	case PIXELFORMAT_XRGB8888:  { SetMask( Header, 0x00000000, 0x00FF0000, 0x0000FF00, 0x000000FF ); }break;
	case PIXELFORMAT_RGB888:	{ SetMask( Header, 0x00000000, 0x00FF0000, 0x0000FF00, 0x000000FF ); }break;
	case PIXELFORMAT_ARGB4444:	{ SetMask( Header, 0x0000F000, 0x00000F00, 0x000000F0, 0x0000000F ); }break;
	case PIXELFORMAT_ARGB1555:	{ SetMask( Header, 0x00008000, 0x00007C00, 0x000003E0, 0x0000001F ); }break;
	case PIXELFORMAT_XRGB1555:	{ SetMask( Header, 0x00000000, 0x00007C00, 0x000003E0, 0x0000001F ); }break;
	case PIXELFORMAT_RGB565:	{ SetMask( Header, 0x00000000, 0x0000F800, 0x000007E0, 0x0000001F ); }break;
		//	パレットは未サポート
	}
*/
	HDC hDC = ::CreateCompatibleDC(NULL);

	if( hDC==NULL ) { MAID_WARNING( MAIDTEXT("hDC==NULL") ); return ; }
	m_hDIB = CreateDIBSection( hDC, (BITMAPINFO *)&dib, DIB_RGB_COLORS, (void **)&m_pPlane, NULL, 0 );
	::DeleteDC(hDC);
	if( m_hDIB==NULL ) { MAID_WARNING( MAIDTEXT("DIB確保に失敗") );  return ; }


	m_Size   = size;
	m_Format = fmt;
	m_Pitch  = pitch;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! サーフェスの破棄
/*!
 	引数などは ISurfaceBuffer::Destroy() を参照すること
 */
void SurfaceDIB::Destroy()
{
  if( m_hDIB==NULL ) { return ; }

	::DeleteObject(m_hDIB);
	m_hDIB = NULL;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! HBITMAP の取得
/*!
 	@return HBITMAP
 */
HBITMAP SurfaceDIB::GetHBITMAP() const
{
	MAID_ASSERT( m_hDIB==NULL, "まだ作成されていません" );
	return m_hDIB;
}


void* SurfaceDIB::GetPlanePTR()  { return m_pPlane; }
void* SurfaceDIB::GetCLUTPTR()  { return NULL; }



}

