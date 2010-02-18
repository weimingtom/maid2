#include"depthstencil.h"
#include"../auxiliary/debug/warning.h"
#include"rendertargetbase.h"

#include"graphicscore.h"

namespace Maid
{
	/*!
	 	@class	DepthStencil depthstencil.h
	 	@brief	DepthStencil
	 */ 



//! 指定したRenderTargetと同じ大きさのDepthStencilを作成する
/*!
    指定したフォーマットでないものが作成されることもある

    @param tex    [i ]  RenderTarget
    @param format [i ]  作成するフォーマット
 */
void DepthStencil::CreateCompatible( const RenderTargetBase& tex, PIXELFORMAT format )
{
  const SIZE2DI size = tex.GetTextureSize();
  Create( size, format );
}


//! 指定した大きさのDepthStencilを作成する
/*!
    指定したフォーマットでないものが作成されることもある

    @param size   [i ]  作成するサイズ
    @param format [i ]  作成するフォーマット
 */
void DepthStencil::Create( const SIZE2DI& size, PIXELFORMAT format )
{
  Destroy();

  GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get();

  const PIXELFORMAT DstFormat = pCore->FindFormatDepthStencil( format );
  if( DstFormat==PIXELFORMAT_NONE )
  {
    MAID_WARNING( MAIDTEXT("このフォーマットは対応していません ") << format );
    return ; 
  }

//  const SIZE2DI TexSize = pCore->CalcTextureSize( size );
  const SIZE2DI TexSize = size;

  Graphics::SPTEXTURE2D pTexture;

  {
    Graphics::CREATERETEXTURE2DPARAM param;

    param.Size = TexSize;
    param.MipLevels = 1;
    param.ArraySize = 1;
    param.Format = DstFormat;
    param.Usage = Graphics::RESOURCEUSAGE_DEFAULT;
    param.BindFlags = Graphics::RESOURCEBINDFLAG_DEPTHSTENCIL;

    pTexture = pCore->GetDevice()->CreateTexture2D( param, NULL );
    if( pTexture.get()==NULL )
    {
        MAID_WARNING( MAIDTEXT("DepthStencilの作成に失敗 ") );
        return ; 
    }
  }

  Graphics::SPDEPTHSTENCIL pTarget;
  {
    Graphics::CREATEDEPTHSTENCILPARAM param;
    param.Format = DstFormat;
    param.Dimension =  Graphics::CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE2D;

//    pTarget = pCore->GetDevice()->CreateDepthStencil( pTexture, param );
    pTarget = pCore->GetDevice()->CreateDepthStencil( pTexture, NULL );
    if( pTarget.get()==NULL )
    {
        MAID_WARNING( MAIDTEXT("DepthStencilの作成に失敗 ") );
        return ; 
    }
  }
  
  IDepthStencil::Set( pTarget );
}

//! 設定を開放する
/*!
 */
void DepthStencil::Destroy()
{
  IDepthStencil::Clear();
}

}