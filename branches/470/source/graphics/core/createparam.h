/*!
    @file
    @brief デバイスから作成する各種オブジェクトの初期値を定義する
 */

#ifndef maid2_graphics_core_createparam_h
#define maid2_graphics_core_createparam_h


#include"../../config/define.h"
#include"../../config/typedef.h"
#include"../../auxiliary/mathematics/size.h"
#include"../../auxiliary/mathematics/rect.h"
#include"../../auxiliary/string.h"
#include"../pixelformat.h"
#include"../color.h"

#include<vector>
#include<string>

namespace Maid { namespace Graphics {

  struct SAMPLEDESC
  {
    int Count;
    int Quality;

    SAMPLEDESC()
      :Count(1)
      ,Quality(0)
    {}
  };

  struct REFRESHRATE
  {
    int Numerator;   //!<  リフレッシュレート(分子)
    int Denominator; //!<  リフレッシュレート(分母)

    REFRESHRATE()
      :Numerator(0)
      ,Denominator(1)
    {}
  };



  struct SWAPCHAINFORMAT
  {
    SIZE2DI     Size;         //!<  画面解像度
    PIXELFORMAT Format;       //!<  スクリーンモード
    SAMPLEDESC  MultiSample;  //!<  マルチサンプル
    REFRESHRATE RefreshRate;  //!<  リフレッシュレート

    enum SCANLINEORDER
    {
      SCANLINEORDER_UNSPECIFIED,  //!<  明示しない（ドライバ依存？）
      SCANLINEORDER_PROGRESSIVE,  //!<  プログレッシブ
      SCANLINEORDER_INTERLACE_U,  //!<  upper field が先のインターレース
      SCANLINEORDER_INTERLACE_L,  //!<  lower field が先のインターレース
    };

    SCANLINEORDER ScanlineOrder;

    enum SCALING
    {
      SCALING_UNSPECIFIED,  //!<  明示しない（ドライバ依存？）
      SCALING_CENTERED,     //!<  中央に小さく表示
      SCALING_STRETCHED,    //!<  画面全体に伸ばす
    };

    SCALING Scaling;

    int SyncInterval; //!<  VSync同期の間隔(0...同期取らない 1...普通に待つ)
  };

  struct DISPLAYINFO
  {
    String  Name; //!<  ディスプレイ名
    RECT2DI Rect; //!<  ディスプレイの配置座標
  };


  struct ENABLEFORMAT
  {
    SIZE2DI MaxTextureSize;   //  作成できる限界のテクスチャサイズ
    std::vector<PIXELFORMAT>  Texture;              //!<  作成可能なテクスチャフォーマット
    std::vector<PIXELFORMAT>  RenderTargetTexture;  //!<  作成可能なレンダーテクスチャフォーマット
    std::vector<PIXELFORMAT>  RenderTargetSurface;  //!<  作成可能なレンダーサーフェスフォーマット
    std::vector<PIXELFORMAT>  DepthStencil;         //!<  作成可能なDepthStencilフォーマット
    std::vector<int>          IndexBit;             //!<  作成可能な１インデックスあたりのビット数
  };

  struct VIEWPORT
  {
    RECT2DI Screen; //!<  表示範囲
    float   MinZ;   //!<  カリングＺ
    float   MaxZ;   //!<  カリングＺ

    VIEWPORT():Screen(0,0,0,0),MinZ(0),MaxZ(1){}
    VIEWPORT( const RECT2DI& s, float min, float max ):Screen(s),MinZ(min),MaxZ(max){}
  };


  struct SUBRESOURCE
  {
    const void *pData;
    int Pitch;
    int Slice;

    SUBRESOURCE()
      :pData(NULL)
      ,Pitch(0)
      ,Slice(0)
    {}
  };
  struct MAPPEDRESOURCE
  {
    void *pData;
    int Pitch;
    int Slice;

    MAPPEDRESOURCE()
      :pData(NULL)
      ,Pitch(0)
      ,Slice(0)
    {}
  };


  enum RESOURCEUSAGE
  {                           //  CPU読み CPU書き GPU読み GPU書き
      RESOURCEUSAGE_DEFAULT,  //    x       x       o       o
      RESOURCEUSAGE_IMMUTABLE,//    x     最初だけ  o       x
      RESOURCEUSAGE_DYNAMIC,  //    x     map()で   o       o
      RESOURCEUSAGE_STAGING,  //    o       o       x       x
      RESOURCEUSAGE_GPUWRITE, //    x       x       x       o
      RESOURCEUSAGE_NONE,     //   この値はエラー
  };

  enum RESOURCEBINDFLAG
  {
      RESOURCEBINDFLAG_VERTEX   = 0x01L,
      RESOURCEBINDFLAG_INDEX    = 0x02L,
      RESOURCEBINDFLAG_CONSTANT = 0x04L,
      RESOURCEBINDFLAG_MATERIAL = 0x08L,
      RESOURCEBINDFLAG_STREAMOUTPUT = 0x10L,
      RESOURCEBINDFLAG_RENDERTARGET = 0x20L,
      RESOURCEBINDFLAG_DEPTHSTENCIL = 0x40L,
  };

  enum CPUACCESSFLAG
  {
      CPUACCESSFLAG_WRITE = 0x10000L,
      CPUACCESSFLAG_READ  = 0x20000L,
  };

  enum RESOURCEMISCFLAG
  {
      RESOURCEMISCFLAG_GENERATE_MIPS = 0x1L,
      RESOURCEMISCFLAG_SHARED = 0x2L,
      RESOURCEMISCFLAG_TEXTURECUBE = 0x4L,
      RESOURCEMISCFLAG_SHARED_KEYEDMUTEX = 0x10L,
      RESOURCEMISCFLAG_GDI_COMPATIBLE = 0x20L,
  };










  enum COMPARISON
  {
    COMPARISON_NEVER        = 1,	//!<	常に通る
    COMPARISON_LESS         = 2,	//!<	o >  n
    COMPARISON_EQUAL		    = 3,	//!<	o == n
    COMPARISON_LESSEQUAL	  = 4,	//!<	o >= n
    COMPARISON_GREATER		  = 5,	//!<	o <  n
    COMPARISON_NOTEQUAL     = 6,	//!<	o != n
    COMPARISON_GREATEREQUAL = 7,	//!<	o <= n
    COMPARISON_ALWAYS       = 8,	//!<	常にダメ
  };





  struct DEPTHSTENCILSTATEPARAM
  {
    bool DepthTest;
    COMPARISON DepthFunc;
    bool StencilTest;

    DEPTHSTENCILSTATEPARAM()
      :DepthTest(true)
      ,DepthFunc(COMPARISON_LESS)
      ,StencilTest(false)
    {}
  };



 

}}

#endif