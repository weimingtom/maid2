#include"isceneadmin.h"
#include"../graphics/depthstencil.h"


namespace Maid
{

  //  IScene も ISceneAdmin もお試し的なコードなので
  //  ゲームによってはつくりかえたほうがいいかも


//! このクラスの初期化
/*!
*/
void ISceneAdmin::Initialize()
{
  m_State = STATE_UPDATEFRAME;

  m_SceneStack.push_back( CreateFirstScene() );
}


//! 初期化が終わっているか？
/*!
*/
bool ISceneAdmin::IsInitializing() const
{
  return GetCurrentScene()->IsInitializing();
}

void ISceneAdmin::PushScene( const SPSCENE& pScene )
{
  m_SceneStack.push_front( pScene );
}

SPSCENE& ISceneAdmin::GetCurrentScene()
{
  return m_SceneStack.front();
}

const SPSCENE& ISceneAdmin::GetCurrentScene()const
{
  return m_SceneStack.front();
}

void ISceneAdmin::ExitCurrentScene()
{
  MAID_ASSERT( m_SceneStack.empty(), "シーンがありません" );

  GetCurrentScene()->ExitScene();
}

int  ISceneAdmin::GetSceneStackCount()const
{
  return m_SceneStack.size();
}



//! フレーム毎の更新
/*!
*/
void ISceneAdmin::UpdateFrame()
{
  if( m_SceneStack.empty() ) { return ; }

  switch( m_State )
  {
  case STATE_UPDATEFRAME:
    {
      SceneUpdateFrame();
    }break;

  case STATE_CHANGING:
    {
      if( IsFadeEnd() )
      {
        EndFade();
        m_State = STATE_UPDATEFRAME;
      }else
      {
        UpdateFadeFrame();
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
  if( m_SceneStack.empty() ) { return ; }

  SceneUpdateDraw(target,depth);

  if( m_State==STATE_CHANGING )
  {
    UpdateFadeDraw(target,depth);
  }
}


const ISceneAdmin::SCENESTACK& ISceneAdmin::GetSceneStack()
{
  return m_SceneStack;
}


void ISceneAdmin::SceneUpdateFrame()
{
  const SPSCENE& pCurrent = GetCurrentScene();

  if( pCurrent->IsInitializing() )
  {

  }
  else if( pCurrent->IsExit() )
  {
    //  シーンが終了していたら
    //  切り替え開始
    BeginFade();

    { //  終了させて、次のシーンを作る
      SPSCENEOUTPUT pOut;
      GetCurrentScene()->Finalize( pOut );
      m_SceneStack.pop_front();

      //  シーンがない == メインしーんの移動
      //  シーンがまだある == 割り込みシーンだった
      if( m_SceneStack.empty() )
      {
        m_SceneStack.push_back( CreateNextScene(pOut) );
      }else
      {
        pCurrent->EndInterrupt(pOut);
      }
    }

    //  切り替えシーンの開始
    m_State = STATE_CHANGING;
  }
  else if( pCurrent->IsInterruptScene() )
  {
    BeginFade();

    //  割り込み命令が出ていたら、そいつを作る
    SPSCENEINTERRUPTINPUT pIn;
    pCurrent->BeginInterrupt(pIn);
    m_SceneStack.push_front( CreateInterruptScene(pIn) );

    m_State = STATE_CHANGING;

  }else
  {
    pCurrent->UpdateFrame();
  }
}

void ISceneAdmin::SceneUpdateDraw( const RenderTargetBase& target, const IDepthStencil& depth )
{
  for( SCENESTACK::const_reverse_iterator ite=m_SceneStack.rbegin(); ite!=m_SceneStack.rend(); ++ite )
  {
    const SPSCENE& pScene = *ite;
    if( pScene->IsInitializing() ) { continue; }
    pScene->UpdateDraw( target, depth );
  }
}


void ISceneAdmin::Finalize()
{
  m_State = STATE_UPDATEFRAME;
  m_SceneStack.clear();
}


}

