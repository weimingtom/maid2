#include"texture2ddynamic.h"
#include"../auxiliary/debug/warning.h"


namespace Maid
{

void Texture2DDynamic::Create( const SIZE2DI& Size, PIXELFORMAT fmt )
{
  GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get();

  SIZE2DI RealSize;
  SIZE2DI TextureSize;

  pCore->CalcTextureSize( Size, RealSize, TextureSize );

  if( Size.w <= RealSize.w && Size.h <= RealSize.h )
  { //  画像より作成するサイズの方が大きいなら引数どおりの大きさにする
    RealSize = Size;
  }

  const PIXELFORMAT DstFormat = pCore->FindFormatTexture2D( fmt, false );
  if( DstFormat==PIXELFORMAT_NONE )
  {
    MAID_WARNING( MAIDTEXT("このフォーマットは対応していません ") << fmt );
    return ; 
  }


  Graphics::SPTEXTURE2D pTexture;

  {
    Graphics::CREATERETEXTURE2DPARAM param;

    param.Size = TextureSize;
    param.MipLevels = 1;
    param.ArraySize = 1;
    param.Format = DstFormat;
    param.Usage = Graphics::RESOURCEUSAGE_DYNAMIC;
    param.BindFlags = Graphics::RESOURCEBINDFLAG_MATERIAL;
    param.CPUAccessFlags = Graphics::CPUACCESSFLAG_WRITE;

    pTexture = pCore->GetDevice()->CreateTexture2D( param, NULL );
    if( pTexture.get()==NULL )
    {
      MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") );
      return ; 
    }
  }

  Graphics::SPMATERIAL pMaterial;
  {
    pMaterial = pCore->GetDevice()->CreateMaterial( pTexture, NULL );
    if( pMaterial.get()==NULL )
    {
      MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") );
      return ; 
    }
  }

  Setup( pTexture, pMaterial, Size, RealSize, fmt );

}

}