﻿#ifndef maid2_graphics_color_color_x01r05g05b05i_h
#define maid2_graphics_color_color_x01r05g05b05i_h

/*!
 	@file
 	@brief	１ピクセルをあらわすクラス郡
\n			XRGB1555 フォーマット Ver.
*/

#include"../../config/define.h"
#include"../../config/typedef.h"

#include"bitconvertfunction.h"

namespace Maid
{
	/*!
	 	@class	COLOR_X01R05G05B05I color_x01r05g05b05i.h
	 */
	class COLOR_X01R05G05B05I
	{
	public:
		COLOR_X01R05G05B05I(){}	//!<	コンストラクタ
		COLOR_X01R05G05B05I( unt16 r, unt16 g, unt16 b ){ SetRGB( r, g, b ); }	//!<	コンストラクタ
		template <class COLOR>
		COLOR_X01R05G05B05I( const COLOR& Col ){  SetRGB( Col.GetR(), Col.GetG(), Col.GetB() );  }	//!<	コンストラクタ


		unt16 GetA() const	{ return 0xFFFF;		}	//!<	アルファ成分の取得
		unt16 GetR() const	{ return BITCONVERT05Ito16I((m_XRGB&0x7C00)>>10);	}	//!<	赤成分の取得
		unt16 GetG() const	{ return BITCONVERT05Ito16I((m_XRGB&0x03E0)>> 5);	}	//!<	緑成分の取得
		unt16 GetB() const	{ return BITCONVERT05Ito16I((m_XRGB&0x001F)<< 3);	}	//!<	青成分の取得

		operator unt16() const { return m_XRGB; }	//!<	一括変換

		void SetA( unt16 a )	{ /*何もしない*/	}
		void SetR( unt16 r )	{ m_XRGB = (m_XRGB&~0x7C00)|(BITCONVERT16Ito05I(r)<<10);	}	//!<	赤成分の設定
		void SetG( unt16 g )	{ m_XRGB = (m_XRGB&~0x03E0)|(BITCONVERT16Ito05I(g)<< 5);	}	//!<	緑成分の設定
		void SetB( unt16 b )	{ m_XRGB = (m_XRGB&~0x001F)|(BITCONVERT16Ito05I(b)<< 0);	}	//!<	青成分の設定

		void SetRGB( unt16 r, unt16 g, unt16 b )	//!<	いっきに設定
		{
      m_XRGB = ~0;
			SetR( r );
			SetG( g );
			SetB( b );
		}

		void SetARGB( unt16 a, unt16 r, unt16 g, unt16 b )	//!<	いっきに設定
		{
      m_XRGB = ~0;
			SetA( a );
			SetR( r );
			SetG( g );
			SetB( b );
		}

		//!	違うピクセルフォーマットとのコピー
		template <class PIXEL>
		COLOR_X01R05G05B05I& operator = (const PIXEL &src)
		{
			SetRGB(src.GetR(),src.GetG(),src.GetB());
			return *this;
		}

		//!	同じピクセルフォーマットとのコピー
		COLOR_X01R05G05B05I& operator = (const COLOR_X01R05G05B05I& src)
		{
			m_XRGB = src.m_XRGB;
			return *this;
		}

	private:
		unt16 m_XRGB;
	};


}

#endif
