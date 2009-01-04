/*!
	  @file
	  @brief シーンクラスを管理するクラスのテンプレ
 */

#ifndef framework_isceneadmin_h
#define framework_isceneadmin_h

#include"../config/define.h"

#include"iscene.h"
#include"../graphics/rendertargettexture.h"
#include"../graphics/graphicscore.h"
#include"../graphics/graphicsrender.h"
#include"../auxiliary/counter.h"
#include"../auxiliary/mathematics/size.h"


namespace Maid
{
  class ISceneAdmin
  {
  public:
    virtual ~ISceneAdmin(){}

    void Initialize( const GraphicsCore& core );
    void UpdateFrame();
    void UpdateDraw();

    bool IsInitializing() const;


  protected:
    virtual SPSCENE CreateFirstScene()=0; 
    virtual SPSCENE CreateNextScene( const SPSCENEOUTPUT& pOut )=0;

  private:
    void SceneUpdateFrame();
    void SceneUpdateDraw();
  private:
    const GraphicsCore* m_pCore;
    GraphicsRender  m_Render;
    SPSCENE m_pCurrentScene;
    RenderTargetTexture   m_FadeTexture;
    LinearCounter<float>  m_FadeAlpha;

    enum STATE
    {
      STATE_UPDATEFRAME,  //  普通に進行している
      STATE_CHANGING,     //  シーン切り替え中
    };

    STATE m_State;
  };
}

#endif
