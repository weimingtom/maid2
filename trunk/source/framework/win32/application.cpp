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
Application::Application( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
  :m_Device(hInstance,hPrevInstance,lpCmdLine,nCmdShow)
{
}


IOSDevice& Application::GetOSDevice() { return m_Device; }

}