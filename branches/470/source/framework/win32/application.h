/*!
    @file
    @brief Windows用フレームワークベースクラス
 */

#ifndef maid2_framework_win32_application_h
#define maid2_framework_win32_application_h

#include"../../config/define.h"
#include"../../config/Win32.h"

#include<vector>

#include"../../auxiliary/globalpointer.h"
#include"../../auxiliary/string.h"

#include"../iapplication.h"

#include"osdevice.h"

namespace Maid
{
  class Application : public IApplication
  {
  public:
    int Run( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );

  protected:
    IOSDevice& GetOSDevice();
  private:
    OSDevice  m_OSDevice;
  };
}
#endif
