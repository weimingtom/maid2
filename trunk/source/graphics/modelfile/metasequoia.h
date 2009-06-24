#ifndef maid2_graphics_modelfile_metasequoia_h
#define maid2_graphics_modelfile_metasequoia_h

/*!
    @file
    @brief  メタセコイアローダ
 */


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"../../auxiliary/string.h"
#include"../../auxiliary/mathematics.h"

#include"../color.h"
#include<vector>

namespace Maid
{
  namespace Metasequoia
  {
    struct DATA
    {
      struct SCENE
      {
        COLOR_R32G32B32A32F Ambient;
      };

      SCENE Scene;

      struct MATERIAL
      {
        String Name;
        COLOR_R32G32B32A32F Color;
        float Diffuse;
        float Ambient;
        float Specular;
      	float Emissive;
        float Power;
        String Texture;
        String Alpha;
        String Bump;
      };

      std::vector<MATERIAL> Material;

      struct OBJECT
      {
        String Name;
        int Shading;
        int Depth;
        SIZE3DF Scale;
        float RotateHedding;//  Y軸回転(ラジアン度)
        float RotatePitch;  //  X軸回転
        float RotateBank;   //  Z軸回転
        VECTOR3DF Trans;

        // 辺の色タイプ
        enum COLORTYPE
        {
	        COLORTYPE_AMBIENT=0,	//!<	環境設定での色を使用
	        COLORTYPE_OBJECT =1,	//!<	オブジェクト固有の色を使用
        };

        COLOR_R32G32B32A32F Color;
        COLORTYPE ColorType;
        std::vector<POINT3DF> Point;

        struct FACE
        {
          //  三角形専用です。
          enum
          {
            MATERIAL_NONE = -1, //  マテリアルが設定されていない
          };

          FACE()
          {
            PointIndex[0] = 0;
            PointIndex[1] = 0;
            PointIndex[2] = 0;
            MaterialNo = MATERIAL_NONE;
            UV[0] = POINT2DF(0,0);
            UV[1] = POINT2DF(0,0);
            UV[2] = POINT2DF(0,0);
            Color[0] = COLOR_R32G32B32A32F(1,1,1,1);
            Color[1] = COLOR_R32G32B32A32F(1,1,1,1);
            Color[2] = COLOR_R32G32B32A32F(1,1,1,1);
          };

          unt16       PointIndex[3];
          int         MaterialNo;
          POINT2DF    UV[3];
          COLOR_R32G32B32A32F Color[3];
        };
        std::vector<FACE> Face;
      };

      std::vector<OBJECT> Object;
    };

    bool Check( const std::vector<unt08>& FileImage );
    int Load( const std::vector<unt08>& FileImage, DATA& data );
  }
}


#endif