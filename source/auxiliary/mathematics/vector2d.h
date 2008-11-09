/*!
    @file
    @brief	２次元ベクトルテンプレート
 */



#ifndef maid2_auxiliary_mathematics_vector2d_h
#define maid2_auxiliary_mathematics_vector2d_h

#include"../../config/define.h"
#include"point2d.h"
#include"function.h"

namespace Maid
{ 
  /*!
      @brief	２次元ベクトルを保持する構造体
   */
  template<typename TYPE>
  struct VECTOR2D_TEMPLATE
  { 
    typedef VECTOR2D_TEMPLATE<TYPE> VECTOR;
    typedef POINT2D_TEMPLATE<TYPE>  POINT;

    TYPE	x;		//!<	横
    TYPE	y;		//!<	縦

    VECTOR2D_TEMPLATE(){}		//!	コンストラクタ
    VECTOR2D_TEMPLATE( TYPE _x, TYPE _y ): x(_x), y(_y){}
    VECTOR2D_TEMPLATE( const POINT& Start, const POINT& End ) { Reset(Start,End); }
  	
    //! 正規化する
    /*!
    */
    const VECTOR& Normalize()
    {
	    const TYPE f = Math<TYPE>::sqrt(x*x+y*y);
	    *this /= f;
	    return *this;
    }

    //! 座標をベクトルに変換する
    /*!
 	      @param	Start [i ]	ベクトルの開始座標
 	      @param	End   [i ]	ベクトルの終了座標
     */
    void Reset( const POINT& Start, const POINT& End )
    {
	    x = End.x-Start.x;
	    y = End.y-Start.y;
    }

    //! 座標をベクトルに変換する
    /*!
 	      @param	pos [i ]	ベクトルの開始座標
     */
    void Reset( const POINT& pos )
    {
	    x = pos.x;
	    y = pos.y;
    }

    POINT ConvertPOINT2D() { return POINT(x,y); }

    //	単項演算子
    VECTOR operator-() const{ return VECTOR(-x,-y); }

    //	比較演算子
    bool operator==( const VECTOR& rha ) const{ return x==rha.x && y==rha.y; }
    bool operator!=( const VECTOR& rha ) const{ return !(*this==rha); }

    //	再設定
    VECTOR& operator+=( const VECTOR& rha)	{ *this = *this+rha; return *this; }
    VECTOR& operator-=( const VECTOR& rha)	{ *this = *this-rha; return *this; }
    VECTOR& operator*=( const TYPE& val)	{ *this = *this*val; return *this; }
    VECTOR& operator/=( const TYPE& val)	{ *this = *this/val; return *this; }
  };


  //	二項演算子
  template<typename TYPE>
  VECTOR2D_TEMPLATE<TYPE> operator+( const VECTOR2D_TEMPLATE<TYPE>& lha, const VECTOR2D_TEMPLATE<TYPE>& rha )
  {
    return VECTOR2D_TEMPLATE<TYPE>( lha.x+rha.x, lha.y+rha.y); 
  }
  template<typename TYPE>
  VECTOR2D_TEMPLATE<TYPE> operator-( const VECTOR2D_TEMPLATE<TYPE>& lha, const VECTOR2D_TEMPLATE<TYPE>& rha )
  {
    return VECTOR2D_TEMPLATE<TYPE>( lha.x-rha.x, lha.y-rha.y); 
  }
  template<typename TYPE>
  VECTOR2D_TEMPLATE<TYPE> operator*( const VECTOR2D_TEMPLATE<TYPE>& lha, const TYPE& rha )
  {
    return VECTOR2D_TEMPLATE<TYPE>( lha.x*rha, lha.y*rha); 
  }
  template<typename TYPE>
  VECTOR2D_TEMPLATE<TYPE> operator/( const VECTOR2D_TEMPLATE<TYPE>& lha, const TYPE& rha )
  {
    return VECTOR2D_TEMPLATE<TYPE>( lha.x/rha, lha.y/rha); 
  }

  //! 内積を求める
  /*!
      @param	lha		[i ]	ベクトルその１
      @param	rha		[i ]	ベクトルその２
     
      @return	内積の値
   */
  template<typename TYPE>
  inline TYPE VectorDot( const VECTOR2D_TEMPLATE<TYPE>& lha, const VECTOR2D_TEMPLATE<TYPE>& rha )
  {
    return (lha.x*rha.x) + (lha.y*rha.y);
  }
}

//	Windows のときのみ DXLIB を使ったバージョンを存在させる
#ifdef USE_DIRECT3DX9
  #include<d3dx9.h>
  #pragma comment( lib, "d3dx9.lib" )

  namespace Maid
  {
    const VECTOR2D_TEMPLATE<FLOAT>& VECTOR2D_TEMPLATE<FLOAT>::Normalize()
    {
      D3DXVec2Normalize( (D3DXVECTOR2*)this, (D3DXVECTOR2*)this );
      return *this;
    }

    inline FLOAT VectorDot( const VECTOR2D_TEMPLATE<FLOAT>& lha, const VECTOR2D_TEMPLATE<FLOAT>& rha )
    {
      return D3DXVec2Dot( (D3DXVECTOR2*)&lha, (D3DXVECTOR2*)&rha );
    }
  }
#endif

#endif
