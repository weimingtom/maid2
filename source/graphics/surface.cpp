#include"surface.h"
#include"../auxiliary/string.h"
#include"../auxiliary/debug/warning.h"


namespace Maid
{

Surface::Surface()
:m_Size(0,0),m_PixelFormat(PIXELFORMAT_NONE),m_Pitch(0),m_IsCreated(false)
{

}


void Surface::Create( const SIZE2DI& size, PIXELFORMAT fmt )
{
  const int pitch = size.w * GetPixelBPP(fmt) / 8;

  Create( size, fmt, pitch );
}

void Surface::Create( const SIZE2DI& size, PIXELFORMAT fmt, int Pitch )
{
  Destroy();

  {
    const int bpp = GetPixelBPP(fmt);
    m_Plane.resize( Pitch * size.h );
  }
  {
    const int bpp = GetCLUTBPP(fmt);
    const int len = GetCLUTIndexCount(fmt);
    m_CLUT.resize( bpp * len /8 );
  }

  m_Size = size;
  m_PixelFormat = fmt;
  m_Pitch = Pitch;
  m_IsCreated = true;
}

void Surface::Destroy()
{
  if( IsEmpty() ) { return ; }

  m_Plane.clear();
  m_CLUT.clear();
  m_Size = SIZE2DI(0,0);
  m_PixelFormat = PIXELFORMAT_NONE;
  m_Pitch = 0;
}

bool Surface::IsEmpty() const
{
  return !m_IsCreated;
}

const SIZE2DI& Surface::GetSize() const
{
  MAID_ASSERT( IsEmpty(), "まだ作成されてません" );

  return m_Size;
}

PIXELFORMAT Surface::GetPixelFormat() const
{
  MAID_ASSERT( IsEmpty(), "まだ作成されてません" );
  return m_PixelFormat;
}


int   Surface::GetPitch() const
{
  MAID_ASSERT( IsEmpty(), "まだ作成されてません" );
  return m_Pitch;
}






void* Surface::_GetPlanePTR()
{
  MAID_ASSERT( IsEmpty(), "まだ作成されてません" );
  return &(m_Plane[0]); 
}

void* Surface::_GetCLUTPTR()
{
  MAID_ASSERT( IsEmpty(), "まだ作成されてません" );
  return &(m_CLUT[0]); 
}

void* Surface::_GetLinePTR( int y )
{
  MAID_ASSERT( IsEmpty(), "まだ作成されてません" );
  if( IsEmpty() ) { return NULL; }
  MAID_ASSERT( GetSize().h<=y, MAIDTEXT("範囲外です ") << y );

  unt08* p = (unt08*)GetPlanePTR();
  const int pos = GetPitch() * y;

  return p + pos;
}

void* Surface::_GetPixelPTR( const POINT2DI& pos )
{
  MAID_ASSERT( IsEmpty(), "まだ作成されてません" );
  if( IsEmpty() ) { return NULL; }

  unt08* p = (unt08*)GetLinePTR(pos.y);

  const int bpp = GetPixelBPP(GetPixelFormat());

  p += bpp * pos.x / 8;

  return p;
}


void* Surface::_GetColorPTR( int index )
{
  MAID_ASSERT( IsEmpty(), "まだ作成されてません" );
  MAID_ASSERT( GetCLUTIndexCount(GetPixelFormat())<=index, MAIDTEXT("範囲外です ") << index );
  if( IsEmpty() ) { return NULL; }

  const int bpp = GetCLUTBPP(GetPixelFormat());
  unt08* p = (unt08*)GetCLUTPTR();

  return p + (bpp*index/8);
}




COLOR_R32G32B32A32F Surface::GetPixel( const POINT2DI& pos ) const
{
  MAID_ASSERT( IsEmpty(), "まだ作成されてません" );
  if( IsEmpty() ) { return COLOR_R32G32B32A32F(0,0,0,0); }

  const void* p = GetPixelPTR( pos );

  const PIXELFORMAT fmt = GetPixelFormat();

  switch( fmt )
  {
  case PIXELFORMAT_R05G06B05I:    { return PIXELCONVERTtoF( *(COLOR_R05G06B05I*) p); }break;
  case PIXELFORMAT_X01R05G05B05I: { return PIXELCONVERTtoF( *(COLOR_X01R05G05B05I*) p); }break;
  case PIXELFORMAT_A01R05G05B05I: { return PIXELCONVERTtoF( *(COLOR_A01R05G05B05I*) p); }break;
  case PIXELFORMAT_A01B05G05R05I: { return PIXELCONVERTtoF( *(COLOR_A01B05G05R05I*) p); }break;
  case PIXELFORMAT_A04R04G04B04I: { return PIXELCONVERTtoF( *(COLOR_A04R04G04B04I*) p); }break;
  case PIXELFORMAT_R08G08B08I:    { return PIXELCONVERTtoF( *(COLOR_R08G08B08I*) p); }break;

  case PIXELFORMAT_A08R08G08B08I: { return PIXELCONVERTtoF( *(COLOR_A08R08G08B08I*) p); }break;
  case PIXELFORMAT_X08R08G08B08I: { return PIXELCONVERTtoF( *(COLOR_X08R08G08B08I*) p); }break;
  case PIXELFORMAT_A02R10G10B10I: { return PIXELCONVERTtoF( *(COLOR_A02R10G10B10I*) p); }break;
  case PIXELFORMAT_A02B10G10R10I: { return PIXELCONVERTtoF( *(COLOR_A02B10G10R10I*) p); }break;
  case PIXELFORMAT_A16B16G16R16I: { return PIXELCONVERTtoF( *(COLOR_A16B16G16R16I*) p); }break;

  case PIXELFORMAT_A32B32G32R32F: { return *(COLOR_A32B32G32R32F*)p; }break;

  default: { MAID_WARNING( MAIDTEXT("未サポート ") << fmt ); }break;
  }

  return COLOR_R32G32B32A32F(0,0,0,0);
}

void Surface::SetPixel( const POINT2DI& pos, const COLOR_R32G32B32A32F& col )
{
  MAID_ASSERT( IsEmpty(), "まだ作成されてません" );
  if( IsEmpty() ) { return ; }

  void* p = GetPixelPTR( pos );
  const PIXELFORMAT fmt = GetPixelFormat();

  switch( fmt )
  {
  case PIXELFORMAT_R05G06B05I:    { *(COLOR_R05G06B05I*)p = PIXELCONVERTtoI(col) ; }break;
  case PIXELFORMAT_X01R05G05B05I: { *(COLOR_X01R05G05B05I*)p = PIXELCONVERTtoI( col ); }break;
  case PIXELFORMAT_A01R05G05B05I: { *(COLOR_A01R05G05B05I*)p = PIXELCONVERTtoI( col ); }break;
  case PIXELFORMAT_A01B05G05R05I: { *(COLOR_A01B05G05R05I*)p = PIXELCONVERTtoI( col ); }break;
  case PIXELFORMAT_A04R04G04B04I: { *(COLOR_A04R04G04B04I*)p = PIXELCONVERTtoI( col ); }break;
  case PIXELFORMAT_R08G08B08I:    { *(COLOR_R08G08B08I*)p = PIXELCONVERTtoI( col ); }break;

  case PIXELFORMAT_A08R08G08B08I:    { *(COLOR_A08R08G08B08I*)p = PIXELCONVERTtoI( col ); }break;
  case PIXELFORMAT_X08R08G08B08I:    { *(COLOR_X08R08G08B08I*)p = PIXELCONVERTtoI( col ); }break;
  case PIXELFORMAT_A02R10G10B10I:    { *(COLOR_A02R10G10B10I*)p = PIXELCONVERTtoI( col ); }break;
  case PIXELFORMAT_A02B10G10R10I:    { *(COLOR_A02B10G10R10I*)p = PIXELCONVERTtoI( col ); }break;
  case PIXELFORMAT_A16B16G16R16I:    { *(COLOR_A16B16G16R16I*)p = PIXELCONVERTtoI( col ); }break;

  case PIXELFORMAT_A32B32G32R32F:    { *(COLOR_A32B32G32R32F*)p = col; }break;

  default: { MAID_WARNING( MAIDTEXT("未サポート ") << fmt ); }break;
  }
}


}


