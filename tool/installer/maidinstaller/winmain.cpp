#include"../../../source/framework/application.h"
#include"../../../source/framework/win32/shell.h"

#include"resource.h"
#include"../installprogram.h"
#include"../define.h"
#include"install.h"

#include <shlobj.h>
#include <shellapi.h>

using namespace Maid;

static BOOL CALLBACK InstallStateProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );


Install s_InstallThread;
INSTALLPROGRAM  s_InstallProgram;

static HWND s_ProgressHWND;		//	インストール進行具合ダイアログのHWND
static const int PROGRESSMAX = 1000;


void SetProgressText( const String& text )
{
	const std::wstring t = String::ConvertMAIDtoUNICODE(text);

	wchar_t buf[1024];
	GetDlgItemText( s_ProgressHWND, STATIC_NOWSTATE, buf, NUMELEMENTS(buf) );
	if( t!=buf ) { ::SetDlgItemText( s_ProgressHWND, STATIC_NOWSTATE, t.c_str() ); }
}

void SetNowProgress( float per )
{
	const int v = int(per*float(PROGRESSMAX)+0.5f);

	::SendMessage( GetDlgItem( s_ProgressHWND, PROGRESS_NOWSTATE  ), PBM_SETPOS,   v,   0 );
}

void SetTotalProgress( float per )
{
	const int v = int(per*float(PROGRESSMAX)+0.5f);

	::SendMessage( GetDlgItem( s_ProgressHWND, PROGRESS_TOTALSTATE  ), PBM_SETPOS,   v,   0 );
}









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
		const String InstallProgramFileName = os.GetCmdLine(1);


    {
      const FUNCTIONRESULT ret = LoadInstallProgram( InstallProgramFileName, s_InstallProgram );
      if( ret==FUNCTIONRESULT_ERROR )
      {
			  ::MessageBox( NULL, L"設定ファイルが読み込みません", L"起動エラー", MB_OK );
			  OnExit(s_INSTALLSTATE_ERROR);
        return;
      }
    }


		const DWORD ret = ::DialogBox( os.GetHINSTANCE(), MAKEINTRESOURCE(ID_DIALOG_INSTALLSTATE), NULL, InstallStateProc );

	  OnExit(ret);
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
//! インストール状況を知らせるダイアログボックス
/*!	
 *	\param	hWnd	[i ]	呼び出し元のウィンドウハンドル
 *	\param	msg		[i ]	送られてきたメッセージ
 *	\param	wParam	[i ]	メッセージの詳細
 *	\param	lParam	
 */
static BOOL CALLBACK InstallStateProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	const int TIMERID = 100;

	switch(msg)
	{
	case WM_TIMER:
		{
			switch( wParam )
			{
			case TIMERID:
				{
					Install::STATUS s = s_InstallThread.GetStatus();

					SetProgressText( s.ProgressText );
					SetNowProgress( s.NowProgress );
					SetTotalProgress( s.TotalProgress );

					switch( s.Status )
					{
					case Install::STATUS::THREADSTATUS_EXECUTING:
						{
						}break;
					case Install::STATUS::THREADSTATUS_ERROR:
						{ 
							::KillTimer( hWnd, TIMERID );

							::MessageBox( hWnd, L"エラーが発生しました", L"エラー", MB_ICONSTOP|MB_OK );
							::EndDialog( hWnd, s_INSTALLSTATE_ERROR );
						}break;

					case Install::STATUS::THREADSTATUS_SUCCESS:
						{ 
							::KillTimer( hWnd, TIMERID );

							::MessageBox( hWnd, L"インストールが終了しました", L"インストールの成功", MB_OK );

							::EndDialog( hWnd, s_INSTALLSTATE_SUCCESS );
						}break;

					case Install::STATUS::THREADSTATUS_CANCEL: 
						{ 
							::KillTimer( hWnd, TIMERID );

							::MessageBox( hWnd, L"キャンセルしました", L"キャンセル", MB_ICONINFORMATION|MB_OK );
							::EndDialog( hWnd, s_INSTALLSTATE_CANCEL );
						}break;
					}
				}
				break;
			}
		}
		break;

	case WM_COMMAND:
		{
			switch( LOWORD(wParam) )
			{
			case IDC_BUTTON1:
				{
					const int ret = ::MessageBox( hWnd, L"インストールを中断しますか？", L"確認", MB_YESNO );
					if( ret==IDYES ) { s_InstallThread.OnCancel(); }
				}
				break;
		   }
		}
		break;

	case WM_INITDIALOG:
		{
			s_ProgressHWND = hWnd;

			SendMessage( GetDlgItem( hWnd, PROGRESS_NOWSTATE), PBM_SETRANGE, 0, MAKELPARAM( 0, PROGRESSMAX ) );
			SendMessage( GetDlgItem( hWnd, PROGRESS_NOWSTATE), PBM_SETSTEP,  1,   0 );
			SendMessage( GetDlgItem( hWnd, PROGRESS_NOWSTATE), PBM_SETPOS,   0,   0 );

			SendMessage( GetDlgItem( hWnd, PROGRESS_TOTALSTATE), PBM_SETRANGE, 0, MAKELPARAM( 0, PROGRESSMAX ) );
			SendMessage( GetDlgItem( hWnd, PROGRESS_TOTALSTATE), PBM_SETSTEP,  1,   0 );
			SendMessage( GetDlgItem( hWnd, PROGRESS_TOTALSTATE), PBM_SETPOS,   0,   0 );

			try
			{
        Install::INSTALLDATA dat( &s_InstallProgram, hWnd );
				s_InstallThread.BeginInstall( dat );
			}catch(...)
			{
				::MessageBox( hWnd, L"インストールに失敗しました", NULL, MB_OK );
				::EndDialog( hWnd, s_INSTALLSTATE_ERROR );
				return 0;
			}

			SetTimer( hWnd, TIMERID, 100, NULL );
		}
		break;

	case WM_CLOSE:
		{
			::SendMessage( hWnd, WM_COMMAND, IDCANCEL, 0 );
		}
		break;
  }

	return 0;
}
