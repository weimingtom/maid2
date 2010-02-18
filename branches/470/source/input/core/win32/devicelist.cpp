#include"devicelist.h"
#include"mousedevicewindowsapi.h"
#include"keyborddevicewindowsapi.h"
#include"charcodedevicewindows.h"
#include"inputmethoddevicewindows.h"


namespace Maid{ namespace Input {

DeviceList::DeviceList( Window& hWnd )
 :m_Window(hWnd)
{

}


void DeviceList::Initialize()
{

}

void DeviceList::GetKeybordList( std::vector<KEYBORDINFO>& ret )
{
  KEYBORDINFO info;
  info.DeviceID = 0;
  info.Name = MAIDTEXT("WindowsAPI");

  ret.push_back( info );
}

SPKEYBORDDEVICE DeviceList::CreateKeybordDevice( int DeviceID )
{
  return SPKEYBORDDEVICE( new KeybordDeviceWindowsAPI );
}


void DeviceList::GetMouseList( std::vector<MOUSEINFO>& ret )
{
  MOUSEINFO info;
  info.DeviceID = 0;
  info.Name = MAIDTEXT("WindowsAPI");

  ret.push_back( info );
}

SPMOUSEDEVICE   DeviceList::CreateMouseDevice( int DeviceID )
{
  return SPMOUSEDEVICE( new MouseDeviceWindowsAPI(m_Window) );
}


SPCHARCODEDEVICE   DeviceList::CreateCharCodeDevice()
{
  return SPCHARCODEDEVICE( new CharCodeDeviceWindows(m_Window) );
}

SPINPUTMETHODDEVICE   DeviceList::CreateInputMethodDevice()
{
  return SPINPUTMETHODDEVICE( new InputMethodDeviceWindows(m_Window) );
}

}}

