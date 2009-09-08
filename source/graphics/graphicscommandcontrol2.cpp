#include"graphicscommandcontrol.h"
#include"../auxiliary/debug/warning.h"
#include"transiter.h"


namespace Maid
{

struct YUVtoRGBINFO
{
  SIZE2DI SrcSizeY;
  int SrcPitchY;
  const unt08* pSrcY;

  SIZE2DI SrcSizeUV;
  int SrcPitchUV;
  const unt08* pSrcU;
  const unt08* pSrcV;

  SIZE2DI DstSize;
  int DstPitch;
  PIXELFORMAT DstFormat;
  void* pDst;
};


template<class CONVERTER>
void YUVtoRGB( const YUVtoRGBINFO& info, const CONVERTER& conv )
{
  //  yuv の変換はここみてね
  //  http://vision.kuee.kyoto-u.ac.jp/~hiroaki/firewire/yuv.html
  //  http://d.hatena.ne.jp/Ko-Ta/20090124

  const SIZE2DI& yplane = info.SrcSizeY;
  const int ypitch = info.SrcPitchY;
  const unt08* PixelY = info.pSrcY;

  const SIZE2DI& uvplane = info.SrcSizeUV;
  const int uvpitch   = info.SrcPitchUV;
  const unt08* PixelU = info.pSrcU;
  const unt08* PixelV = info.pSrcV;

  const int bpp = GetPixelBPP(info.DstFormat);
  const int PixelPitch = bpp / 8;

  const SIZE2DI& DestSize = info.DstSize;

  //  ソースのサイズとテクスチャの実サイズが（ライブラリの設定で）違うことがあるので
  //  縮小する必要がある
  //  本当はフィルタかけたいけど、手間なのでしない

  for( int dst_y=0; dst_y<DestSize.h; ++dst_y )
  {
    const int y_y  = dst_y * yplane.h  / DestSize.h;
    const int uv_y = dst_y * uvplane.h / DestSize.h;

    const unt08* pSrcLineY = PixelY + ypitch  * y_y;
    const unt08* pSrcLineU = PixelU + uvpitch * uv_y;
    const unt08* pSrcLineV = PixelV + uvpitch * uv_y;
    unt08* pDstLine = (unt08*)(info.pDst) + (dst_y*info.DstPitch);

    //  X軸の移動は固定小数点で行う。8bitの精度で大丈夫...?
    const unt FIXBIT = 8;
    const unt32 delta_y_x  = ( yplane.w << FIXBIT) / DestSize.w;
    const unt32 delta_uv_x = (uvplane.w << FIXBIT) / DestSize.w;
    unt32 fix_y_x = 0;
    unt32 fix_uv_x = 0;

    for( int dst_x=0; dst_x<DestSize.w; ++dst_x )
    {
      const unt32 y_x  = fix_y_x >> FIXBIT;
      const unt32 uv_x = fix_uv_x>> FIXBIT;

      int y = pSrcLineY[y_x ];
      int u = pSrcLineU[uv_x];
      int v = pSrcLineV[uv_x];

      //  32bitギリギリなので注意
      y=(y-16 )*76608 >>8;    //Y成分を16～235→0～65535に伸縮
      u=(u-128)*74896 >>8;    //U成分をCbに変換16-240 -> -32767-32767
      v=(v-128)*74896 >>8;    //V成分をCrに変換

      const int FLOATBIT = 13;

      int r = (y*8192          +v*11485) >> FLOATBIT; //R = Y            + 1.40200V
      int g = (y*8192 -u* 2819 -v* 5850) >> FLOATBIT; //G = Y - 0.34414U - 0.71414V
      int b = (y*8192 +u*14516         ) >> FLOATBIT; //B = Y + 1.77200U
      //0～65535の閾値処理
      if( r < 0 ) { r=0; }else if( 65535<r ) { r = 65535; }
      if( g < 0 ) { g=0; }else if( 65535<g ) { g = 65535; }
      if( b < 0 ) { b=0; }else if( 65535<b ) { b = 65535; }

      CONVERTER* dst = (CONVERTER*)(pDstLine + (dst_x*PixelPitch));

      dst->SetA(0xFFFF);
      dst->SetR(r);
      dst->SetG(g);
      dst->SetB(b);

      fix_y_x += delta_y_x;
      fix_uv_x+= delta_uv_x;
    }
  }
}


void YUVtoRGB( const YUVtoRGBINFO& info )
{
  switch( info.DstFormat )
  {
  case PIXELFORMAT_R08G08B08I:    { YUVtoRGB(info,COLOR_R08G08B08I()); }break;
  case PIXELFORMAT_A08R08G08B08I: { YUVtoRGB(info,COLOR_A08R08G08B08I()); }break;
  case PIXELFORMAT_A08B08G08R08I: { YUVtoRGB(info,COLOR_A08B08G08R08I()); }break;
  case PIXELFORMAT_X08R08G08B08I: { YUVtoRGB(info,COLOR_X08R08G08B08I()); }break;

  case PIXELFORMAT_A02R10G10B10I: { YUVtoRGB(info,COLOR_A02R10G10B10I()); }break;
  case PIXELFORMAT_A02B10G10R10I: { YUVtoRGB(info,COLOR_A02B10G10R10I()); }break;

  case PIXELFORMAT_R05G06B05I:    { YUVtoRGB(info,COLOR_R05G06B05I()); }break;
  case PIXELFORMAT_X01R05G05B05I: { YUVtoRGB(info,COLOR_X01R05G05B05I()); }break;
  case PIXELFORMAT_A01R05G05B05I: { YUVtoRGB(info,COLOR_A01R05G05B05I()); }break;
  case PIXELFORMAT_A01B05G05R05I: { YUVtoRGB(info,COLOR_A01B05G05R05I()); }break;
  case PIXELFORMAT_A04R04G04B04I: { YUVtoRGB(info,COLOR_A04R04G04B04I()); }break;

  default:
    {
      MAID_ASSERT( true, "対応してません" );
    }break;
  }
}




void GraphicsCommandControl::UpdateTexture( Texture2DDynamic& dst, const SPSAMPLEIMAGE& pSrc )
{
  MAID_ASSERT( pSrc.get()==NULL, "バッファがありません" );
  if( pSrc.get()==NULL ) { return ; }

  ITexture2D& tex = dst;

  const int sub = 0;
  Graphics::MAPPEDRESOURCE map;

  Graphics::IDrawCommand& com = *m_pDrawCommand;

  com.ResourceMap( tex.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map );
  {
    YUVtoRGBINFO info;

    info.SrcSizeY = pSrc->SizeY;
    info.SrcPitchY = pSrc->PitchY;
    info.pSrcY = &(pSrc->BitsY[0]);
    info.SrcSizeUV = pSrc->SizeUV;
    info.SrcPitchUV = pSrc->PitchUV;
    info.pSrcU = &(pSrc->BitsU[0]);
    info.pSrcV = &(pSrc->BitsV[0]);

    info.DstSize = dst.GetRealSize();
    info.DstPitch = map.Pitch;
    info.DstFormat = dst.GetRealFormat();
    info.pDst = map.pData;

    YUVtoRGB( info );
  }
  com.ResourceUnmap( tex.Get(), sub );
}





























struct SURFACEINFO
{
  SIZE2DI SrcSize;
  int     SrcPitch;
  PIXELFORMAT SrcFormat;
  const void* pSrc;

  SIZE2DI DstSize;
  int     DstPitch;
  PIXELFORMAT DstFormat;
  void*   pDst;
};


template<class SRCPIXEL, class DSTPIXEL>
void RGBtoRGB( const SURFACEINFO& info, const SRCPIXEL& SrcPixel, const DSTPIXEL& DstPixel )
{
  const int SrcPixelSize = GetPixelBPP(info.SrcFormat) / 8;
  const int DstPixelSize = GetPixelBPP(info.DstFormat) / 8;

  const int width  = std::min( info.SrcSize.w, info.DstSize.w );
  const int height = std::min( info.SrcSize.h, info.DstSize.h );

  for( int y=0; y<height; ++y )
  {
    const unt08* pSrc = ((unt08*)info.pSrc) + info.SrcPitch*y;
    const unt08* pDst = ((unt08*)info.pDst) + info.DstPitch*y;
    for( int x=0; x<width; ++x )
    {
      const SRCPIXEL& src = *(SRCPIXEL*)pSrc;
            DSTPIXEL& dst = *(DSTPIXEL*)pDst;

      dst.SetA( src.GetA() );
      dst.SetR( src.GetR() );
      dst.SetG( src.GetG() );
      dst.SetB( src.GetB() );

      pSrc += SrcPixelSize;
      pDst += DstPixelSize;
    }
  }
}



void GraphicsCommandControl::UpdateTexture( Texture2DDynamic& dst, const SurfaceInstance& Src )
{
  ITexture2D& tex = dst;

  const int sub = 0;
  Graphics::MAPPEDRESOURCE map;

  Graphics::IDrawCommand& com = *m_pDrawCommand;

  com.ResourceMap( tex.Get(), sub, Graphics::IDrawCommand::MAPTYPE_WRITE_DISCARD, 0, map );

  SURFACEINFO info;

  info.SrcSize = Src.GetSize();
  info.SrcPitch= Src.GetPitch();
  info.SrcFormat=Src.GetPixelFormat();
  info.pSrc     = Src.GetPlanePTR();

  info.DstSize = dst.GetRealSize();
  info.DstPitch= map.Pitch;
  info.DstFormat=dst.GetRealFormat();
  info.pDst     =map.pData;


  int kk=0;
#define PIXELCOPY( dstpixel ) \
switch( info.SrcFormat )  \
{                         \
case PIXELFORMAT_R08G08B08I:    { RGBtoRGB(info, COLOR_R08G08B08I(), dstpixel() ); }break;  \
case PIXELFORMAT_B08G08R08I:    { RGBtoRGB(info, COLOR_B08G08R08I(), dstpixel() ); }break;  \
case PIXELFORMAT_A08R08G08B08I: { RGBtoRGB(info, COLOR_A08R08G08B08I(), dstpixel() ); }break;  \
case PIXELFORMAT_A08B08G08R08I: { RGBtoRGB(info, COLOR_A08B08G08R08I(), dstpixel() ); }break;  \
case PIXELFORMAT_X08R08G08B08I: { RGBtoRGB(info, COLOR_X08R08G08B08I(), dstpixel() ); }break;  \
case PIXELFORMAT_A02R10G10B10I: { RGBtoRGB(info, COLOR_A02R10G10B10I(), dstpixel() ); }break;  \
case PIXELFORMAT_A02B10G10R10I: { RGBtoRGB(info, COLOR_A02B10G10R10I(), dstpixel() ); }break;  \
case PIXELFORMAT_R05G06B05I:    { RGBtoRGB(info, COLOR_R05G06B05I(),    dstpixel() ); }break;  \
case PIXELFORMAT_X01R05G05B05I: { RGBtoRGB(info, COLOR_X01R05G05B05I(), dstpixel() ); }break;  \
case PIXELFORMAT_A01R05G05B05I: { RGBtoRGB(info, COLOR_A01R05G05B05I(), dstpixel() ); }break;  \
case PIXELFORMAT_A01B05G05R05I: { RGBtoRGB(info, COLOR_A01B05G05R05I(), dstpixel() ); }break;  \
case PIXELFORMAT_A04R04G04B04I: { RGBtoRGB(info, COLOR_A04R04G04B04I(), dstpixel() ); }break;  \
}  \


  switch( info.DstFormat )
  {
  case PIXELFORMAT_R08G08B08I:    { PIXELCOPY( COLOR_R08G08B08I ); }break;
  case PIXELFORMAT_B08G08R08I:    { PIXELCOPY( COLOR_B08G08R08I ); }break;
  case PIXELFORMAT_A08R08G08B08I: { PIXELCOPY( COLOR_A08R08G08B08I ); }break;
  case PIXELFORMAT_A08B08G08R08I: { PIXELCOPY( COLOR_A08B08G08R08I ); }break;
  case PIXELFORMAT_X08R08G08B08I: { PIXELCOPY( COLOR_X08R08G08B08I ); }break;
  case PIXELFORMAT_A02R10G10B10I: { PIXELCOPY( COLOR_A02R10G10B10I ); }break;
  case PIXELFORMAT_A02B10G10R10I: { PIXELCOPY( COLOR_A02B10G10R10I ); }break;
  case PIXELFORMAT_R05G06B05I:    { PIXELCOPY( COLOR_R05G06B05I    ); }break;
  case PIXELFORMAT_X01R05G05B05I: { PIXELCOPY( COLOR_X01R05G05B05I ); }break;
  case PIXELFORMAT_A01R05G05B05I: { PIXELCOPY( COLOR_A01R05G05B05I ); }break;
  case PIXELFORMAT_A01B05G05R05I: { PIXELCOPY( COLOR_A01B05G05R05I ); }break;
  case PIXELFORMAT_A04R04G04B04I: { PIXELCOPY( COLOR_A04R04G04B04I ); }break;

  default:
    {
      MAID_ASSERT( true, "対応してません" );
    }break;
  }

#undef PIXELCOPY

  com.ResourceUnmap( tex.Get(), sub );
}


void GraphicsCommandControl::CopyRenderTarget( SurfaceInstance& dst, const RenderTargetBase& SrcRT )
{
  GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get();
  Graphics::IDrawCommand& com = *m_pDrawCommand;

  Graphics::SPTEXTURE2D pSysTex;
  const IRenderTarget& VideoRT = SrcRT;

  {
    Graphics::ITexture2D* pTex = static_cast<Graphics::ITexture2D*>(VideoRT.Get()->GetResource().get());

    const Graphics::CREATERETEXTURE2DPARAM& SrcParam = pTex->GetParam();
    Graphics::CREATERETEXTURE2DPARAM param;

    param.Size = SrcParam.Size;
    param.MipLevels = SrcParam.MipLevels;
    param.ArraySize = SrcParam.ArraySize;
    param.Format = SrcParam.Format;
    param.Usage = Graphics::RESOURCEUSAGE_STAGING;
    param.BindFlags = 0;
    param.CPUAccessFlags = Graphics::CPUACCESSFLAG_WRITE | Graphics::CPUACCESSFLAG_READ;

    pSysTex = pCore->GetDevice()->CreateTexture2D( param, NULL );
    if( pSysTex.get()==NULL )
    {
      MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") );
      return ; 
    }
  }

  com.CopyResource( pSysTex, VideoRT.Get()->GetResource() );


  //  ここまでで、VRAM からSystemMemory に転送が終わった
  //  あとは src が持っている情報どおりに再設定するだけ

  dst.Create( SrcRT.GetSize(), SrcRT.GetFormat() );


  const int sub = 0;
  Graphics::MAPPEDRESOURCE map;
  com.ResourceMap( pSysTex, sub, Graphics::IDrawCommand::MAPTYPE_READ, 0, map );
  {
    SurfaceReference  src;
    src.Create( map.pData, NULL, SrcRT.GetSize(), SrcRT.GetFormat(), map.Pitch );

    const RECT2DI SrcRECT = SrcRT.CalcDrawArea();
    const RECT2DI DstRECT( POINT2DI(0,0), SrcRT.GetSize() );

    Transiter::Bilinear( src, SrcRECT, dst, DstRECT );
  }

  com.ResourceUnmap( pSysTex, sub );

}

}