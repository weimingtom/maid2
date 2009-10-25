#include"rendertargetscreen.h"

#include"graphicscore.h"
#include"../auxiliary/debug/warning.h"

namespace Maid
{
  /*!
      @class  RenderTargetScreen rendertargetscreen.h
      @brief  スクリーンをあらわす平面
    \n        GraphicsCoreが吸収するので単体では使わないはず。
  */


  //  こいつが解像度変更機能を持つべきかなぁ

RenderTargetScreen::RenderTargetScreen()
:m_AspectSize(0,0)
{

}

void RenderTargetScreen::Initialize()
{
  const Graphics::SPDEVICE& pDevice = GlobalPointer<GraphicsCore>::Get()->GetDevice();

  m_AspectSize = pDevice->GetSwapChainFormat().Size;

  Update();
}

void RenderTargetScreen::Finalize()
{
  m_AspectSize = SIZE2DI(0,0);

  RenderTargetBase::Clear();
}


void RenderTargetScreen::Present()
{
  const Graphics::SPDEVICE& pDevice = GlobalPointer<GraphicsCore>::Get()->GetDevice();

  pDevice->Present();

  Update();
}


void RenderTargetScreen::SetAspectSize( const SIZE2DI& size )
{
  const Graphics::SPDEVICE& pDevice = GlobalPointer<GraphicsCore>::Get()->GetDevice();

  if( size.w==0 || size.h==0 )
  {
    m_AspectSize = pDevice->GetSwapChainFormat().Size;
  }else
  {
    //  アスペクト比を固定するなら ごにょごにょする
    const SIZE2DI Screen = pDevice->GetSwapChainFormat().Size;
    const SIZE2DI Aspect = size;

    SIZE2DI NewSize;

		const float ScreenW = (float)Screen.w;
		const float ScreenH = (float)Screen.h;
		const float AspectW = (float)Aspect.w;
		const float AspectH = (float)Aspect.h;

    // AspectW:AspectH を ScreenW:ScreenH に変換する
    // AspectW*ScreenW:AspectH*ScreenW  ScreenW*AspectW:ScreenH*AspectW
    // これで横幅は同じになったので ScreenH*AspectW / AspectH*ScreenW 倍すれば縦あまりの大きさになる

    // AspectW*ScreenH:AspectH*ScreenH  ScreenW*AspectH:ScreenH*AspectH
    // これで縦幅は同じになったので ScreenW*AspectH / AspectW*ScreenH 倍すれば横あまりの大きさになる

    //  どちらにそろえるかは、画面に入りきる大きさ（＝倍率が１倍以下）のほうにする

    const float scale = (ScreenH*AspectW) / (AspectH*ScreenW);

    if( scale <=1.0f )
    {
      const int h = int(Screen.h*scale + 0.5f);

      NewSize.w = Screen.w;
      NewSize.h = h;
    }else
    {
      const float scale = ScreenW*AspectH / (AspectW*ScreenH);
      const int w = int(Screen.w*scale + 0.5f);

      NewSize.w = w;
      NewSize.h = Screen.h;
    }

    m_AspectSize = NewSize;
  }

  Update();
}

void RenderTargetScreen::Update()
{
  const Graphics::SPDEVICE& pDevice = GlobalPointer<GraphicsCore>::Get()->GetDevice();

  Graphics::SPRENDERTARGET pTarget = pDevice->GetCurrentRenderTarget();

  {
    #pragma COMPILERMSG("ここでクリアするんじゃなくて、Graphics::SPDEVICEが保障してもいいかも")	
    const Graphics::SPDRAWCOMMAND& pCommand = pDevice->GetDrawCommandExecute();
    pCommand->SetRenderTarget( 0, pTarget, Graphics::SPDEPTHSTENCIL() );
    pCommand->ClearRenderTarget( COLOR_R32G32B32A32F(0,0,0,0) );
  }
 
  const SIZE2DI Screen = pDevice->GetSwapChainFormat().Size;

  RenderTargetBase::Setup( pTarget, Screen, m_AspectSize, DRAWAREA_CENTER );
}

}