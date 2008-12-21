#include"rendertargetscreen.h"


namespace Maid
{

RenderTargetScreen::RenderTargetScreen()
{
  Graphics::SPRENDERTARGET obj = GlobalPointer<Graphics::Core>::Get()->GetDevice()->GetDefaultRenderTarget();

  IRenderTarget::Set( obj );
}
	
}