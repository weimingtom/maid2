#ifndef maid2_graphics_graphics3drenderlocal_h
#define maid2_graphics_graphics3drenderlocal_h

#include"../config/define.h"
#include"../auxiliary/mathematics.h"
#include"color.h"

struct CONSTANTSPRITE
{
  Maid::MATRIX4DF mWVP;
};




struct CONSTANT0100
{
  Maid::MATRIX4DF s_mWVP;
  Maid::COLOR_R32G32B32A32F s_MaterialColor;  //  素材そのものの色
  float s_Alpha;  //  プログラム側で調節するモデルの透明度
};



struct CONSTANT_COLOR_DIRECTIONALLIGHT_VS
{
  Maid::MATRIX4DF s_mWVP;
  Maid::VECTOR4DF s_Eye;       // 視線ベクトル
};

struct CONSTANT_COLOR_DIRECTIONALLIGHT_PS
{
  Maid::COLOR_R32G32B32A32F s_MaterialColor;  // 素材そのものの色
  Maid::COLOR_R32G32B32A32F s_MaterialLight;  // 素材の光反射率
  Maid::COLOR_R32G32B32A32F s_MaterialEmissive;  // 素材の自己発光量
  Maid::VECTOR4DF           s_LightDir;       // 平行光源の向き
  Maid::COLOR_R32G32B32A32F s_LightColor;     // 平行光源の色
  Maid::COLOR_R32G32B32A32F s_Ambient;        // ワールド全体の明るさ
  float           s_Alpha;  //  プログラム側で調節するモデルの透明度
  float32         s_Tmp[3];       //  s_Alphaレジスタの空き
  float           s_Speculer;     //  スペキュラの効き具合
  float           s_SpeculerPow;  //  スペキュラのパワー
  float32         s_Tmp2[2];      //  s_Speculerレジスタの空き
};





struct CONSTANT_TEXTURE_COLOR_LIGHT0
{
  Maid::MATRIX4DF s_mWVP;
  Maid::SIZE2DF   s_TextureScale; //  テクスチャのＵＶとデータ上のＵＶを調節するための値
  float32         s_Tmp[2];       //  s_TextureScaleレジスタの空き
  float s_Alpha;  //  プログラム側で調節するモデルの透明度
};

struct CONSTANT_TEXTURE_DIRECTIONALLIGHT_VS
{
  Maid::MATRIX4DF s_mWVP;
  Maid::VECTOR4DF s_Eye;          // カメラの向き
  Maid::SIZE2DF   s_TextureScale; //  テクスチャのＵＶとデータ上のＵＶを調節するための値
  float32         s_Tmp[2];       //  s_TextureScaleレジスタの空き
};

struct CONSTANT_TEXTURE_DIRECTIONALLIGHT_PS
{
  Maid::COLOR_R32G32B32A32F s_MaterialColor;  // 素材そのものの色
  Maid::COLOR_R32G32B32A32F s_MaterialLight;  // 素材の光反射率
  Maid::COLOR_R32G32B32A32F s_MaterialEmissive;  // 素材の自己発光量
  Maid::VECTOR4DF           s_LightDir;       // 平行光源の向き
  Maid::COLOR_R32G32B32A32F s_LightColor;     // 平行光源の色
  Maid::COLOR_R32G32B32A32F s_Ambient;        // ワールド全体の明るさ
  float           s_Alpha;        //  プログラム側で調節するモデルの透明度
  float32         s_Tmp2[3];      //  s_Alphaレジスタの空き
  float           s_Speculer;     //  スペキュラの効き具合
  float           s_SpeculerPow;  //  スペキュラのパワー
  float32         s_Tmp3[2];      //  s_Speculerレジスタの空き
};


struct CONSTANT_BUMP_DIRECTIONALLIGHT_VS
{
  Maid::MATRIX4DF s_mWVP;
  Maid::VECTOR4DF s_Eye;          // カメラの向き
  Maid::VECTOR4DF s_LightDir;     // 平行光源の向き
  Maid::SIZE2DF   s_TextureScale; //  テクスチャのＵＶとデータ上のＵＶを調節するための値
  float32         s_Tmp[2];       //  s_TextureScaleレジスタの空き
};


struct CONSTANT_BUMP_DIRECTIONALLIGHT_PS
{
  Maid::COLOR_R32G32B32A32F s_MaterialColor;  // 素材そのものの色
  Maid::COLOR_R32G32B32A32F s_MaterialLight;  // 素材の光反射率
  Maid::COLOR_R32G32B32A32F s_MaterialEmissive;  // 素材の自己発光量
  Maid::COLOR_R32G32B32A32F s_LightColor;     // 平行光源の色
  Maid::COLOR_R32G32B32A32F s_Ambient;        // ワールド全体の明るさ
  float           s_Alpha;  //  プログラム側で調節するモデルの透明度
  float32         s_Tmp2[3];       //  s_Alphaレジスタの空き
  float           s_Speculer;     //  スペキュラの効き具合
  float           s_SpeculerPow;  //  スペキュラのパワー
  float32         s_Tmp3[2];      //  s_Speculerレジスタの空き
};










struct CONSTANT_SHADOWMAP_1pass_VS
{
  Maid::MATRIX4DF s_mWVP; //  カメラ座標系
};

struct CONSTANT_SHADOWMAP_2pass_VS
{
  Maid::MATRIX4DF s_mWVP;         //  カメラ座標系
  Maid::MATRIX4DF s_mWLightP;     //  ライト射影空間
  Maid::MATRIX4DF s_mWLightPTex;  //  ライト射影空間->テクスチャ座標系
  Maid::VECTOR4DF s_Eye;          // カメラの向き
};

struct CONSTANT_SHADOWMAP_2pass_COLOR_PS
{
  Maid::COLOR_R32G32B32A32F s_MaterialColor;  // 素材そのものの色
  Maid::COLOR_R32G32B32A32F s_MaterialLight;  // 素材の光反射率
  Maid::COLOR_R32G32B32A32F s_MaterialEmissive;  // 素材の自己発光量
  Maid::VECTOR4DF           s_LightDir;       // 平行光源の向き
  Maid::COLOR_R32G32B32A32F s_LightColor;     // 平行光源の色
  Maid::COLOR_R32G32B32A32F s_Ambient;        // ワールド全体の明るさ
  float           s_Alpha;  //  プログラム側で調節するモデルの透明度
  float32         s_Tmp[3];       //  s_Alphaレジスタの空き
  float           s_Speculer;     //  スペキュラの効き具合
  float           s_SpeculerPow;  //  スペキュラのパワー
  float32         s_Tmp2[2];      //  s_Speculerレジスタの空き
};



struct CONSTANT_SHADOWMAP_2pass_TEXTURE_PS
{
  Maid::COLOR_R32G32B32A32F s_MaterialColor;  // 素材そのものの色
  Maid::COLOR_R32G32B32A32F s_MaterialLight;  // 素材の光反射率
  Maid::COLOR_R32G32B32A32F s_MaterialEmissive;  // 素材の自己発光量
  Maid::VECTOR4DF           s_LightDir;       // 平行光源の向き
  Maid::COLOR_R32G32B32A32F s_LightColor;     // 平行光源の色
  Maid::COLOR_R32G32B32A32F s_Ambient;        // ワールド全体の明るさ
  float           s_Alpha;  //  プログラム側で調節するモデルの透明度
  float32         s_Tmp[3];       //  s_Alphaレジスタの空き
  float           s_Speculer;     //  スペキュラの効き具合
  float           s_SpeculerPow;  //  スペキュラのパワー
  float32         s_Tmp2[2];      //  s_Speculerレジスタの空き
};


#endif

