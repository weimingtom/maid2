/*!
 	@file
 	@brief	独自頂点構造体テンプレート
 */

#ifndef maid2_graphics_customvertextemplate_h
#define maid2_graphics_customvertextemplate_h


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"

#include"../auxiliary/string.h"
#include"../auxiliary/mathematics.h"
#include"color.h"

namespace Maid
{ 
  /*!
    @brief  独自頂点構造体テンプレート
  \n        頂点構造を記述した構造体にこのテンプレートをかますことによって
  \n        いちいちメンバ関数を書く必要がなくなる
  \n        条件としてメンバ変数の名前を同じにしておくこと
   */
  template<class Struct>
  struct CUSTOMVERTEX_TEMPLATE
  {
  public:
    const int GetStructSize()   const{ return sizeof(Struct); } //!<  頂点サイズの取得
  };


  /*!
    @struct CUSTOMVERTEX_COLOR customvertextemplate.h
    @brief  頂点とカラー
   */
  struct CUSTOMVERTEX_COLOR : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_COLOR>
  {
    //! コンストラクタ
    CUSTOMVERTEX_COLOR(){};
    CUSTOMVERTEX_COLOR( const POINT3DF& pos, const COLOR_R32G32B32A32F& color )
      : Position(pos), Color(color){}
    POINT3DF            Position; //!< 頂点座標
    COLOR_R32G32B32A32F Color;    //!< 頂点色
  };

  /*!
    @struct	CUSTOMVERTEX_SPRITE customvertextemplate.h
    @brief	頂点とカラー,テクスチャ
   */
  struct CUSTOMVERTEX_SPRITE : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_SPRITE>
  {
    //!	コンストラクタ
    CUSTOMVERTEX_SPRITE(){};
    CUSTOMVERTEX_SPRITE( const POINT3DF& pos, const COLOR_R32G32B32A32F& color, const POINT2DF& tex)
      : Position(pos), Color(color)
    {
      Texture[0] = tex;
    }
    POINT3DF			Position;	    //!< 頂点座標
    COLOR_R32G32B32A32F	Color;	//!< 頂点色
    POINT2DF			Texture[1]; 	//!< テクスチャ座標
  };


  /*!
    @struct	CUSTOMVERTEX_SPRITE2 customvertextemplate.h
    @brief	頂点変換済みテクスチャ
   */
  struct CUSTOMVERTEX_SPRITE2 : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_SPRITE2>
  {
    //!	コンストラクタ
    CUSTOMVERTEX_SPRITE2(){};
    CUSTOMVERTEX_SPRITE2( const POINT3DF& pos, const COLOR_R32G32B32A32F& color, const POINT2DF& tex1, const POINT2DF& tex2 )
      : Position(pos), Color(color)
    {
      Texture[0] = tex1;
      Texture[1] = tex2;
    }
    POINT3DF			Position;	//!< 頂点座標
    COLOR_R32G32B32A32F	Color;		//!< 頂点色
    POINT2DF			Texture[2];	//!< テクスチャ座標
  };

#if 0
  /*!
    @struct	CUSTOMVERTEX_SPRITE3 customvertextemplate.h
    @brief	頂点変換済みテクスチャ
   */
  struct CUSTOMVERTEX_SPRITE3 : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_SPRITE3,VERTEXFORMAT_SPRITE3>
  {
    //!	コンストラクタ
    CUSTOMVERTEX_SPRITE3(){};
    CUSTOMVERTEX_SPRITE3( const POINT3DF& pos, float32 rhw, const COLOR_A08R08G08B08I& color, const POINT2DF& tex1, const POINT2DF& tex2, const POINT2DF& tex3 )
      : Position(pos), RHW(rhw), Color(color)
    {
      Texture[0] = tex1;
      Texture[1] = tex2;
      Texture[2] = tex3;
    }
    POINT3DF			Position;	//!< 頂点座標
    float32				RHW;		//!< 頂点変換値
    COLOR_A08R08G08B08I	Color;		//!< 頂点色
    POINT2DF			Texture[3];	//!< テクスチャ座標
  };

  /*!
    @struct	CUSTOMVERTEX_SPRITE4 customvertextemplate.h
    @brief	頂点変換済みテクスチャ
   */
  struct CUSTOMVERTEX_SPRITE4 : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_SPRITE4,VERTEXFORMAT_SPRITE4>
  {
    //!	コンストラクタ
    CUSTOMVERTEX_SPRITE4(){};
    CUSTOMVERTEX_SPRITE4( const POINT3DF& pos, float32 rhw, const COLOR_A08R08G08B08I& color, const POINT2DF& tex1, const POINT2DF& tex2, const POINT2DF& tex3, const POINT2DF& tex4 )
      : Position(pos), RHW(rhw), Color(color)
    {
      Texture[0] = tex1;
      Texture[1] = tex2;
      Texture[2] = tex3;
      Texture[3] = tex4;
    }
    POINT3DF			Position;	//!< 頂点座標
    float32				RHW;		//!< 頂点変換値
    COLOR_A08R08G08B08I	Color;		//!< 頂点色
    POINT2DF			Texture[4];	//!< テクスチャ座標
  };

  /*!
    @struct	CUSTOMVERTEX_3DCOLOR customvertextemplate.h
    @brief	頂点変換前カラー
   */
  struct CUSTOMVERTEX_3DCOLOR : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_3DCOLOR,VERTEXFORMAT_3DCOLOR>
  {
    //!	コンストラクタ
    CUSTOMVERTEX_3DCOLOR(){};
    CUSTOMVERTEX_3DCOLOR( const POINT3DF& pos, const COLOR_A08R08G08B08I& color)
      : Position(pos), Color(color){}
    POINT3DF			Position;	//!< 頂点座標
    COLOR_A08R08G08B08I	Color;		//!< 頂点色
  };

  /*!
    @struct	CUSTOMVERTEX_3DSPRITE customvertextemplate.h
    @brief	頂点変換前テクスチャ
   */
  struct CUSTOMVERTEX_3DSPRITE : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_3DSPRITE,VERTEXFORMAT_3DSPRITE>
  {
    //!	コンストラクタ
    CUSTOMVERTEX_3DSPRITE(){};
    CUSTOMVERTEX_3DSPRITE( const POINT3DF& pos, const COLOR_A08R08G08B08I& color, const POINT2DF& tex)
      : Position(pos), Color(color)
    {
      Texture[0] = tex;
    }

    POINT3DF			Position;	//!< 頂点座標
    COLOR_A08R08G08B08I	Color;		//!< 頂点色
    POINT2DF			Texture[1];	//!< テクスチャ座標
  };

  /*!
    @struct	CUSTOMVERTEX_3DSPRITE2 customvertextemplate.h
    @brief	頂点変換前テクスチャ
   */
  struct CUSTOMVERTEX_3DSPRITE2 : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_3DSPRITE2,VERTEXFORMAT_3DSPRITE2>
  {
    //!	コンストラクタ
    CUSTOMVERTEX_3DSPRITE2(){};
    CUSTOMVERTEX_3DSPRITE2( const POINT3DF& pos, const COLOR_A08R08G08B08I& color, const POINT2DF& tex1, const POINT2DF& tex2 )
      : Position(pos), Color(color)
    {
      Texture[0] = tex1;
      Texture[1] = tex2;
    }
    POINT3DF			Position;	//!< 頂点座標
    COLOR_A08R08G08B08I	Color;		//!< 頂点色
    POINT2DF			Texture[2];	//!< テクスチャ座標
  };

  /*!
    @struct	CUSTOMVERTEX_3DCOLORNORMAL customvertextemplate.h
    @brief	頂点変換前カラー法線付
   */
  struct CUSTOMVERTEX_3DCOLORNORMAL : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_3DCOLORNORMAL,VERTEXFORMAT_3DCOLORNORMAL>
  {
    //!	コンストラクタ
    CUSTOMVERTEX_3DCOLORNORMAL(){};
    CUSTOMVERTEX_3DCOLORNORMAL( const POINT3DF& pos, const VECTOR3DF& nom, const COLOR_A08R08G08B08I& color )
      : Position(pos), Normal(nom), Color(color){}
    POINT3DF			Position;	//!< 頂点座標
    VECTOR3DF			Normal;		//!< 法線ベクトル
    COLOR_A08R08G08B08I	Color;		//!< 頂点色
  };

  /*!
    @struct	CUSTOMVERTEX_3DSPRITENORMAL customvertextemplate.h
    @brief	頂点変換前テクスチャ法線付
   */
  struct CUSTOMVERTEX_3DSPRITENORMAL : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_3DSPRITENORMAL,VERTEXFORMAT_3DSPRITENORMAL>
  {
    //!	コンストラクタ
    CUSTOMVERTEX_3DSPRITENORMAL(){};
    CUSTOMVERTEX_3DSPRITENORMAL( const POINT3DF& pos, const VECTOR3DF& nom, const COLOR_A08R08G08B08I& color, const POINT2DF& tex)
      : Position(pos), Normal(nom), Color(color)
    {
      Texture[0] = tex;
    }

    POINT3DF			Position;	//!< 頂点座標
    VECTOR3DF			Normal;		//!< 法線ベクトル
    COLOR_A08R08G08B08I	Color;		//!< 頂点色
    POINT2DF			Texture[1];	//!< テクスチャ座標
  };

  /*!
    @struct	CUSTOMVERTEX_3DSPRITENORMAL2 customvertextemplate.h
    @brief	頂点変換前テクスチャ法線付
   */
  struct CUSTOMVERTEX_3DSPRITENORMAL2 : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_3DSPRITENORMAL2,VERTEXFORMAT_3DSPRITENORMAL2>
  {
    //!	コンストラクタ
    CUSTOMVERTEX_3DSPRITENORMAL2(){};
    CUSTOMVERTEX_3DSPRITENORMAL2( const POINT3DF& pos, const VECTOR3DF& nom, const COLOR_A08R08G08B08I& color, const POINT2DF& tex1 , const POINT2DF& tex2 )
      : Position(pos), Normal(nom), Color(color)
    {
      Texture[0] = tex1;
      Texture[1] = tex2;
    }
    POINT3DF			Position;	//!< 頂点座標
    VECTOR3DF			Normal;		//!< 法線ベクトル
    COLOR_A08R08G08B08I	Color;		//!< 頂点色
    POINT2DF			Texture[2];	//!< テクスチャ座標
  };

  /*!
    @struct	CUSTOMVERTEX_3DT0W1 customvertextemplate.h
    @brief	モデル用頂点フォーマット（テクスチャ０、ウェイト１)
   */
  struct CUSTOMVERTEX_3DT0W1 : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_3DT0W1,VERTEXFORMAT_3DT0W1>
  {
    POINT3DF			Position;	//!< 頂点座標
    unt08				Index[4];	//!< インデックス
    VECTOR3DF			Normal;		//!< 法線ベクトル
    COLOR_A08R08G08B08I	Color;		//!< 頂点色
  };
  /*!
    @struct	CUSTOMVERTEX_3DT0W2 customvertextemplate.h
    @brief	モデル用頂点フォーマット（テクスチャ０、ウェイト２)
   */
  struct CUSTOMVERTEX_3DT0W2 : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_3DT0W2,VERTEXFORMAT_3DT0W2>
  {
    POINT3DF			Position;	//!< 頂点座標
    float32				Blend[1];	//!< ブレンド
    unt08				Index[4];	//!< インデックス
    VECTOR3DF			Normal;		//!< 法線ベクトル
    COLOR_A08R08G08B08I	Color;		//!< 頂点色
  };
  /*!
    @struct	CUSTOMVERTEX_3DT0W3 customvertextemplate.h
    @brief	モデル用頂点フォーマット（テクスチャ０、ウェイト３)
   */
  struct CUSTOMVERTEX_3DT0W3 : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_3DT0W3,VERTEXFORMAT_3DT0W3>
  {
    POINT3DF			Position;	//!< 頂点座標
    float32				Blend[2];	//!< ブレンド
    unt08				Index[4];	//!< インデックス
    VECTOR3DF			Normal;		//!< 法線ベクトル
    COLOR_A08R08G08B08I	Color;		//!< 頂点色
  };
  /*!
    @struct	CUSTOMVERTEX_3DT0W4 customvertextemplate.h
    @brief	モデル用頂点フォーマット（テクスチャ０、ウェイト４)
   */
  struct CUSTOMVERTEX_3DT0W4 : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_3DT0W4,VERTEXFORMAT_3DT0W4>
  {
    POINT3DF			Position;	//!< 頂点座標
    float32				Blend[3];	//!< ブレンド
    unt08				Index[4];	//!< インデックス
    VECTOR3DF			Normal;		//!< 法線ベクトル
    COLOR_A08R08G08B08I	Color;		//!< 頂点色
  };
  /*!
    @struct	CUSTOMVERTEX_3DT1W1 customvertextemplate.h
    @brief	モデル用頂点フォーマット（テクスチャ１、ウェイト１)
   */
  struct CUSTOMVERTEX_3DT1W1 : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_3DT1W1,VERTEXFORMAT_3DT1W1>
  {
    POINT3DF			Position;	//!< 頂点座標
    unt08				Index[4];	//!< インデックス
    VECTOR3DF			Normal;		//!< 法線ベクトル
    COLOR_A08R08G08B08I	Color;		//!< 頂点色
    POINT2DF			Texture[1];	//!< テクスチャ座標
  };
  /*!
    @struct	CUSTOMVERTEX_3DT1W2 customvertextemplate.h
    @brief	モデル用頂点フォーマット（テクスチャ１、ウェイト２)
   */
  struct CUSTOMVERTEX_3DT1W2 : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_3DT1W2,VERTEXFORMAT_3DT1W2>
  {
    POINT3DF			Position;	//!< 頂点座標
    float32				Blend[1];	//!< ブレンド
    unt08				Index[4];	//!< インデックス
    VECTOR3DF			Normal;		//!< 法線ベクトル
    COLOR_A08R08G08B08I	Color;		//!< 頂点色
    POINT2DF			Texture[1];	//!< テクスチャ座標
  };
  /*!
    @struct	CUSTOMVERTEX_3DT1W3 customvertextemplate.h
    @brief	モデル用頂点フォーマット（テクスチャ１、ウェイト３)
   */
  struct CUSTOMVERTEX_3DT1W3 : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_3DT1W3,VERTEXFORMAT_3DT1W3>
  {
    POINT3DF			Position;	//!< 頂点座標
    float32				Blend[2];	//!< ブレンド
    unt08				Index[4];	//!< インデックス
    VECTOR3DF			Normal;		//!< 法線ベクトル
    COLOR_A08R08G08B08I	Color;		//!< 頂点色
    POINT2DF			Texture[1];	//!< テクスチャ座標
  };
  /*!
    @struct	CUSTOMVERTEX_3DT1W4 customvertextemplate.h
    @brief	モデル用頂点フォーマット（テクスチャ１、ウェイト４)
   */
  struct CUSTOMVERTEX_3DT1W4 : public CUSTOMVERTEX_TEMPLATE<CUSTOMVERTEX_3DT1W4,VERTEXFORMAT_3DT1W4>
  {
    POINT3DF			Position;	//!< 頂点座標
    float32				Blend[3]; //!< ブレンド
    unt08				Index[4];	//!< インデックス
    VECTOR3DF			Normal;		//!< 法線ベクトル
    COLOR_A08R08G08B08I	Color;		//!< 頂点色
    POINT2DF			Texture[1];	//!< テクスチャ座標
  };
#endif
}

#endif
