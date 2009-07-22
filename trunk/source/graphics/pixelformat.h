#ifndef maid2_graphics_pixelformat_h
#define maid2_graphics_pixelformat_h

/*!
    @file
    @brief  各種ピクセルフォーマットで使いそうな関数
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"


namespace Maid
{
  /*!
    @enum PIXELFORMAT pixelformat.h
    @brief  サーフェスのピクセルフォーマットを表すenum
   */
  enum PIXELFORMAT
  {
    PIXELFORMAT_NONE=0,	//!<	不明　大概はエラー


    //  unt 08bit/pixel
    PIXELFORMAT_P08X08R08G08B08I,
    PIXELFORMAT_P08A08R08G08B08I,

    //  unt 16bit/pixel
    PIXELFORMAT_R05G06B05I,
    PIXELFORMAT_X01R05G05B05I,
    PIXELFORMAT_A01R05G05B05I,
    PIXELFORMAT_A01B05G05R05I,
    PIXELFORMAT_A04R04G04B04I,

    //  unt 24bit/pixel
    PIXELFORMAT_R08G08B08I,
    PIXELFORMAT_B08G08R08I,

    //  unt 32bit/pixel
    PIXELFORMAT_A08R08G08B08I,
    PIXELFORMAT_X08R08G08B08I,
    PIXELFORMAT_A08B08G08R08I,
    PIXELFORMAT_A02R10G10B10I,
    PIXELFORMAT_A02B10G10R10I,

    //  unt 64bit/pixel
    PIXELFORMAT_A16B16G16R16I,

    //  float 64bit/pixel
    PIXELFORMAT_A16B16G16R16F,

    //  float 128bit/pixel
    PIXELFORMAT_A32B32G32R32F,


    // カラーフォーマットが増えた場合はここに追加
    //	↓の GetPixelBPP() にも追加しておくこと






    PIXELFORMAT_DXT1,	//!<	DirectX Texture Compression Format 1
    PIXELFORMAT_DXT2,	//!<	DirectX Texture Compression Format 2
    PIXELFORMAT_DXT3,	//!<	DirectX Texture Compression Format 3
    PIXELFORMAT_DXT4,	//!<	DirectX Texture Compression Format 4
    PIXELFORMAT_DXT5,	//!<	DirectX Texture Compression Format 5

    // 圧縮カラーフォーマットが増えた場合はここに追加
    //	↓の GetPixelBPP() にも追加しておくこと

    // depthstencilが増えた場合はここに追加
    //	↓の GetPixelBPP() にも追加しておくこと
    PIXELFORMAT_D15IS1, //!<	整数16bit/Pixel のうち、 15Bitの ZBuffer 1Bit のステンシルバッファ
    PIXELFORMAT_D16I,   //!<	整数16bit/Pixel の ZBuffer
    PIXELFORMAT_D32I,   //!<	整数32bit/Pixel の ZBuffer
    PIXELFORMAT_D24IX8, //!<	整数32bit/Pixel (有効範囲24Bit) の ZBuffer
    PIXELFORMAT_D24IS8, //!<	整数32bit/Pixel のうち、 24Bitの ZBuffer 8Bit のステンシルバッファ
    PIXELFORMAT_D24IX4S4, //!<	整数32bit/Pixel のうち、 24Bitの ZBuffer 4Bit のステンシルバッファ

    PIXELFORMAT_D24FS8, //!<	32bit/Pixel のうち、 小数24Bitの ZBuffer 8Bit のステンシルバッファ
  };


  /*!
    @brief  ピクセルフォーマットから１ピクセルあたりに使用する
  \n        ビット数を求める
   
    @param  Format	[i ]	求めるフォーマット
   
    @return	求めたビット数
   */
  inline int GetPixelBPP( PIXELFORMAT Format )
  {
    switch( Format )
    {
    case PIXELFORMAT_NONE:			{ return  0; }break;

    case PIXELFORMAT_P08X08R08G08B08I:	{ return  8; }break;
    case PIXELFORMAT_P08A08R08G08B08I:	{ return  8; }break;

    case PIXELFORMAT_R05G06B05I:      { return 16; }break;
    case PIXELFORMAT_A04R04G04B04I:		{ return 16; }break;
    case PIXELFORMAT_X01R05G05B05I:		{ return 16; }break;
    case PIXELFORMAT_A01R05G05B05I:		{ return 16; }break;
    case PIXELFORMAT_A01B05G05R05I:		{ return 16; }break;

    case PIXELFORMAT_R08G08B08I:      { return 24; }break;
    case PIXELFORMAT_B08G08R08I:      { return 24; }break;

    case PIXELFORMAT_A08R08G08B08I:		{ return 32; }break;
    case PIXELFORMAT_A08B08G08R08I:		{ return 32; }break;
    case PIXELFORMAT_X08R08G08B08I:		{ return 32; }break;
    case PIXELFORMAT_A02R10G10B10I:		{ return 32; }break;
    case PIXELFORMAT_A02B10G10R10I:		{ return 32; }break;

    case PIXELFORMAT_A16B16G16R16I:		{ return 64; }break;

    case PIXELFORMAT_A16B16G16R16F:		{ return 64; }break;

    case PIXELFORMAT_A32B32G32R32F:		{ return 128; }break;


    case PIXELFORMAT_DXT1:			{ return  4; }break;
    case PIXELFORMAT_DXT2:			{ return  8; }break;
    case PIXELFORMAT_DXT3:			{ return  8; }break;
    case PIXELFORMAT_DXT4:			{ return  8; }break;
    case PIXELFORMAT_DXT5:			{ return  8; }break;

    }
    MAID_ASSERT( true, "不明なフォーマットです " << Format );
    return 0;
  }

  /*!
    @brief  現在の情報を維持したまま、アルファ付フォーマットを決定する
   
    @param  src	[i ]	求めるフォーマット
   
    @return	見つかったフォーマット
   */
  inline PIXELFORMAT ConvertAlphaFormat( PIXELFORMAT src )
  {
    switch( src )
    {
    case PIXELFORMAT_R05G06B05I:
    case PIXELFORMAT_X01R05G05B05I:
    case PIXELFORMAT_A01R05G05B05I:
    case PIXELFORMAT_A01B05G05R05I: { return  PIXELFORMAT_A08R08G08B08I; }break;

    case PIXELFORMAT_R08G08B08I:
    case PIXELFORMAT_B08G08R08I:
    case PIXELFORMAT_X08R08G08B08I: { return  PIXELFORMAT_A08R08G08B08I; }break;

    }
    return src;
  }

  /*!
    @brief	ピクセルフォーマットからCLUT１つあたりあたりに使用する
  \n        ビット数を求める
   
    @param	Format	[i ]	求めるフォーマット
   
    @return	求めたビット数
   */
  inline int GetCLUTBPP( PIXELFORMAT Format )
  {
    switch( Format )
    {
    case PIXELFORMAT_P08X08R08G08B08I:	{ return  32; }break;
    case PIXELFORMAT_P08A08R08G08B08I:	{ return  32; }break;
    }
    return 0;
  }

  /*!
    @brief	ピクセルフォーマットからパレットのインデックス数を求める
   
    @param	Format	[i ]	求めるフォーマット
   
    @return	インデックス数
   */
  inline int GetCLUTIndexCount( PIXELFORMAT Format )
  {
    switch( Format )
    {
    case PIXELFORMAT_P08X08R08G08B08I:	{ return  256; }break;
    case PIXELFORMAT_P08A08R08G08B08I:	{ return  256; }break;
    }
    return 0;
  }

  inline int GetCLUTbyte( PIXELFORMAT fmt )
  {
    return GetCLUTBPP(fmt) * GetCLUTIndexCount(fmt) / 8;
  }

  /*!
    @brief	圧縮テクスチャか調べる
   
    @param	Format	[i ]	調べるフォーマット
   
    @return	圧縮テクスチャであったら true
   */
  inline bool IsCompressSurfaceBuffer( PIXELFORMAT Format )
  {
    switch( Format )
    {
    case PIXELFORMAT_DXT1:
    case PIXELFORMAT_DXT2:
    case PIXELFORMAT_DXT3:
    case PIXELFORMAT_DXT4:
    case PIXELFORMAT_DXT5:
      { return true; }break;
    }

    return false;
  }

  /*!
    @brief	αピクセルを含むか調べる
   
    @param	Format	[i ]	調べるフォーマット
   
    @return	αピクセルがあるなら true
   */
  inline bool IsAlphaPixel( PIXELFORMAT Format )
  {
    switch( Format )
    {
    case PIXELFORMAT_A04R04G04B04I:
    case PIXELFORMAT_A01R05G05B05I:
    case PIXELFORMAT_A08R08G08B08I:
    case PIXELFORMAT_A08B08G08R08I:
    case PIXELFORMAT_A02R10G10B10I:
    case PIXELFORMAT_A02B10G10R10I:
    case PIXELFORMAT_A16B16G16R16I:
    case PIXELFORMAT_P08A08R08G08B08I:
    case PIXELFORMAT_DXT2:
    case PIXELFORMAT_DXT3:
    case PIXELFORMAT_DXT4:
    case PIXELFORMAT_DXT5: { return true; }break;
    }
    return false;
  }

  inline bool WithStencil( PIXELFORMAT fmt )
  {
    switch( fmt )
    {
    case PIXELFORMAT_D15IS1:
    case PIXELFORMAT_D24IS8:
    case PIXELFORMAT_D24FS8:
      { return true; }break;
    }
    return false;
  }

}


#endif
