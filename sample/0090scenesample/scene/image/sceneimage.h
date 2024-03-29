#ifndef sample_0090scenesample_scane_image_sceneimage_h
#define sample_0090scenesample_scane_image_sceneimage_h

#include"../../../../source/config/define.h"
#include"../../../../source/auxiliary/globalpointer.h"
#include"../../../../source/graphics/core/ifontdevice.h"
#include"../../../../source/framework/iscene.h"

#include"../../../../source/graphics/texture2d.h"
#include"../../../../source/graphics/rendertargettexture.h"
#include"../../../../source/graphics/depthstencil.h"

class App;
class SceneImage
  :public Maid::IScene
  ,public Maid::GlobalPointer<App>
{
public:
  virtual void Initialize( const Maid::SPSCENEINPUT& pInput );
  virtual bool IsInitializing() const;
  virtual void Finalize( Maid::SPSCENEOUTPUT& pOutput );
  virtual void UpdateFrame();
  virtual void UpdateDraw( const Maid::RenderTargetBase& target, const Maid::IDepthStencil& depth  );

private:
  Maid::Texture2D  m_Texture;

  int   m_FrameCount;
};


#endif
