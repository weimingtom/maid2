/*!
    @file
    @brief Windows用フレームワークベースクラス
 */

#ifndef framework_win32_application_h
#define framework_win32_application_h

#include"../../config/define.h"
#include"../../config/Win32.h"

#include<vector>

#include"../../auxiliary/globalpointer.h"
#include"../../auxiliary/string.h"

#include"../applicationbase.h"


namespace Maid
{
  class Application : public ApplicationBase
	{
	public:
    int Run( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );
  };
}
#endif
