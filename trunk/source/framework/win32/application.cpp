#include"application.h"


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
Application::Application( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
  :m_Device(hInstance,hPrevInstance,lpCmdLine,nCmdShow)
{
}


IOSDevice& Application::GetOSDevice() { return m_Device; }

}