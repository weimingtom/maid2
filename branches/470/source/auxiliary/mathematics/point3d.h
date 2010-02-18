/*!
    @file
    @brief	３次元座標テンプレート
 */



#ifndef maid2_auxiliary_mathematics_point3d_h
#define maid2_auxiliary_mathematics_point3d_h

#include"../../config/define.h"

namespace Maid
{ 
  /*!
     @brief ３次元座標を保持する構造体
   */
  template<typename TYPE>
  struct POINT3D_TEMPLATE
  {
    typedef POINT2D_TEMPLATE<TYPE> POINT;

    TYPE	x;		//!<	横
    TYPE	y;		//!<	縦
    TYPE	z;		//!<	奥

    POINT3D_TEMPLATE(){}	//!<	コンストラクタ
    POINT3D_TEMPLATE( TYPE _x, TYPE _y, TYPE _z ) 	//!<	コンストラクタ
      : x(_x), y(_y), z(_z){}
    template<typename TYPE2>
    POINT3D_TEMPLATE( const POINT3D_TEMPLATE<TYPE2>& rha ) 	//!<	コンストラクタ
    {
      x = (TYPE)rha.x;
      y = (TYPE)rha.y;
      z = (TYPE)rha.z;
    }
    //	単項演算子
    POINT operator-() { return POINT(-x,-y,-z); }

    //	比較演算子
    bool operator==( const POINT& rha ) { return x==rha.x && y==rha.y && z==rha.z; }
    bool operator!=( const POINT& rha ) { return !(*this==rha); }
  };
  //	座標同士を足したり引いたりするのはおかしいので、
  //	二項演算子は用意しない VECTOR を使うこと

}

#endif
