#include"isceneadmin.h"
#include"../graphics/depthstencil.h"


namespace Maid
{

void ISceneAdmin::Initialize( const GraphicsCore& core )
{
  m_Render.Initialize( core );
  m_pCurrentScene = CreateFirstScene();

  m_State = STATE_UPDATEFRAME;
  m_pCore = &core;
}

bool ISceneAdmin::IsInitializing() const
{
  return m_Render.IsInitializing() || m_pCurrentScene->IsInitializing();
}

void ISceneAdmin::UpdateFrame()
{
  switch( m_State )
  {
  case STATE_UPDATEFRAME:
    {
      //  シーンが終了していたら
      //  切り替え開始
      if( m_pCurrentScene->IsExit() )
      {
        const SIZE2DI size = m_pCore->GetScreenMode().Size;
        DepthStencil Depth;
        {
          //  最後の描画をフェードアウト用に保存
          m_FadeTexture.Create( size, PIXELFORMAT_X08R08G08B08I );
          Depth.CreateCompatible( m_FadeTexture, PIXELFORMAT_D24IS8 );
          m_pCurrentScene->LastDraw( m_FadeTexture, Depth );
          m_FadeAlpha.Set( 1.0f, 0.0f, 36 );
        }

        { //  終了させて、次のシーンを作る
          SPSCENEOUTPUT pOut;
          m_pCurrentScene->Finalize( pOut );
          m_pCurrentScene.reset();

          m_pCurrentScene = CreateNextScene(pOut);
        }
        m_State = STATE_CHANGING;
      }else
      {
        SceneUpdateFrame();
      }
    }break;

  case STATE_CHANGING:
    {
      if( m_FadeAlpha.IsEnd() )
      {
        m_FadeTexture.Destroy();
        m_State = STATE_UPDATEFRAME;
      }else
      {
        ++m_FadeAlpha;
      }
      SceneUpdateFrame();
    }break;
  }

}

void ISceneAdmin::UpdateDraw()
{
  SceneUpdateDraw();

  if( m_State==STATE_CHANGING )
  {
    const SIZE2DI size = m_FadeTexture.GetDefaultSize();

    m_Render.Begin();
    const POINT2DI pos(size.w/2,size.h/2);
    const RECT2DI  rc( POINT2DI(0,0), size );
    const POINT2DI center(size.w/2,size.h/2);
    const float alpha = m_FadeAlpha;
    m_Render.Blt( pos, m_FadeTexture, rc, center, alpha );
    m_Render.End();
  }
}


void ISceneAdmin::SceneUpdateFrame()
{
  if( m_pCurrentScene->IsInitializing() ) { return ; }
  m_pCurrentScene->UpdateFrame();
}

void ISceneAdmin::SceneUpdateDraw()
{
  if( m_pCurrentScene->IsInitializing() ) { return ; }
  m_pCurrentScene->UpdateDraw();
}

}

