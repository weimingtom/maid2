#include"../../source/framework/application.h"



using namespace Maid;


class MyApp : public Application
{
protected:
  virtual void OnSetup(){}
  virtual void OnLoop()
  {
    ::MessageBox( NULL, L"test", L"aaa", MB_OK );
    ApplicationExit();
  
  }
  virtual void OnCleanup(){}
};



int WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPSTR lpCmdLine, 
    int nCmdShow
)
{
  MyApp app;
  
  return app.Run(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
}

