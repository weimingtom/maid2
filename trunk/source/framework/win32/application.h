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

#include"../iapplication.h"

#include"osdevice.h"

namespace Maid
{
  class Application : public IApplication
  {
  public:
    Application( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );

  protected:
    IOSDevice& GetOSDevice();
  private:
    OSDevice  m_Device;
  };
}
#endif
