#include"application.h"


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
  m_OSDevice.SetParam( hInstance,hPrevInstance,lpCmdLine,nCmdShow );

  return IApplication::Run();
}

IOSDevice& Application::GetOSDevice() { return m_OSDevice; }

}