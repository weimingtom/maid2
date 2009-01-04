#ifndef sample_0090scenesample_app_h
#define sample_0090scenesample_app_h

#include"../../source/config/define.h"
#include"../../source/framework/gameframework.h"
#include"../../source/framework/igamethread.h"
#include"../../source/graphics/graphicsrender.h"

#include"../../source/graphics/texture2d.h"

#include"../../source/auxiliary/globalpointer.h"

#include"sceneadmin.h"

class App 
  :public Maid::IGameThread
  ,public Maid::GlobalPointer<App>
{
public:
  Maid::GraphicsRender& GetRender() { return m_Render; }

protected:
  bool SelectDevice( const DEVICELIST& DeviceList, Maid::IGameThread::DEFAULTCONFIG& conf );
  void Initialize();
  void UpdateFrame();
  void UpdateDraw();
  void Finalize();

private:

  enum STATE
  {
    STATE_INITIALIZING,
    STATE_PLAY,
  };

  STATE m_State;

private:
  Maid::GraphicsRender  m_Render;
  SceneAdmin  m_SceneAdmin;
};


#endif
