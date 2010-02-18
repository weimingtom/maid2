/*!
    @file
    @brief	１次元直線テンプレート
  */



#ifndef maid2_auxiliary_mathematics_line1d_h
#define maid2_auxiliary_mathematics_line1d_h

#include"../../config/define.h"

namespace Maid
{ 
  /*!
      @brief	１次元直線を保持する構造体
      始点 <= x < 終点です
   */
  template<typename TYPE>
  struct LINE1D_TEMPLATE
  {
    TYPE	begin;		//!<	始点
    TYPE	end;		//!<	終点

    LINE1D_TEMPLATE(){}	//!<	コンストラクタ
    LINE1D_TEMPLATE( TYPE x0, TYPE x1 ) 	//!<	コンストラクタ
	    : begin(x0), end(x1){}
    template<typename TYPE2>
    LINE1D_TEMPLATE( const LINE1D_TEMPLATE<TYPE2>& rha ) 	//!<	コンストラクタ
	    : begin(rha.begin), end(rha.end){}
  };

  template<typename TYPE>
  bool operator==( const LINE1D_TEMPLATE<TYPE>& lha, const LINE1D_TEMPLATE<TYPE>& rha )//!<	比較
  {
    return lha.begin==rha.begin && lha.end==rha.end; 
  }
  template<typename TYPE>
  bool operator!=( const LINE1D_TEMPLATE<TYPE>& lha, const LINE1D_TEMPLATE<TYPE>& rha )//!<	比較
  {
    return !(lha==rha); 
  }

	//	直線同士の結合はできないと思うので + 演算子のオーバーロードは禁止
}

#endif
