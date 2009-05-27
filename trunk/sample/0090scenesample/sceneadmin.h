#ifndef sample_0090scenesample_sceneadmin_h
#define sample_0090scenesample_sceneadmin_h

#include"../../source/config/define.h"

#include"../../source/framework/isceneadmin.h"
#include"../../source/auxiliary/counter.h"


class App;
class SceneAdmin 
  : public Maid::ISceneAdmin
  , public Maid::GlobalPointer<App>

{
public:

protected:
  virtual Maid::SPSCENE CreateFirstScene(); 
  virtual Maid::SPSCENE CreateNextScene( const Maid::SPSCENEOUTPUT& pOut );
  virtual Maid::SPSCENE CreateInterruptScene( const Maid::SPSCENEINTERRUPTINPUT& pIn );

  virtual void BeginFade();
  virtual bool IsFadeEnd();
  virtual void EndFade();
  virtual void UpdateFadeFrame();
  virtual void UpdateFadeDraw( const Maid::RenderTargetBase& target, const Maid::IDepthStencil& depth );

private:
  Maid::LinearCounter<float>  m_FadeAlpha;

  Maid::RenderTargetTexture m_ScreenShot;
};

#endif



