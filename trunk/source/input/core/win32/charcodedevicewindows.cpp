#include"charcodedevicewindows.h"

namespace Maid{ namespace Input {

  
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
  if( m_Buffer.empty() ) { return ; }
  m_Buffer.push_back('\0');
  buffer = String::ConvertUNICODEtoMAID(&(m_Buffer[0]));
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
      const wchar_t c = (wchar_t)wParam;
      m_Buffer.push_back(c);
    }break;
	}

	return MessageHook::RETURNCODE_DEFAULT;
}

}}
