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
  Maid::COLOR_R32G32B32A32F s_MaterialColor;  // 素材そのものの色
  Maid::COLOR_R32G32B32A32F s_MaterialLight;  // 素材の光反射率
  Maid::COLOR_R32G32B32A32F s_MaterialEmissive;  // 素材の自己発光量
  Maid::VECTOR4DF           s_LightDir;       // 平行光源の向き
  Maid::COLOR_R32G32B32A32F s_LightColor;     // 平行光源の色
  Maid::COLOR_R32G32B32A32F s_Ambient;        // ワールド全体の明るさ
};


struct CONSTANT_TEXTURE_COLOR_LIGHT0
{
  Maid::MATRIX4DF s_mWVP;
  Maid::SIZE2DF   s_TextureScale; //  テクスチャのＵＶとデータ上のＵＶを調節するための値
  float32         s_Tmp[2];       //  c4レジスタの空き
};

struct CONSTANT_TEXTURE_DIRECTIONALLIGHT
{
  Maid::MATRIX4DF s_mWVP;
  Maid::COLOR_R32G32B32A32F s_MaterialLight;  // 素材の光反射率
  Maid::COLOR_R32G32B32A32F s_MaterialEmissive;  // 素材の自己発光量
  Maid::VECTOR4DF           s_LightDir;       // 平行光源の向き
  Maid::COLOR_R32G32B32A32F s_LightColor;     // 平行光源の色
  Maid::COLOR_R32G32B32A32F s_Ambient;        // ワールド全体の明るさ
  Maid::SIZE2DF   s_TextureScale; //  テクスチャのＵＶとデータ上のＵＶを調節するための値
  float32         s_Tmp[2];       //  c4レジスタの空き
};


struct CONSTANTSPRITE
{
  Maid::MATRIX4DF mWVP;
};

#endif

