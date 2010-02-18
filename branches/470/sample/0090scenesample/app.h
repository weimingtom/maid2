#ifndef sample_0090scenesample_app_h
#define sample_0090scenesample_app_h

#include"../../source/config/define.h"
#include"../../source/framework/gameframework.h"
#include"../../source/framework/igamethread.h"
#include"../../source/graphics/graphics2drender.h"
#include"../../source/graphics/graphicscommandcontrol.h"

#include"../../source/graphics/texture2d.h"

#include"../../source/auxiliary/globalpointer.h"

#include"sceneadmin.h"

class App 
  :public Maid::IGameThread
  ,public Maid::GlobalPointer<App>
{
public:
  App();
  Maid::GraphicsCommandControl& GetCommand() { return m_Command; }
  Maid::Graphics2DRender& GetRender() { return m_Render; }

protected:
  bool SelectDevice( const DEVICELIST& DeviceList, DEFAULTCONFIG& conf );
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
  Maid::GraphicsCommandControl  m_Command;
  Maid::Graphics2DRender  m_Render;
  SceneAdmin  m_SceneAdmin;
};


#endif
