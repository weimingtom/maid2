/*!
    @file
    @brief	３次元矩形テンプレート
 */

#ifndef maid2_auxiliary_mathematics_rect3d_h
#define maid2_auxiliary_mathematics_rect3d_h

#include"../../config/define.h"

#include"point3d.h"
#include"size3d.h"

namespace Maid
{ 
  /*!
      @brief	直方体をあらわす構造体
   */
  template<typename TYPE>
  struct RECT3D_TEMPLATE
  {
    TYPE x;	//	基準点
    TYPE y;
    TYPE z;

    TYPE w;	//	幅
    TYPE h;
    TYPE d;

    RECT3D_TEMPLATE(){}
    RECT3D_TEMPLATE( TYPE _x, TYPE _y, TYPE _z, TYPE _w, TYPE _h, TYPE _d ) //!<	コンストラクタ
      : x(_x),y(_y),z(_z),w(_w),h(_h),d(_d)
    {
    }	

    template<typename TYPE2>
    RECT3D_TEMPLATE( const RECT3D_TEMPLATE<TYPE2>& rha ) 	//!<	コンストラクタ
    {
      x = (TYPE)rha.x;
      y = (TYPE)rha.y;
      z = (TYPE)rha.z;
      w = (TYPE)rha.w;
      h = (TYPE)rha.h;
      d = (TYPE)rha.d;
    }

    TYPE GetRight() const  { return x + w; }
    TYPE GetBottom()const { return y + h; }
    TYPE GetBack()	const { return z + d; }

    POINT3D_TEMPLATE<TYPE> GetPoint()const{ return POINT3D_TEMPLATE<TYPE>(x,y,z); }
    SIZE3D_TEMPLATE<TYPE>  GetSize() const{ return SIZE3D_TEMPLATE<TYPE>(w,h,d); }
  };

}
#endif
