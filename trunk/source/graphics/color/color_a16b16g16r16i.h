#ifndef maid2_graphics_color_color_a16b16g16r16i_h
#define maid2_graphics_color_color_a16b16g16r16i_h

/*!
 	  @file
 	  @brief	１ピクセルをあらわすクラス
\n          A16B16G16R16 フォーマット Ver.
 */
#include"../../config/define.h"
#include"../../config/typedef.h"

#include"bitconvertfunction.h"

namespace Maid
{
	/*!
      @class	COLOR_A16B16G16R16I color_a16b16g16r16i.h
	 */
	class COLOR_A16B16G16R16I
	{
	public:
		COLOR_A16B16G16R16I(){}	//!<	コンストラクタ
		COLOR_A16B16G16R16I( unt16 a, unt16 b, unt16 g, unt16 r ){ SetARGB( a, r, g, b ); }	//!<	コンストラクタ
		template <class COLOR>
		COLOR_A16B16G16R16I( const COLOR& Col ){  SetARGB( Col.GetA(), Col.GetR(), Col.GetG(), Col.GetB() );  }	//!<	コンストラクタ


		unt16 GetA() const	{ return m_Alpha;	}	//!<	アルファ成分の取得
		unt16 GetR() const	{ return m_Red;		}	//!<	赤成分の取得
		unt16 GetG() const	{ return m_Green;	}	//!<	緑成分の取得
		unt16 GetB() const	{ return m_Blue;	}	//!<	青成分の取得

		operator unt64() const { return *((unt64*)&m_Blue); }	//!<	一括変換

		void SetA( unt16 a )	{ m_Alpha = a;	}	//!<	アルファ成分の設定
		void SetR( unt16 r )	{ m_Red   = r;	}	//!<	赤成分の設定
		void SetG( unt16 g )	{ m_Green = g;	}	//!<	緑成分の設定
		void SetB( unt16 b )	{ m_Blue  = b;	}	//!<	青成分の設定

		void SetARGB( unt16 a, unt16 r, unt16 g, unt16 b )	//!<	いっきに設定
		{
			SetA( a );
			SetR( r );
			SetG( g );
			SetB( b );
		}

		//!	違うピクセルフォーマットとのコピー
		template <class PIXEL>
		COLOR_A16B16G16R16I& operator = (const PIXEL &src)
		{
			SetARGB(src.GetA(),src.GetR(),src.GetG(),src.GetB());
			return *this;
		}

	private:
		unt16 m_Red;
		unt16 m_Green;
		unt16 m_Blue;
		unt16 m_Alpha;
	};
}

#endif
