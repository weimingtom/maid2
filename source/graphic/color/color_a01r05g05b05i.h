#ifndef maid2_graphic_color_color_a01r05g05b05i_h
#define maid2_graphic_color_color_a01r05g05b05i_h

/*!
    @file
    @brief	１ピクセルをあらわすクラス
\n          A01R05G05B05 integer フォーマット Ver.
 */
#include"../../config/define.h"
#include"../../config/typedef.h"

#include"bitconvertfunction.h"

namespace Maid
{
	/*!
      @class	COLOR_A08R08G08B08I color_a01r05g05b05i.h
	 */
  class COLOR_A01R05G05B05I
	{
	public:
		COLOR_A01R05G05B05I(){}	//!<	コンストラクタ
		COLOR_A01R05G05B05I( unt16 a, unt16 r, unt16 g, unt16 b ){ SetARGB( a, r, g, b ); }	//!<	コンストラクタ
		template <class COLOR>
		COLOR_A01R05G05B05I( const COLOR& Col ){ SetARGB( Col.GetA(), Col.GetR(), Col.GetG(), Col.GetB() ); }	//!<	コンストラクタ

		unt16 GetA() const	{ return BITCONVERT01Ito16I((m_ARGB&0x8000)>>15);	}	//!<	アルファ成分の取得
		unt16 GetR() const	{ return BITCONVERT05Ito16I((m_ARGB&0x7C00)>>10);	}	//!<	赤成分の取得
		unt16 GetG() const	{ return BITCONVERT05Ito16I((m_ARGB&0x03E0)>> 5);	}	//!<	緑成分の取得
		unt16 GetB() const	{ return BITCONVERT05Ito16I((m_ARGB&0x001F)<< 3);	}	//!<	青成分の取得

		operator unt16() const { return m_ARGB; }	//!<	一括変換

		void SetA( unt16 a )	{ m_ARGB = (m_ARGB&~0x8000)|(BITCONVERT16Ito01I(a)<<15);	}	//!<	アルファ成分の設定
		void SetR( unt16 r )	{ m_ARGB = (m_ARGB&~0x7C00)|(BITCONVERT16Ito05I(r)<<10);	}	//!<	赤成分の設定
		void SetG( unt16 g )	{ m_ARGB = (m_ARGB&~0x03E0)|(BITCONVERT16Ito05I(g)<< 5);	}	//!<	緑成分の設定
		void SetB( unt16 b )	{ m_ARGB = (m_ARGB&~0x001F)|(BITCONVERT16Ito05I(b)<< 0);	}	//!<	青成分の設定

		void SetARGB( unt16 a, unt16 r, unt16 g, unt16 b )	//!<	いっきに設定
		{
			SetA( a );
			SetR( r );
			SetG( g );
			SetB( b );
		}

		//!	違うピクセルフォーマットとのコピー
		template <class PIXEL>
		COLOR_A01R05G05B05I& operator = (const PIXEL &src)
		{
			SetARGB(src.GetA(), src.GetR(),src.GetG(),src.GetB());
			return *this;
		}

		//!	同じピクセルフォーマットとのコピー
		COLOR_A01R05G05B05I& operator = (const COLOR_A01R05G05B05I& src)
		{
			m_ARGB = src.m_ARGB;
			return *this;
		}

	private:
		unt16 m_ARGB;
	};
}

#endif
