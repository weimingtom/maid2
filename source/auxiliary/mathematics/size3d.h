/*!
    @file
    @brief	３次元サイズテンプレート
 */



#ifndef maid2_auxiliary_mathematics_size3d_h
#define maid2_auxiliary_mathematics_size3d_h

#include"../../config/define.h"


namespace Maid
{ 
  /*!
      @brief	横幅、縦幅,奥幅を保持する構造体
   */
  template<typename TYPE>
  struct SIZE3D_TEMPLATE
  {
    typedef SIZE3D_TEMPLATE<TYPE> SIZE;

    TYPE	w;		//!<	横幅
    TYPE	h;		//!<	縦幅
    TYPE	d;		//!<	奥幅

    SIZE3D_TEMPLATE(){}
    SIZE3D_TEMPLATE( TYPE _w, TYPE _h, TYPE _d ) 	//!<	コンストラクタ
      : w(_w), h(_h),d(_d){}
    template<typename TYPE2>
    SIZE3D_TEMPLATE( const SIZE3D_TEMPLATE<TYPE2>& rha ) 	//!<	コンストラクタ
    {
      w = (TYPE)rha.w;
      h = (TYPE)rha.h;
      d = (TYPE)rha.d;
    }

    //	比較演算子
    bool operator==( const SIZE& rha ) { return w==rha.w && h==rha.h && d==rha.d; }
    bool operator!=( const SIZE& rha ) { return !(*this==rha); }
	};
}

#endif
