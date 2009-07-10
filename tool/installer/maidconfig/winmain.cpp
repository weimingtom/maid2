#include"../../../source/framework/application.h"
#include"../../../source/framework/win32/shell.h"

#include"resource.h"
#include"setupconfig.h"

#include <shlobj.h>
#include <shellapi.h>
#include <map>

using namespace Maid;

static BOOL CALLBACK InstallConfigDlgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );


//	チェックボックスのあたり判定
//	この中に入ると HELP_TEXT のテキストが IDC_STATIC_HELP に表示される
static const DWORD HELP_CONTROLID[] =
{
	COMBBOX_INSTALLTYPE,
	BUTTON_BEGININSTALL,
	EDIT_INSTALLDIR,
	BUTTON_SELECTINSTALLDIR,
	CHECK_MD5CHECK,
	CHECK_CREATEDESKTOPSHOTCUT,
};

static const wchar_t* HELP_TEXT[] =
{
	L"",	//	<- 設定ファイルで更新するので変化しない
	L"インストールを開始します",
	L"インストールするフォルダです\nフォルダが存在しない場合、自動的に作成されます",
	L"インストールするフォルダを変更します",
	L"インストール中にファイルが破損していないかチェックします",
	L"スタートメニューの他にデスクトップにショートカットを作成します",
};

typedef std::map<HWND,const wchar_t*> HELPLIST;
static HELPLIST	s_Control;		//	HELP_CONTROLID,HELP_TEXT の関係を持っているやつ
static HWND		s_NowControl;	//	現在ヘルプを表示しているコントロール


SetupConfig s_SetupConfig;


class MyApp 
  : public Application
{
protected:
  virtual bool BootCheck()
  {

    return true;
  }

  virtual void Initialize()
  {

  }

	virtual void OnLoop()
	{
    const OSDevice& os = static_cast<OSDevice&>(GetOSDevice());
		const String InstallConfigFileName = os.GetCmdLine(1);


    {
      const FUNCTIONRESULT ret = s_SetupConfig.Initialize( InstallConfigFileName );
      if( ret==FUNCTIONRESULT_ERROR )
      {
			  ::MessageBox( NULL, L"設定ファイルが読み込みません", L"起動エラー", MB_OK );
			  OnExit(0);
        return;
      }
    }

		::DialogBox( os.GetHINSTANCE(), MAKEINTRESOURCE(DIALOG_INSTALLCONFIG), NULL, InstallConfigDlgProc );


	  OnExit(0);
	}

  virtual void Finalize()
  {

  }
};











int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  MyApp app;
	return app.Run( hInstance, hPrevInstance, lpCmdLine, nCmdShow );
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! パックファイル管理クラス
/*!	
    @param  hWnd    [i ]	呼び出し元のウィンドウハンドル
    @param  msg     [i ]	送られてきたメッセージ
    @param  wParam  [i ]	メッセージの詳細
    @param  lParam	
 */
static BOOL CALLBACK InstallConfigDlgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_INITDIALOG:
		{
			{	//	ダイアログボックスを画面中央に持ってくる
				RECT rc;
				::GetWindowRect( hWnd, &rc );

				const int w = rc.right  - rc.left;
				const int h = rc.bottom - rc.top;
				const int x = (::GetSystemMetrics(SM_CXSCREEN) - w) /2;
				const int y = (::GetSystemMetrics(SM_CYSCREEN) - h) /2;

				::MoveWindow( hWnd, x, y, w, h, TRUE );
			}


			{	//	リストボックスにインストール項目を追加する
				const HWND hCombBox = ::GetDlgItem(hWnd, COMBBOX_INSTALLTYPE);

				for( int i=0; i<s_SetupConfig.GetInstallTypeCount(); ++i )
				{
          const SetupConfig::INSTALLTYPE& type = s_SetupConfig.GetInstallType(i);
					const std::wstring str = String::ConvertMAIDtoUNICODE(type.Name);
					::SendMessage( hCombBox, CB_INSERTSTRING, (WPARAM)i, (LPARAM)str.c_str() );
				}

				::SendMessage( hCombBox, CB_SETCURSEL, 0, 0L);
			}

			{	//	デフォルトのインストール先の設定
				const std::wstring str = String::ConvertMAIDtoUNICODE(s_SetupConfig.GetDefaultTarget());

				::SetDlgItemText( hWnd, EDIT_INSTALLDIR, str.c_str() );
			}

			{	//	コントロールハンドルからヘルプテキストを参照できるようにしておく
				//	ただしそのコントロールが使えるものだけに限定する
				for( int i=0; i<NUMELEMENTS(HELP_CONTROLID); ++i )
				{
					const HWND hwnd  = GetDlgItem(hWnd,HELP_CONTROLID[i]);
					const LONG Style = ::GetWindowLong(hwnd,GWL_STYLE);
					if( !(Style&WS_DISABLED) )
					{
						s_Control[hwnd] = HELP_TEXT[i];
					}
				}
			}

			//	ＭＤ５チェックはデフォルトでＯＮ
			::CheckDlgButton( hWnd, CHECK_MD5CHECK, BST_CHECKED );
		}
		break;

	case WM_COMMAND:
		{
			switch( LOWORD(wParam) )
			{
			case BUTTON_BEGININSTALL:
				{
					String InstallFolder;

					{
						wchar_t buf[MAX_PATH];
						::GetDlgItemText( hWnd, EDIT_INSTALLDIR, buf, MAX_PATH );
						InstallFolder = String::ConvertUNICODEtoMAID(buf);
						while( true )
						{
							const unt32 c = InstallFolder[InstallFolder.length()-1];
							if( (c!='\\' && c!='/') || InstallFolder.empty() ) { break; }

							InstallFolder.resize( InstallFolder.length()-1 );
						}

						if( InstallFolder.empty() )
						{
							::MessageBox( hWnd, L"インストールするディレクトリ名が不明です", NULL, MB_OK );
							return 0;
						}
					}

					const int32 ComboNo = (int)(DWORD)SendMessage(GetDlgItem(hWnd, COMBBOX_INSTALLTYPE), CB_GETCURSEL, 0L, 0L);

/*
					s_SetupConfig.SetInstallFolder(InstallFolder);

					s_InstallData.pConfig = &s_SetupConfig;
					s_InstallData.InstallTypeNo = ComboNo;
					s_InstallData.IsMD5Check = ::IsDlgButtonChecked(hWnd,CHECK_MD5CHECK)==BST_CHECKED;

					::DialogBox( s_hInstance, MAKEINTRESOURCE(DIALOG_INSTALLSTATE), hWnd, InstallStateProc );
*/
					::EndDialog( hWnd, 0 );

				}break;
			case BUTTON_SELECTINSTALLDIR:
				{
					wchar_t buf[MAX_PATH];
					::GetDlgItemText( hWnd, EDIT_INSTALLDIR, buf, MAX_PATH );

          const String Folder = Maid::Shell::BrowseForFolder( hWnd, MAIDTEXT("インストールフォルダの選択"), String::ConvertUNICODEtoMAID(buf) );

					if( Folder.empty() ) { break; }

					{
						const std::wstring FolderName = String::ConvertMAIDtoUNICODE(Folder);
						::SetDlgItemText( hWnd, EDIT_INSTALLDIR, FolderName.c_str() );
					}
				}break;
			}
		}
		break;	

	case WM_CLOSE:
		{
			::EndDialog( hWnd, 0 );
		}
		break;

	case WM_SETCURSOR:
		{
			const HWND hwnd		= (HWND)wParam;
			const UINT MouseMSG = HIWORD(lParam);
			const int  HitCode  = LOWORD(lParam);

			switch( MouseMSG )
			{
			case WM_MOUSEMOVE:
				{
					//	各コントロールの上に乗っていたら
					//	ヘルプを表示する
					HELPLIST::iterator ite = s_Control.find(hwnd);

					if( ite!=s_Control.end()  )
					{
						if( s_NowControl!=hwnd )
						{
							if( hwnd == GetDlgItem(hWnd,COMBBOX_INSTALLTYPE) )
							{	//	リストボックスの時は設定ファイルのテキストを拾ってくる
								const int ComboNo = (int)(DWORD)SendMessage(GetDlgItem(hWnd, COMBBOX_INSTALLTYPE), CB_GETCURSEL, 0L, 0L);

								const SetupConfig::INSTALLTYPE type = s_SetupConfig.GetInstallType(ComboNo);
								const std::wstring str = String::ConvertMAIDtoUNICODE(type.Comment);

								SetWindowText( GetDlgItem(hWnd,STATIC_HELPTEXT), str.c_str() );
							}else
							{
								SetWindowText( GetDlgItem(hWnd,STATIC_HELPTEXT), ite->second );
							}
						}
						s_NowControl = hwnd;
					}
					else
					{
						SetWindowText( GetDlgItem(hWnd,STATIC_HELPTEXT), NULL );
						s_NowControl = NULL;
					}
				}break;
			}

		}break;
	}

	return 0;
}


