#include"isceneadmin.h"
#include"../graphics/depthstencil.h"


namespace Maid
{

//! このクラスの初期化
/*!
*/
void ISceneAdmin::Initialize()
{
  m_Render.Initialize();
  m_pCurrentScene = CreateFirstScene();

  m_State = STATE_UPDATEFRAME;
}


//! 初期化が終わっているか？
/*!
*/
bool ISceneAdmin::IsInitializing() const
{
  return m_Render.IsInitializing() || m_pCurrentScene->IsInitializing();
}


//! フレーム毎の更新
/*!
*/
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
        const SIZE2DI size = GlobalPointer<GraphicsCore>::Get()->GetScreenMode().Format.Size;

        DepthStencil Depth;
        {
          //  最後の描画をフェードアウト用に保存
          m_FadeTexture.Create( size, PIXELFORMAT_X08R08G08B08I );
          Depth.CreateCompatible( m_FadeTexture, PIXELFORMAT_D24IS8 );
          m_Render.SetRenderTarget(m_FadeTexture);
          m_Render.ClearRenderTarget( COLOR_A32B32G32R32F(0,0,0,0) );
          m_Render.ClearDepthStencil( 1, 0 );
          m_pCurrentScene->LastDraw( m_FadeTexture, Depth );
          m_FadeAlpha.Set( 1.0f, 0.0f, 120 );
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

//! フレーム毎の描画
/*!
    @param  target  [i ]  描画先
    @param  depth   [i ]  描画先
*/
void ISceneAdmin::UpdateDraw( const RenderTargetBase& target, const IDepthStencil& depth )
{
  SceneUpdateDraw(target,depth);

  if( m_State==STATE_CHANGING )
  {
    #pragma COMPILERMSG("TODO:この描画部分は継承先クラスでやってもらうほうがパターン作れていいかもしれない")	
    m_Screen.Update();

    m_Render.SetRenderTarget(m_Screen);

    m_Render.Begin();
    const SIZE2DI size = static_cast<Texture2DBase&>(m_FadeTexture).GetSize();
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

void ISceneAdmin::SceneUpdateDraw( const RenderTargetBase& target, const IDepthStencil& depth )
{
  if( m_pCurrentScene->IsInitializing() ) { return ; }
  m_pCurrentScene->UpdateDraw(target,depth);
}

}

