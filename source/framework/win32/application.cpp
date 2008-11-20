#include"application.h"
#include"osdriverwin32.h"


namespace Maid
{

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 実行
/*!
 	  @param	hInstance		  [i ]	WinMain() から渡された引数
 	  @param	hPrevInstance	[i ]	WinMain() から渡された引数
 	  @param	lpCmdLine		  [i ]	WinMain() から渡された引数
 	  @param	nCmdShow		  [i ]	WinMain() から渡された引数
 */
int Application::Run( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  SetOSDriver( SPOSDRIVER(new OSDriverWin32(hInstance,hPrevInstance,lpCmdLine,nCmdShow)) );

  return IApplication::Run();
}

}