#include"charcodedevicewindows.h"

namespace Maid
{
  
CharCodeDeviceWindows::CharCodeDeviceWindows( const Window& hWnd )
:m_Window(hWnd)
{

}

void CharCodeDeviceWindows::Initialize()
{
	AddProc( m_Window.GetHWND() );
}

void CharCodeDeviceWindows::Finalize()
{

}

void CharCodeDeviceWindows::Flush( String& buffer )
{
  buffer = String::ConvertSJIStoMAID(m_Buffer);
  m_Buffer.clear();
}

CharCodeDeviceWindows::RETURNCODE CharCodeDeviceWindows::OnMessage( WindowsMessage& mess )
{
	const UINT	 msg = mess.GetMSG();
	const WPARAM wParam = mess.GetWPARAM();
	const LPARAM lParam = mess.GetLPARAM();

	switch( msg )
	{
	case WM_CHAR:
		{
      const char c = (char)wParam;
      m_Buffer += c;
    }break;
	}

	return MessageHook::RETURNCODE_DEFAULT;
}

}
