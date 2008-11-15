#include"../../source/framework/application.h"
#include"../../source/framework/gameframework.h"

#include"../../source/graphic/color/color_a01r05g05b05i.h"
#include"../../source/graphic/color/color_a04r04g04b04i.h"


using namespace Maid;


class MyApp : public GameThread
{
protected:
    virtual void LocalSetup( APPCONFIG& conf )
    {
      COLOR_A01R05G05B05I a;
      COLOR_A04R04G04B04I b;

      a = b;
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

