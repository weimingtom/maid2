#ifndef maid2_graphics_color_color_a02r10g10b10i_h
#define maid2_graphics_color_color_a02r10g10b10i_h

/*!
 	  @file
 	  @brief	１ピクセルをあらわすクラス
\n          A02R10G10B10 フォーマット Ver.
 */
#include"../../config/define.h"
#include"../../config/typedef.h"

#include"bitconvertfunction.h"

namespace Maid
{
	/*!
      @class	COLOR_A02R10G10B10I color_a02r10g10b10i.h
	 */
	class COLOR_A02R10G10B10I
	{
	public:
		COLOR_A02R10G10B10I(){}	//!<	コンストラクタ
		COLOR_A02R10G10B10I( unt16 a, unt16 r, unt16 g, unt16 b ){ SetARGB( a, r, g, b ); }	//!<	コンストラクタ
		template <class COLOR>
		COLOR_A02R10G10B10I( const COLOR& Col ){  SetARGB( Col.GetA(), Col.GetR(), Col.GetG(), Col.GetB() );  }	//!<	コンストラクタ


		unt16 GetA() const	{ return BITCONVERT02Ito16I((m_ARGB&0xC0000000)>>30);	}	//!<	アルファ成分の取得
		unt16 GetR() const	{ return BITCONVERT10Ito16I((m_ARGB&0x3FF00000)>>20);	}	//!<	赤成分の取得
		unt16 GetG() const	{ return BITCONVERT10Ito16I((m_ARGB&0x000FFC00)>>10);	}	//!<	緑成分の取得
		unt16 GetB() const	{ return BITCONVERT10Ito16I((m_ARGB&0x000003FF)>> 0);	}	//!<	青成分の取得

		operator unt32() const { return m_ARGB; }	//!<	一括変換

		void SetA( unt16 a )	{ m_ARGB = (m_ARGB&~0xC0000000)|(BITCONVERT16Ito02I(a)<<30);	}	//!<	アルファ成分の設定
		void SetR( unt16 r )	{ m_ARGB = (m_ARGB&~0x3FF00000)|(BITCONVERT16Ito10I(r)<<20);	}	//!<	赤成分の設定
		void SetG( unt16 g )	{ m_ARGB = (m_ARGB&~0x000FFC00)|(BITCONVERT16Ito10I(g)<<10);	}	//!<	緑成分の設定
		void SetB( unt16 b )	{ m_ARGB = (m_ARGB&~0x000003FF)|(BITCONVERT16Ito10I(b)<< 0);	}	//!<	青成分の設定

		void SetARGB( unt16 a, unt16 r, unt16 g, unt16 b )	//!<	いっきに設定
		{
			SetA( a );
			SetR( r );
			SetG( g );
			SetB( b );
		}

		//!	違うピクセルフォーマットとのコピー
		template <class PIXEL>
		COLOR_A02R10G10B10I& operator = (const PIXEL &src)
		{
			SetARGB(src.GetA(),src.GetR(),src.GetG(),src.GetB());
			return *this;
		}

		//!	同じピクセルフォーマットとのコピー
		COLOR_A02R10G10B10I& operator = (const COLOR_A02R10G10B10I& src)
		{
			m_ARGB = src.m_ARGB;
			return *this;
		}
	private:
		unt32 m_ARGB;
	};
}

#endif
