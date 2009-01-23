/*!
	  @file
	  @brief シーンクラスを管理するクラスのテンプレ
 */

#ifndef framework_isceneadmin_h
#define framework_isceneadmin_h

#include"../config/define.h"

#include"iscene.h"
#include"../auxiliary/globalpointer.h"
#include"../graphics/rendertargettexture.h"
#include"../graphics/rendertargetscreen.h"
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

    void Initialize( const SIZE2DI& FadeSize, PIXELFORMAT TargetFMT, PIXELFORMAT DepthFMT );
    void UpdateFrame();
    void UpdateDraw( const RenderTargetBase& target, const IDepthStencil& depth );

    bool IsInitializing() const;


  protected:
    virtual SPSCENE CreateFirstScene()=0; 
    virtual SPSCENE CreateNextScene( const SPSCENEOUTPUT& pOut )=0;

    virtual void BeginFade()=0;
    virtual bool IsFadeEnd()=0;
    virtual void UpdateFadeFrame()=0;
    virtual void UpdateFadeDraw( const Texture2DBase& target )=0;

  private:
    void SceneUpdateFrame();
    void SceneUpdateDraw( const RenderTargetBase& target, const IDepthStencil& depth );

  private:
    SPSCENE m_pCurrentScene;
    RenderTargetTexture   m_FadeTexture;

    enum STATE
    {
      STATE_UPDATEFRAME,  //  普通に進行している
      STATE_CHANGING,     //  シーン切り替え中
    };

    STATE m_State;

    SIZE2DI m_FadeSize;
    PIXELFORMAT m_TargetFormat;
    PIXELFORMAT m_DepthFormat;
  };
}

#endif
