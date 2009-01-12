#include"rendertargetbase.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/debug/assert.h"

namespace Maid
{


RenderTargetBase::RenderTargetBase()
  :m_Size(0,0),m_TextureSize(0,0)
{

}

void RenderTargetBase::Clear()
{
  IRenderTarget::Clear();

  m_Size = SIZE2DI(0,0);
  m_TextureSize = SIZE2DI(0,0);
}


bool RenderTargetBase::IsSetupped() const
{
  return IRenderTarget::Get().get()!=NULL;
}


void RenderTargetBase::Setup( const Graphics::SPRENDERTARGET& pTarget, const SIZE2DI& create )
{
  MAID_ASSERT( pTarget->GetResource()->GetType()!=Graphics::IResource::TYPE_TEXTURE2D,
      "設定するリソースがテクスチャではありません" );
  IRenderTarget::Set(pTarget);

  const Graphics::CREATERETEXTURE2DPARAM param = static_cast<Graphics::ITexture2D*>(pTarget->GetResource().get())->GetParam();

  m_Size = create;
  m_TextureSize = param.Size;
  m_Format = param.Format;
}

void RenderTargetBase::Setup( const Graphics::SPRENDERTARGET& pTarget )
{
  MAID_ASSERT( pTarget->GetResource()->GetType()!=Graphics::IResource::TYPE_TEXTURE2D,
      "設定するリソースがテクスチャではありません" );
  const SIZE2DI size = static_cast<Graphics::ITexture2D*>(pTarget->GetResource().get())->GetParam().Size;

  Setup( pTarget, size );
}

}

