#ifndef maid2_graphics_color_color_r08g08b08i_h
#define maid2_graphics_color_color_r08g08b08i_h

/*!
 	@file
 	@brief	１ピクセルをあらわすクラス郡
\n			RGB888 フォーマット Ver.
 */
#include"../../config/define.h"
#include"../../config/typedef.h"

#include"bitconvertfunction.h"

namespace Maid
{
	/*!
	 	@class	COLOR_R08G08B08I color_r08g08b08i.h
	 	@brief	アルファ無２４ビットピクセル
	 */
	class COLOR_R08G08B08I
	{
	public:
		COLOR_R08G08B08I(){}	//!<	コンストラクタ
		COLOR_R08G08B08I( unt16 r, unt16 g, unt16 b ) { SetRGB(r,g,b); }	//!<	コンストラクタ

    template <class COLOR>
		COLOR_R08G08B08I( const COLOR& Col ){  SetRGB( Col.GetR(), Col.GetG(), Col.GetB() );  }	//!<	コンストラクタ


		unt16 GetA()const	{ return 0xFFFF;		}	//!<	α成分の取得
		unt16 GetR()const	{ return BITCONVERT08Ito16I(m_Red);		}	//!<	赤成分の取得
		unt16 GetG()const	{ return BITCONVERT08Ito16I(m_Green);	}	//!<	緑成分の取得
		unt16 GetB()const	{ return BITCONVERT08Ito16I(m_Blue);	}	//!<	青成分の取得


		operator unt32()const{ return (m_Red<<16)|(m_Green<<8)|(m_Blue<<0); }	//!<	一括変換

		void SetA( unt16 a )	{ /*何もしない*/	}
		void SetR( unt16 r )	{ m_Red   = BITCONVERT16Ito08I(r);	}	//!<	赤成分の設定
		void SetG( unt16 g )	{ m_Green = BITCONVERT16Ito08I(g);	}	//!<	緑成分の設定
		void SetB( unt16 b )	{ m_Blue  = BITCONVERT16Ito08I(b);	}	//!<	青成分の設定

		void SetRGB( unt16 r, unt16 g, unt16 b )	//!<	いっきに設定
		{
			SetR( r );
			SetG( g );
			SetB( b );
		}

		void SetARGB( unt16 a, unt16 r, unt16 g, unt16 b )	//!<	いっきに設定
		{
			SetA( a );
			SetR( r );
			SetG( g );
			SetB( b );
		}

		//!	違うピクセルフォーマットとのコピー
		template <class PIXEL>
		COLOR_R08G08B08I& operator = (const PIXEL &src)
		{
			SetRGB(src.GetR(),src.GetG(),src.GetB());
			return *this;
		}
		//!	同じピクセルフォーマットとのコピー
		COLOR_R08G08B08I& operator = (const COLOR_R08G08B08I& src)
		{
			m_Blue = src.m_Blue;
			m_Green = src.m_Green;
			m_Red = src.m_Red;
			return *this;
		}
	private:
		unt08 m_Blue;
		unt08 m_Green;
		unt08 m_Red;
	};

}

#endif
