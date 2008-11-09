/*!
    @file
    @brief	２次元サイズテンプレート
 */



#ifndef maid2_auxiliary_mathematics_size2d_h
#define maid2_auxiliary_mathematics_size2d_h

#include"../../config/define.h"

namespace Maid
{ 
  /*!
      @brief	横幅、縦幅を保持する構造体
   */
  template<typename TYPE>
  struct SIZE2D_TEMPLATE
  {
    typedef SIZE2D_TEMPLATE<TYPE> SIZE;

    TYPE	w;		//!<	横幅
    TYPE	h;		//!<	縦幅

    SIZE2D_TEMPLATE(){}				 		        //!<	コンストラクタ
    SIZE2D_TEMPLATE( TYPE _w, TYPE _h )   //!<	コンストラクタ
      : w(_w), h(_h){}
    template<typename TYPE2>
    SIZE2D_TEMPLATE( const SIZE2D_TEMPLATE<TYPE2>& rha ) 	//!<	コンストラクタ
    {
      w = (TYPE)rha.w;
      h = (TYPE)rha.h;
    }

    //	比較演算子
    bool operator==( const SIZE& rha ) { return w==rha.w && h==rha.h; }
    bool operator!=( const SIZE& rha ) { return !(*this==rha); }
	};
}

#endif
