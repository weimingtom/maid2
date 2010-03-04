#ifndef maid2_graphics_graphics3drenderlocal_h
#define maid2_graphics_graphics3drenderlocal_h

#include"../config/define.h"
#include"../auxiliary/mathematics.h"
#include"color.h"

struct CONSTANT0100
{
  Maid::MATRIX4DF s_mWVP;
  Maid::COLOR_R32G32B32A32F s_MaterialColor;  //  マテリアルの色
};

struct CONSTANT_COLOR_DIRECTIONALLIGHT
{
  Maid::MATRIX4DF s_mWVP;
  Maid::COLOR_R32G32B32A32F s_MaterialColor;  //  マテリアルの色
  Maid::VECTOR4DF s_Light;
  Maid::COLOR_R32G32B32A32F s_LightColor;  //  ライトの色
};

struct CONSTANTSPRITE
{
  Maid::MATRIX4DF mWVP;
};

#endif

