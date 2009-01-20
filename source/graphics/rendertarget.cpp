#include"rendertarget.h"
#include"../auxiliary/debug/warning.h"

#include"graphicscore.h"

namespace Maid
{
  /*!
      @class  RenderTarget rendertarget.h
      @brief  描画対象となる平面をあらわすクラス
  \n          描画対象なだけなので、テクスチャとしては使えません
  */



//! 指定した大きさで作る
/*!
    @param	size    [i ]	作成したい大きさ
    @param	format  [i ]	作成したいピクセルフォーマット
 */
void RenderTarget::Create( const SIZE2DI& size, PIXELFORMAT format )
{
  Destroy();

  GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get();

  const PIXELFORMAT DstFormat = pCore->FindFormatRenderTarget( format );
  if( DstFormat==PIXELFORMAT_NONE )
  {
    MAID_WARNING( MAIDTEXT("このフォーマットは対応していません ") << format );
    return ; 
  }
  Graphics::SPTEXTURE2D pTexture;

  {
    Graphics::CREATERETEXTURE2DPARAM param;

    param.Size = size;
    param.MipLevels = 0;
    param.ArraySize = 0;
    param.Format = DstFormat;
    param.Usage = Graphics::RESOURCEUSAGE_DEFAULT;
    param.BindFlags = Graphics::RESOURCEBINDFLAG_RENDERTARGET;

    pTexture = pCore->GetDevice()->CreateTexture2D( param, NULL );
    if( pTexture.get()==NULL )
    {
        MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") );
        return ; 
    }
  }
  Graphics::SPRENDERTARGET pTarget;
  {
    Graphics::CREATERENDERTARGETPARAM param;
    param.Format = DstFormat;
    param.Dimension =  Graphics::CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2D;

    pTarget = pCore->GetDevice()->CreateRenderTarget( pTexture, param );
    if( pTarget.get()==NULL )
    {
        MAID_WARNING( MAIDTEXT("テクスチャの作成に失敗 ") );
        return ; 
    }
  }
  
  RenderTargetBase::Setup( pTarget, size );
}


//! 作っていたものを開放する
/*!
 */
void RenderTarget::Destroy()
{
  RenderTargetBase::Clear();
}


}
