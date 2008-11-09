/*!
    @file
    @brief	数学演算テンプレート
 */



#ifndef maid2_auxiliary_mathematics_function_h
#define maid2_auxiliary_mathematics_function_h

#include"../../config/define.h"

#include<cmath>

namespace Maid
{ 
  /*!
      @class Math function.h
      @brief いろいろな数学演算を行う関数
   */

  template<typename TYPE>
  class Math
  {
  public:
    static TYPE sin( TYPE rad ) { return std::sin(rad); }
    static TYPE cos( TYPE rad ) { return std::cos(rad); }
    static TYPE tan( TYPE rad ) { return std::tan(rad); }
    static TYPE atan2( TYPE y, TYPE x ) { return std::atan2(y,x); }
    static TYPE sqrt( TYPE rad ) { return std::sqrt(rad); }
    static TYPE log10( TYPE val ) { return std::log10(val); }
    static TYPE pow( TYPE x, TYPE y ) { return std::pow(x,y); }
    static TYPE abs( TYPE x ) { return std::abs(x); }

    static TYPE b_spline( TYPE x1, TYPE x2, TYPE x3, TYPE time )
    {
      const TYPE t  = time;
      const TYPE it = 1-time;

      return (it*it*x1) + (2*t*it*x2) + (t*t*x3);
    }
  };
}

#endif