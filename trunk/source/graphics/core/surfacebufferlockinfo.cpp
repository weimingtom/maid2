
#include"surfacebufferlockinfo.h"
#include"../../auxiliary/debug/assert.h"


namespace Maid
{
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! サーフェスの先頭アドレスの取得
/*! 
    このアドレスが座標(0,0)のアドレスであるとは限りません（圧縮サーフェス等）
	 
    @return サーフェスの先頭アドレス
 */
void* SurfaceBufferLockInfo::GetAddress() const
{
  return m_pBits;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! サーフェスのピッチの取得
/*! 
    @return サーフェスのピッチ
 */
int SurfaceBufferLockInfo::GetPitch() const
{
  return m_Pitch;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! サーフェスの縦横幅の取得
/*! 
\n	SurfaceBuffer::GetSize() と同じです
 
    @return サーフェスの横幅
 */
const SIZE2DI&	SurfaceBufferLockInfo::GetSize() const
{
  return m_Size;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! サーフェスのピクセルフォーマットの取得
/*! 
    @return サーフェスのピクセルフォーマット
 */
PIXELFORMAT SurfaceBufferLockInfo::GetPixelFormat() const
{
  return m_Format;
}

//! カラールックアップテーブル（いわゆるパレット）の取得
/*! 
    @return パレットの先頭アドレス
 */
void* SurfaceBufferLockInfo::GetCLUT() const
{
  return m_pCLUT;
}

//! 各ラインの先頭アドレスの取得
/*! 
    @param	PosY	[i ]	取得したいライン( PosY<GetSize().h であること）
	 
    @return 指定したラインアドレス
 */
void* SurfaceBufferLockInfo::GetLinePtr( int PosY )const
{
  const int h = GetSize().h;
  if( m_IsReverse )
  {
    MAID_ASSERT(!(PosY<h),"指定した範囲を超えています "<< PosY <<":"<< h);
		return (unt08*)m_pBits + m_Pitch*(h-(PosY+1));
  }else
  {
		MAID_ASSERT(!(PosY<h),"指定した範囲を超えています" << PosY <<":"<< h);
		return (unt08*)m_pBits + m_Pitch*PosY; 
  }

  return NULL;
}

 


void* SurfaceBufferLockInfo::GetPixelPtr( const POINT2DI& pos ) const
{
	MAID_ASSERT( pos.x<0, "X座標が不正です " << pos.x );
	MAID_ASSERT( pos.y<0, "Y座標が不正です " << pos.y );

	unt08* p = (unt08*)GetLinePtr( pos.y );

	return p + pos.x * GetPixelBPP(GetPixelFormat())/8;
}

COLOR_A16B16G16R16I SurfaceBufferLockInfo::GetPixelI( const POINT2DI& pos )
{
	COLOR_A16B16G16R16I ret;

	void* p = GetPixelPtr(pos);

	switch( GetPixelFormat() )
	{
	case PIXELFORMAT_A08R08G08B08I:	{ ret = *(COLOR_A08R08G08B08I*)p;}break;
	case PIXELFORMAT_X08R08G08B08I:	{ ret = *(COLOR_X08R08G08B08I*)p;}break;
	case PIXELFORMAT_R08G08B08I:	{ ret = *(COLOR_R08G08B08I*  )p;}break;
	case PIXELFORMAT_A04R04G04B04I:	{ ret = *(COLOR_A04R04G04B04I*)p;}break;
	case PIXELFORMAT_R05G06B05I:	{ ret = *(COLOR_R05G06B05I*  )p;}break;
	case PIXELFORMAT_X01R05G05B05I:	{ ret = *(COLOR_X01R05G05B05I*)p;}break;
	case PIXELFORMAT_A01R05G05B05I:	{ ret = *(COLOR_A01R05G05B05I*)p;}break;

	case PIXELFORMAT_A02R10G10B10I:	{ ret = *(COLOR_A02R10G10B10I*)p;}break;
	case PIXELFORMAT_A02B10G10R10I:	{ ret = *(COLOR_A02B10G10R10I*)p;}break;
	case PIXELFORMAT_A16B16G16R16I:	{ ret = *(COLOR_A16B16G16R16I*)p;}break;

	case PIXELFORMAT_P08X08R08G08B08I:
	case PIXELFORMAT_P08A08R08G08B08I:
	default:
		{
			MAID_ASSERT( true, "未対応" );
		}break;

	}

	return ret;
}

void SurfaceBufferLockInfo::SetPixelI( const POINT2DI& pos, const COLOR_A16B16G16R16I& col )
{
	void* p = GetPixelPtr(pos);

	switch( GetPixelFormat() )
	{
	case PIXELFORMAT_A08R08G08B08I:	{ *(COLOR_A08R08G08B08I*)p = col; }break;
	case PIXELFORMAT_X08R08G08B08I:	{ *(COLOR_X08R08G08B08I*)p = col; }break;
	case PIXELFORMAT_R08G08B08I:	{ *(COLOR_R08G08B08I*)p    = col; }break;
	case PIXELFORMAT_A04R04G04B04I:	{ *(COLOR_A04R04G04B04I*)p = col; }break;
	case PIXELFORMAT_R05G06B05I:	{ *(COLOR_R05G06B05I*)p    = col; }break;
	case PIXELFORMAT_X01R05G05B05I:	{ *(COLOR_X01R05G05B05I*)p = col; }break;
	case PIXELFORMAT_A01R05G05B05I:	{ *(COLOR_A01R05G05B05I*)p = col; }break;

	case PIXELFORMAT_A02R10G10B10I:	{ *(COLOR_A02R10G10B10I*)p = col;}break;
	case PIXELFORMAT_A02B10G10R10I:	{ *(COLOR_A02B10G10R10I*)p = col;}break;
	case PIXELFORMAT_A16B16G16R16I:	{ *(COLOR_A16B16G16R16I*)p = col;}break;

	case PIXELFORMAT_P08X08R08G08B08I:
	case PIXELFORMAT_P08A08R08G08B08I:
	default:
		{
			MAID_ASSERT( true, "未対応" );
		}break;
	}
}
COLOR_A32B32G32R32F SurfaceBufferLockInfo::GetPixelF( const POINT2DI& pos )
{
	MAID_ASSERT( true, "未対応" );
  return COLOR_A32B32G32R32F();
}

void SurfaceBufferLockInfo::SetPixelF( const POINT2DI& pos, const COLOR_A32B32G32R32F& col )
{
	MAID_ASSERT( true, "未対応" );
}


void SurfaceBufferLockInfo::SetInfo( void* pBits, void* pCLUT, const SIZE2DI& Size, PIXELFORMAT fmt, int Pitch, bool IsReverse )
{
  m_pBits   = pBits;
  m_pCLUT   = pCLUT;
  m_Size    = Size;
  m_Format  = fmt;
  m_Pitch   = Pitch;
  m_IsReverse = IsReverse;
}

}

