#include"isceneadmin.h"


namespace Maid
{

void ISceneAdmin::Initialize()
{
  m_pCurrentScene = CreateFirstScene();
}

void ISceneAdmin::UpdateFrame()
{
  //  シーンが終了していたら
  //  最後の描画をフェードアウト用に使う
  if( m_pCurrentScene->IsExit() )
  {

    //RenderTargetScreen

  }

}

void ISceneAdmin::UpdateDraw()
{

}

/*
  class ISceneAdmin
  {
  public:
    virtual ~ISceneAdmin(){}

    void UpdateFrame();
    void UpdateDraw();

  protected:
    virtual void OnEndScene( const SPSCENEOUTPUT& pOut, SPSCENE& pNext )=0;

  private:
    SPSCENE m_pCurrentScene;
  };
*/
}

