#include"../../source/framework/application.h"
#include"../../source/framework/gameframework.h"



using namespace Maid;


class MyApp : public GameThread
{
protected:
    virtual void LocalSetup( APPCONFIG& conf )
    {

    }

    virtual void LocalBegin( const BEGINPARAM& param )
    {

    }

    virtual void LocalLoop()
    {

    }

    virtual void LocalEnd()
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

