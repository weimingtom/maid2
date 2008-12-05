#include"../../source/framework/application.h"
#include"../../source/framework/gameframework.h"

using namespace Maid;


class MyApp : public IGameThread
{
protected:
  virtual bool LocalSelectDevice( const std::vector<Graphics::IDeviceList::INFO>& GraphicsDevice, DEFAULTCONFIG& conf )
    {

      return true;
    }

    virtual void LocalInitialize( const BEGINPARAM& param )
    {

    }

    virtual void LocalLoop()
    {

    }

    virtual void LocalFinalize()
    {

    }
};



int WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, 
    int nCmdShow
)
{
  GameFrameWork app( SPGAMETHREAD(new MyApp) );
  
  return app.Run(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
}

