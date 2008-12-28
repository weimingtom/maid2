/*!
 	  @file
 	  @brief	４次元ベクトルテンプレート
 */



#ifndef maid2_auxiliary_mathematics_vector4d_h
#define maid2_auxiliary_mathematics_vector4d_h

#include"../../config/define.h"
#include"point4d.h"
#include"function.h"


namespace Maid
{ 
  /*!
      @brief	４次元ベクトルを保持する構造体
   */
  template<typename TYPE>
  struct VECTOR4D_TEMPLATE
  { 
    typedef VECTOR4D_TEMPLATE<TYPE> VECTOR;
    typedef POINT4D_TEMPLATE<TYPE>  POINT;

    TYPE	x;		//!<	横
    TYPE	y;		//!<	縦
    TYPE	z;		//!<	奥
    TYPE	w;		//!<	w?

    VECTOR4D_TEMPLATE(){}
    //!	コンストラクタ
    VECTOR4D_TEMPLATE( TYPE _x, TYPE _y, TYPE _z, TYPE _w ): x(_x), y(_y), z(_z), w(_w){}
    VECTOR4D_TEMPLATE( const POINT& Start, const POINT& End ) {	Reset(Start,End); }
  	

    //! 正規化する
    /*!
    */
    const VECTOR& Normalize()
    {
      const TYPE f = Math<TYPE>::sqrt(x*x+y*y+z*z+w*w);
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
      z = End.z-Start.z;
      w = End.w-Start.w;
    }
    //! 座標をベクトルに変換する
    /*!
        @param	pos [i ]	ベクトルの開始座標
     */
    void Reset( const POINT& pos )
    {
      x = pos.x;
      y = pos.y;
      z = pos.z;
      w = pos.w;
    }

    //	単項演算子
    VECTOR operator-() const{ return VECTOR(-x,-y,-z,-w); }

    //	比較演算子
    bool operator==( const VECTOR& rha ) const{ return x==rha.x && y==rha.y && z==rha.z && w==rha.w; }
    bool operator!=( const VECTOR& rha ) const{ return !(*this==rha); }

    //	再設定
    VECTOR& operator+=( const VECTOR& rha)	{ *this = *this+rha; return *this; }
    VECTOR& operator-=( const VECTOR& rha)	{ *this = *this-rha; return *this; }
    VECTOR& operator*=( const TYPE& val)	{ *this = *this*val; return *this; }
    VECTOR& operator/=( const TYPE& val)	{ *this = *this/val; return *this; }

  };

  //	二項演算子
  template<typename TYPE>
  VECTOR4D_TEMPLATE<TYPE> operator-( const VECTOR4D_TEMPLATE<TYPE>& lha, const VECTOR4D_TEMPLATE<TYPE>& rha )
  {
    return VECTOR4D_TEMPLATE<TYPE>( lha.x-rha.x, lha.y-rha.y, lha.z-rha.z, lha.w-rha.w); 
  }
  template<typename TYPE>
  VECTOR4D_TEMPLATE<TYPE> operator+( const VECTOR4D_TEMPLATE<TYPE>& lha, const VECTOR4D_TEMPLATE<TYPE>& rha )
  {
    return VECTOR4D_TEMPLATE<TYPE>( lha.x+rha.x, lha.y+rha.y, lha.z+rha.z, lha.w+rha.w); 
  }
  template<typename TYPE>
  VECTOR4D_TEMPLATE<TYPE> operator*( const VECTOR4D_TEMPLATE<TYPE>& lha, const TYPE& rha )
  {
    return VECTOR4D_TEMPLATE<TYPE>( lha.x*rha, lha.y*rha, lha.z*rha, lha.w*rha); 
  }
  template<typename TYPE>
  VECTOR4D_TEMPLATE<TYPE> operator/( const VECTOR4D_TEMPLATE<TYPE>& lha, const TYPE& rha )
  {
    return VECTOR4D_TEMPLATE<TYPE>( lha.x/rha, lha.y/rha, lha.z/rha, lha.w/rha); 
  }

  //! 内積を求める
  /*!
      @param	lha		[i ]	ベクトルその１
      @param	rha		[i ]	ベクトルその２
     
      @return	内積の値
   */
  template<typename TYPE>
  inline TYPE VectorDot( const VECTOR4D_TEMPLATE<TYPE>& lha, const VECTOR4D_TEMPLATE<TYPE>& rha )
  {
    return (lha.x*rha.x) + (lha.y*rha.y) + (lha.z*rha.z) + (lha.w*rha.w);
  }


}
	//	Windows のときのみ DXLIB を使ったバージョンを存在させる
  #ifdef USE_DIRECT3DX9
  #include"../../config/win32.h"
    #include<d3dx9.h>
    #pragma comment( lib, "d3dx9.lib" )

    namespace Maid
    {
      const VECTOR4D_TEMPLATE<FLOAT>& VECTOR4D_TEMPLATE<FLOAT>::Normalize()
      {
	      D3DXVec4Normalize( (D3DXVECTOR4*)this, (D3DXVECTOR4*)this );
	      return *this;
      }

      inline FLOAT VectorDot( const VECTOR4D_TEMPLATE<FLOAT>& lha, const VECTOR4D_TEMPLATE<FLOAT>& rha )
      {
	      return D3DXVec4Dot( (D3DXVECTOR4*)&lha, (D3DXVECTOR4*)&rha );
      }
    }
  #endif


#endif
