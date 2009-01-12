#include"rendertargetscreen.h"

#include"graphicscore.h"

namespace Maid
{

void RenderTargetScreen::Initialize()
{
  Graphics::SPRENDERTARGET obj = GlobalPointer<GraphicsCore>::Get()->GetDevice()->GetDefaultRenderTarget();

  RenderTargetBase::Setup( obj );
}

void RenderTargetScreen::Destroy()
{
  RenderTargetBase::Clear();

}
}