#ifndef maid2_graphics_color_colorconvert_h
#define maid2_graphics_color_colorconvert_h

#include"../../config/define.h"
#include"../../config/typedef.h"
#include"bitconvertfunction.h"

#include"color_a32b32g32r32f.h"
#include"color_a16b16g16r16i.h"

namespace Maid
{
  inline void PIXELCONVERT( const COLOR_A32B32G32R32F& src, COLOR_A16B16G16R16I& dst )
  {
    dst.SetA( BITCONVERT32Fto16I( src.GetA() ) );
    dst.SetB( BITCONVERT32Fto16I( src.GetB() ) );
    dst.SetG( BITCONVERT32Fto16I( src.GetG() ) );
    dst.SetR( BITCONVERT32Fto16I( src.GetR() ) );
  }

  inline void PIXELCONVERT( const COLOR_A16B16G16R16I& src, COLOR_A32B32G32R32F& dst )
  {
    dst.SetA( BITCONVERT16Ito32F( src.GetA() ) );
    dst.SetB( BITCONVERT16Ito32F( src.GetB() ) );
    dst.SetG( BITCONVERT16Ito32F( src.GetG() ) );
    dst.SetR( BITCONVERT16Ito32F( src.GetR() ) );
  }

  inline COLOR_A32B32G32R32F PIXELCONVERTtoF( const COLOR_A16B16G16R16I& src )
  {
    return COLOR_A32B32G32R32F( 
      BITCONVERT16Ito32F( src.GetA() ),
      BITCONVERT16Ito32F( src.GetB() ),
      BITCONVERT16Ito32F( src.GetG() ),
      BITCONVERT16Ito32F( src.GetR() )
      );
  }

  inline COLOR_A16B16G16R16I PIXELCONVERTtoI( const COLOR_A32B32G32R32F& src )
  {
    return COLOR_A16B16G16R16I( 
      BITCONVERT32Fto16I( src.GetA() ),
      BITCONVERT32Fto16I( src.GetB() ),
      BITCONVERT32Fto16I( src.GetG() ),
      BITCONVERT32Fto16I( src.GetR() )
      );
  }
}


#endif
