#include"stdafx.h"

#include"startupdialog.h"

#include"resource.h"

using namespace Maid;

BOOL CALLBACK DialogProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );





static const IGameThread::DEVICELIST*     s_pDeviceList;
static IGameThread::DEFAULTCONFIG*  s_pConfig;



RETURNCODE	BeginStartupInfo( const IGameThread::DEVICELIST& DeviceList, IGameThread::DEFAULTCONFIG& conf )
{
  s_pDeviceList = &DeviceList;
  s_pConfig = &conf;

	return (RETURNCODE)::DialogBox( NULL, MAKEINTRESOURCE(ID_DIALOG_HARDWARECONFIG), NULL, DialogProc );
}










BOOL CALLBACK DialogProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_INITDIALOG:
		{
      { //  ビデオの設定
        const std::vector<Graphics::IDeviceList::INFO>& info = s_pDeviceList->Graphics;

        const HWND hCombBox = GetDlgItem(hWnd, ID_COMBO_GRAPHICS);
        for( int i=0; i<(int)info.size(); ++i )
        {
          const std::wstring text = String::ConvertMAIDtoUNICODE(info[i].Name);
          SendMessage( hCombBox, CB_INSERTSTRING, (WPARAM)i, (LPARAM)text.c_str());
        }
        ::SendMessage( hCombBox, CB_SETCURSEL, 0, 0L);
      }
      { //  サウンドの設定
        const std::vector<Sound::IDeviceList::INFO>& info = s_pDeviceList->Sound;

        const HWND hCombBox = GetDlgItem(hWnd, ID_COMBO_SOUND);
        for( int i=0; i<(int)info.size(); ++i )
        {
          const std::wstring text = String::ConvertMAIDtoUNICODE(info[i].Name);
          SendMessage( hCombBox, CB_INSERTSTRING, (WPARAM)i, (LPARAM)text.c_str());
        }
        ::SendMessage( hCombBox, CB_SETCURSEL, 0, 0L);
      }
      { //  キーボード
        const std::vector<Input::IDeviceList::KEYBORDINFO>& info = s_pDeviceList->Keybord;

        const HWND hCombBox = GetDlgItem(hWnd, ID_COMBO_KEYBORD);
        for( int i=0; i<(int)info.size(); ++i )
        {
          const std::wstring text = String::ConvertMAIDtoUNICODE(info[i].Name);
          SendMessage( hCombBox, CB_INSERTSTRING, (WPARAM)i, (LPARAM)text.c_str());
        }
        ::SendMessage( hCombBox, CB_SETCURSEL, 0, 0L);
      }
      { //  キーボード
        const std::vector<Input::IDeviceList::MOUSEINFO>& info = s_pDeviceList->Mouse;

        const HWND hCombBox = GetDlgItem(hWnd, ID_COMBO_MOUSE);
        for( int i=0; i<(int)info.size(); ++i )
        {
          const std::wstring text = String::ConvertMAIDtoUNICODE(info[i].Name);
          SendMessage( hCombBox, CB_INSERTSTRING, (WPARAM)i, (LPARAM)text.c_str());
        }
        ::SendMessage( hCombBox, CB_SETCURSEL, 0, 0L);
      } 
    }break;

	case WM_COMMAND:
		{
			switch( LOWORD(wParam) )
			{
			case IDOK:
				{
          {
					  const int ComboNo = (int)SendMessage(GetDlgItem(hWnd, ID_COMBO_GRAPHICS), CB_GETCURSEL, 0L, 0L);
            const std::vector<Graphics::IDeviceList::INFO>& info = s_pDeviceList->Graphics;

            s_pConfig->Graphics.DeviceID = info[ComboNo].DeviceID;
          }
          {
					  const int ComboNo = (int)SendMessage(GetDlgItem(hWnd, ID_COMBO_SOUND), CB_GETCURSEL, 0L, 0L);
            const std::vector<Sound::IDeviceList::INFO>& info = s_pDeviceList->Sound;

            s_pConfig->Sound.DeviceID = info[ComboNo].DeviceID;
          }

          {
					  const int ComboNo = (int)SendMessage(GetDlgItem(hWnd, ID_COMBO_KEYBORD), CB_GETCURSEL, 0L, 0L);
            const std::vector<Input::IDeviceList::KEYBORDINFO>& info = s_pDeviceList->Keybord;

            s_pConfig->Input.KeybordDeviceID = info[ComboNo].DeviceID;
          }

          {
					  const int ComboNo = (int)SendMessage(GetDlgItem(hWnd, ID_COMBO_MOUSE), CB_GETCURSEL, 0L, 0L);
            const std::vector<Input::IDeviceList::MOUSEINFO>& info = s_pDeviceList->Mouse;

            s_pConfig->Input.MouseDeviceID = info[ComboNo].DeviceID;
          }

					::EndDialog( hWnd, GAMESTART );
				}
				break;

			case IDCANCEL:{ ::EndDialog( hWnd, APPEXIT ); } break;
			}
		}
		break;

	case WM_CLOSE:
		{
			::EndDialog( hWnd, APPEXIT );
		}
		break;
  }
	return 0;
}

