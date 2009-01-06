#include"rendertargetscreen.h"


namespace Maid
{

void RenderTargetScreen::Initialize()
{
  Graphics::SPRENDERTARGET obj = GlobalPointer<GraphicsCore>::Get()->GetDevice()->GetDefaultRenderTarget();

  RenderTargetBase::Setup( obj );
}


}