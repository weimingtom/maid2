#ifndef maid2_graphics_light_h
#define maid2_graphics_light_h

/*!
    @file
    @brief  light
 */


#include"../config/define.h"

/*
  ライトのアルゴリズムはここがわかりやすい
http://www.atelier-blue.com/program/mdirectx/3d/3d02-07.htm
*/
namespace Maid
{
  struct LIGHT
  {
    enum TYPE
    {
      DIRECTIONAL,
      POINT,
    };

    //  共通設定
    TYPE Type;
    COLOR_R32G32B32A32F Diffuse;

    //  平行光源用
    VECTOR3DF  Direction;

    //  点光源用
    POINT3DF  Position;
    float     Range;

    //  点光源・
    float     Attenuation0; //  1*Attenuation0
    float     Attenuation1; //  d*Attenuation1
    float     Attenuation2; //  d^2*Attenuation2
  };

}

#endif