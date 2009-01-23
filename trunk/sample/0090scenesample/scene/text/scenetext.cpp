#include"scenetext.h"

#include"../../app.h"

using namespace Maid;

void SceneText::Initialize( const SPSCENEINPUT& pInput )
{
  m_Font.Create( SIZE2DI(16,32), true );

  m_FrameCount = 0;
}

bool SceneText::IsInitializing() const
{
  return false;
}

void SceneText::Finalize( SPSCENEOUTPUT& pOutput )
{

}

void SceneText::UpdateFrame()
{
  if( 300 < m_FrameCount ) { ExitScene(); }
  ++m_FrameCount;

}

void SceneText::UpdateDraw( const RenderTargetBase& target, const IDepthStencil& depth )
{
  GraphicsRender& Render = GlobalPointer<App>::Get()->GetRender();

  Render.SetRenderTarget(target);
  Render.Begin();
  Render.BltText( POINT2DI(0,0), m_Font, MAIDTEXT("テキストを表示するシーン"), COLOR_R32G32B32A32F(1,1,1,1) );
  Render.End();
}

void SceneText::LastDraw( const RenderTargetBase& target, const IDepthStencil& depth )
{
  GraphicsRender& Render = GlobalPointer<App>::Get()->GetRender();

  Render.SetRenderTarget(target);
  Render.ClearRenderTarget( COLOR_A32B32G32R32F(0,0,0,0) );

  UpdateDraw(target,depth);
}
