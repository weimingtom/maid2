#include"rendertargetscreen.h"

#include"graphicscore.h"

namespace Maid
{

  /*!
      @class  RenderTargetScreen rendertargetscreen.h
      @brief  スクリーンをあらわす平面
    \n        GraphicsCoreが吸収するので単体では使わないはず。
  */



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