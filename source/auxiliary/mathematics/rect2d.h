/*!
    @file
    @brief  ２次元矩形テンプレート
 */

#ifndef maid2_auxiliary_mathematics_rect2d_h
#define maid2_auxiliary_mathematics_rect2d_h

#include"../../config/define.h"

#include"point2d.h"
#include"size2d.h"

namespace Maid
{ 
  /*!
      @brief	長方形をあらわす構造体
   */
  template<typename TYPE>
  struct RECT2D_TEMPLATE
  {
    TYPE x;	//	基準点
    TYPE y;
    TYPE w;	//	幅
    TYPE h;

    RECT2D_TEMPLATE(){}
    RECT2D_TEMPLATE( TYPE _x, TYPE _y, TYPE _w, TYPE _h ) //!<	コンストラクタ
      : x(_x),y(_y),w(_w),h(_h)
    {
    }	

    template<typename TYPE2>
    RECT2D_TEMPLATE( const RECT2D_TEMPLATE<TYPE2>& rha ) 	//!<	コンストラクタ
    {
      x = (TYPE)rha.x;
      y = (TYPE)rha.y;
      w = (TYPE)rha.w;
      h = (TYPE)rha.h;
    }

    RECT2D_TEMPLATE( const POINT2D_TEMPLATE<TYPE>& p, const SIZE2D_TEMPLATE<TYPE>& s ) //!<	コンストラクタ
      : x(p.x),y(p.y),w(s.w),h(s.h)
    {
    }	

    TYPE GetRight() const  { return x + w; }
    TYPE GetBottom() const { return y + h; }

    POINT2D_TEMPLATE<TYPE> GetPoint()const{ return POINT2D_TEMPLATE<TYPE>(x,y); }
    SIZE2D_TEMPLATE<TYPE>  GetSize() const{ return SIZE2D_TEMPLATE<TYPE>(w,h); }

    POINT2D_TEMPLATE<TYPE> GetLT()const{ return POINT2D_TEMPLATE<TYPE>(x  ,y  ); }
    POINT2D_TEMPLATE<TYPE> GetRT()const{ return POINT2D_TEMPLATE<TYPE>(x+w,y  ); }
    POINT2D_TEMPLATE<TYPE> GetLB()const{ return POINT2D_TEMPLATE<TYPE>(x  ,y+h); }
    POINT2D_TEMPLATE<TYPE> GetRB()const{ return POINT2D_TEMPLATE<TYPE>(x+w,y+h); }
  };
}
#endif
