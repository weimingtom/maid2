#include"../../source/framework/application.h"



using namespace Maid;


class MyApp : public Application
{
protected:
  virtual void LocalSetup()
  {
		//	とりあえずウィンドウの作成
		//	ウィンドウのｘボタンを押したらアプリが終了するようにする
		m_Window.RegistCallBack( WM_CLOSE, MakeCallBackObject(&MyApp::OnClose,this) );
		m_Window.Create();
		m_Window.Show();
  }

  virtual void LocalLoop()
  {
  
  }
  virtual void LocalCleanup(){}

private:
	Window::CALLBACKCODE OnClose( WindowsMessage& msg )
	{
		OnExit(0);
		return Window::CALLBACKCODE_CONTINUE;
	}

private:
  Window  m_Window;
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

