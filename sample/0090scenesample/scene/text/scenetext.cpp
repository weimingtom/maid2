#include"scenetext.h"

#include"../../app.h"

using namespace Maid;

void SceneText::Initialize( const SPSCENEINPUT& pInput )
{
  App* pApp = GlobalPointer<App>::Get();

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
  if( 600 < m_FrameCount ) { ExitScene(); }
  ++m_FrameCount;

}

void SceneText::UpdateDraw()
{
  GraphicsRender& Render = GlobalPointer<App>::Get()->GetRender();

  Render.Begin();
  Render.BltText( POINT2DI(0,0), m_Font, MAIDTEXT("テキストを表示するシーン"), COLOR_R32G32B32A32F(1,1,1,1) );
  Render.End();
}

void SceneText::LastDraw( IRenderTarget& target, IDepthStencil& depth )
{

}
