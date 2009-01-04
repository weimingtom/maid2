#ifndef sample_0090scenesample_scane_text_scenetext_h
#define sample_0090scenesample_scane_text_scenetext_h

#include"../../../../source/config/define.h"
#include"../../../../source/auxiliary/globalpointer.h"
#include"../../../../source/graphics/font.h"
#include"../../../../source/framework/iscene.h"

class App;
class SceneText
  :public Maid::IScene
  ,public Maid::GlobalPointer<App>
{
public:
  virtual void Initialize( const Maid::SPSCENEINPUT& pInput );
  virtual bool IsInitializing() const;
  virtual void Finalize( Maid::SPSCENEOUTPUT& pOutput );
  virtual void UpdateFrame();
  virtual void UpdateDraw();
  virtual void LastDraw( Maid::IRenderTarget& target, Maid::IDepthStencil& depth );

private:
  Maid::Font  m_Font;

  int   m_FrameCount;
};


#endif
