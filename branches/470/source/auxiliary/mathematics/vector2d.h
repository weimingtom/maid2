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
    VECTOR2D_TEMPLATE( const POINT& pos ) { Reset(pos); }
  	
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

  //! 外積を求める
  /*!
      @param	lha		[i ]	ベクトルその１
      @param	rha		[i ]	ベクトルその２
     
      @return	外積の値（0だと同じベクトル)
   */
  template<typename TYPE>
  inline TYPE VectorCross( const VECTOR2D_TEMPLATE<TYPE>& lha, const VECTOR2D_TEMPLATE<TYPE>& rha )
  {
    //  ２次の外積なんてなので、(x,y,0)にして求める
    const VECTOR3D_TEMPLATE<TYPE> ret(
        (lha.y*    0) - (    0*rha.y),
        (    0*rha.x) - (lha.x*    0),
        (lha.x*rha.y) - (lha.y*rha.x)
      ); 

    return ret.z;
  }

  template<typename TYPE>
  inline VECTOR2D_TEMPLATE<TYPE> VectorRotate( const VECTOR2D_TEMPLATE<TYPE>& lha, float rad )
  {
    /*
      行列計算は
      |x,y|| cos, sin|
           |-sin, cos|
      x' = x * cosθ - y * sinθ
      y' = x * sinθ + y * cosθ
      です。
      普通に図を描いて、加法定理を使えばわかるかと。
    */

    const TYPE cos = Math<TYPE>::cos(rad);
    const TYPE sin = Math<TYPE>::sin(rad);

    const TYPE x = lha.x * cos - lha.y * sin;
    const TYPE y = lha.x * sin + lha.y * cos;

    return VECTOR2D_TEMPLATE<TYPE>(x,y);
  }
}

//	Windows のときのみ DXLIB を使ったバージョンを存在させる
#ifdef USE_DIRECT3DX9
  #include"../../config/win32.h"
  #include<d3dx9.h>

  namespace Maid
  {
    namespace d3dxvector2d
    {
      typedef D3DXVECTOR2* (WINAPI *NORMALIZE)(D3DXVECTOR2*,CONST D3DXVECTOR2*);
      extern NORMALIZE Normalize;

      void Initialize( HMODULE hModule );
      void Finalize();
    }

    const VECTOR2D_TEMPLATE<FLOAT>& VECTOR2D_TEMPLATE<FLOAT>::Normalize()
    {
      d3dxvector2d::Normalize( (D3DXVECTOR2*)this, (D3DXVECTOR2*)this );
      return *this;
    }
  }
#endif

#endif
