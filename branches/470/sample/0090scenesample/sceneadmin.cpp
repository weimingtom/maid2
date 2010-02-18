#include"sceneadmin.h"
#include"scene/text/scenetext.h"
#include"scene/image/sceneimage.h"
#include"app.h"

using namespace Maid;


void SceneAdmin::CreateFirstScene()
{
  SPSCENEINPUT pInput;
  SPSCENE pScene( new SceneText );

  pScene->Initialize( pInput );

  ISceneAdmin::PushScene( pScene );
}

void SceneAdmin::NextScene()
{
  //  こんなとこにstatic あるけど、本当は App にゲームルーチンなクラスを持たせて
  //  そいつが行うべき

  static int count;

  ++count;

  SPSCENEINPUT pInput;
  SPSCENE pScene;

  switch( count%2 )
  {
  case 0: { pScene.reset( new SceneText ); }break;
  case 1: { pScene.reset( new SceneImage ); }break;
  }

  pScene->Initialize( pInput );
  ISceneAdmin::SetScene( pScene );
}



void SceneAdmin::BeginFade()
{
  DepthStencil  dp;

  m_FadeAlpha.Set( 1.0f, 0.0f, 120 );
  m_ScreenShot.Create( SIZE2DI(800,600), PIXELFORMAT_R08G08B08I );

  dp.CreateCompatible( m_ScreenShot, PIXELFORMAT_D32I );

  GraphicsCommandControl& Command = GlobalPointer<App>::Get()->GetCommand();
  Graphics2DRender& Render = GlobalPointer<App>::Get()->GetRender();

  Command.SetRenderTarget( m_ScreenShot, dp );
  Command.ClearRenderTarget();
  Command.ClearDepthStencil();

  const SCENESTACK& stack = GetSceneStack();
  for( SCENESTACK::const_reverse_iterator ite=stack.rbegin(); ite!=stack.rend(); ++ite )
  {
    const SPSCENE& pScene = *ite;

    pScene->UpdateDraw( m_ScreenShot, dp );
  }
}

bool SceneAdmin::IsFadeEnd()
{
  return m_FadeAlpha.IsEnd();
}

void SceneAdmin::EndFade()
{
  m_ScreenShot.Destroy();
}


void SceneAdmin::UpdateFadeFrame()
{
  ++m_FadeAlpha;
}


void SceneAdmin::UpdateFadeDraw( const RenderTargetBase& target, const IDepthStencil& depth )
{
  GraphicsCommandControl& Command = GlobalPointer<App>::Get()->GetCommand();
  Graphics2DRender& Render = GlobalPointer<App>::Get()->GetRender();

  Command.SetRenderTarget( target );


  Command.Begin();
  const RECT2DI rect( POINT2DI(0,0), target.GetSize() );
  const float alpha = m_FadeAlpha;
  Render.SetBlendState( Graphics2DRender::BLENDSTATE_ALPHA );
  Render.Blt( POINT2DI(0,0), m_ScreenShot, rect, POINT2DI(0,0), alpha );
  Command.End();
}
