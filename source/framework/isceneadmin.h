/*!
	  @file
	  @brief シーンクラスを管理するクラスのテンプレ
 */

#ifndef maid2_framework_isceneadmin_h
#define maid2_framework_isceneadmin_h

#include"../config/define.h"

#include"iscene.h"
#include"../auxiliary/globalpointer.h"
#include"../graphics/rendertargettexture.h"
#include"../auxiliary/mathematics/size.h"

#include<list>

namespace Maid
{
  class ISceneAdmin
  {
  public:
    virtual ~ISceneAdmin(){}

    void Initialize();
    void Finalize();
    void UpdateFrame();
    void UpdateDraw( const RenderTargetBase& target, const IDepthStencil& depth );

    bool IsInitializing() const;

    void PushScene( const SPSCENE& pScene );

    void ExitCurrentScene();
    int  GetSceneStackCount()const;

  protected:
    virtual SPSCENE CreateFirstScene()=0; 
    virtual SPSCENE CreateNextScene( const SPSCENEOUTPUT& pOut )=0;

    virtual void BeginFade(){}
    virtual bool IsFadeEnd(){return true;}
    virtual void EndFade(){}
    virtual void UpdateFadeFrame(){}
    virtual void UpdateFadeDraw( const RenderTargetBase& target, const IDepthStencil& depth ){}

    typedef std::list<SPSCENE> SCENESTACK;

    const SCENESTACK& GetSceneStack();
    const SPSCENE& GetCurrentScene()const;

  private:
    void SceneUpdateFrame();
    void SceneUpdateDraw( const RenderTargetBase& target, const IDepthStencil& depth );
    SPSCENE& GetCurrentScene();


  private:
    SCENESTACK m_SceneStack; //  .front()==現在実行中のシーン（一番手前)

    enum STATE
    {
      STATE_UPDATEFRAME,  //  普通に進行している
      STATE_CHANGING,     //  シーン切り替え中
    };

    STATE m_State;
  };
}

#endif
