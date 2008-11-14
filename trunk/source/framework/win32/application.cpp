#include"application.h"
#include"osdriverwin32.h"


namespace Maid
{

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! Às
/*!
 	  @param	hInstance		  [i ]	WinMain() ‚©‚ç“n‚³‚ê‚½ˆø”
 	  @param	hPrevInstance	[i ]	WinMain() ‚©‚ç“n‚³‚ê‚½ˆø”
 	  @param	lpCmdLine		  [i ]	WinMain() ‚©‚ç“n‚³‚ê‚½ˆø”
 	  @param	nCmdShow		  [i ]	WinMain() ‚©‚ç“n‚³‚ê‚½ˆø”
 */
int Application::Run( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  SetOSDriver( SPOSDRIVER(new OSDriverWin32(hInstance,hPrevInstance,lpCmdLine,nCmdShow)) );

  return ApplicationBase::Run();
}

}