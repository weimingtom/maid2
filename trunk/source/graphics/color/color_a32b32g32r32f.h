#ifndef maid2_graphics_color_color_a32b32g32r32f_h
#define maid2_graphics_color_color_a32b32g32r32f_h

/*!
 	@file
 	@brief	１ピクセルをあらわすクラス
\n			ARGB それぞれ float フォーマット Ver.
 */
#include"../../config/define.h"
#include"../../config/typedef.h"

namespace Maid
{
	/*!
      \class	COLOR_A32B32G32R32F color_a32b32g32r32f.h
      \brief	ARGB それぞれ float32ビットカラー
  \n          カラーが整数のクラスとは互換性がありません
	 */
	class COLOR_A32B32G32R32F
	{
	public:
		COLOR_A32B32G32R32F(){}	//!<	コンストラクタ
		COLOR_A32B32G32R32F( float32 a, float32 b, float32 g, float32 r )
			: m_Alpha(a), m_Red(r), m_Green(g), m_Blue(b) {}	//!<	コンストラクタ

		template <class COLOR>
		COLOR_A32B32G32R32F( const COLOR& Col ){  SetARGB( Col.GetA(),Col.GetR(),Col.GetG(),Col.GetB() );  }	//!<	コンストラクタ

		float32 GetA() const	{ return m_Alpha;	}	//!<	アルファ成分の取得
		float32 GetR() const	{ return m_Red;		}	//!<	赤成分の取得
		float32 GetG() const	{ return m_Green;	}	//!<	緑成分の取得
		float32 GetB() const	{ return m_Blue;	}	//!<	青成分の取得

		void SetA( float32 a )	{ m_Alpha = a;	}	//!<	アルファ成分の設定
		void SetR( float32 r )	{ m_Red   = r;	}	//!<	赤成分の設定
		void SetG( float32 g )	{ m_Green = g;	}	//!<	緑成分の設定
		void SetB( float32 b )	{ m_Blue  = b;	}	//!<	青成分の設定
 
    void SetARGB( float32 a, float32 r, float32 g, float32 b )	//!<	いっきに設定
		{
			SetA( a );
			SetR( r );
			SetG( g );
			SetB( b );
		}

		//!	違うピクセルフォーマットとのコピー
		template <class PIXEL>
		COLOR_A32B32G32R32F& operator = (const PIXEL &src)
		{
			SetARGB( src.GetA(),src.GetR(),src.GetG(),src.GetB() );
			return *this;
		}

		//!	同じピクセルフォーマットとのコピー
		COLOR_A32B32G32R32F& operator = (const COLOR_A32B32G32R32F& src)
		{
			m_Alpha = src.m_Alpha;
			m_Red   = src.m_Red;
			m_Green = src.m_Green;
			m_Blue  = src.m_Blue;
			return *this;
		}

	private:
		float32 m_Alpha;	//	透明度
		float32 m_Blue;		//	青成分
		float32 m_Green;	//	緑成分
		float32 m_Red;		//	赤成分
	};
}

#endif
