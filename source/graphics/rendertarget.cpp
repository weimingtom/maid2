#include"rendertarget.h"
#include"../auxiliary/debug/warning.h"

#include"graphicscore.h"

namespace Maid
{

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

void RenderTarget::Destroy()
{
  RenderTargetBase::Clear();

}


}
