#include"../../../source/framework/application.h"
#include"../../../source/storage/fileio/fileoperation.h"
#include"../../../source/storage/fileio/filewrite.h"
#include"../../../source/framework/win32/shell.h"


using namespace Maid;


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
    const String CommonDeskTop     = Shell::GetCommonDeskTopFolder(NULL);
    const String CommonProgramMenu = Shell::GetCommonProgramMenuFolder(NULL);
    const String CommonAppData     = Shell::GetCommonApplicationDataFolder(NULL);
    const String UserDeskTop     = Shell::GetUserDeskTopFolder(NULL);
    const String UserProgramMenu = Shell::GetUserProgramMenuFolder(NULL);
    const String UserAppData     = Shell::GetUserApplicationDataFolder(NULL);
    const String MyDocuments = Shell::GetMyDocumentsFolder(NULL);
    const String CurrentDir  = Shell::GetCurrentDirectory();

    const String msg_text = 
      MAIDTEXT("DeskTop:\n　")     + CommonDeskTop     + MAIDTEXT("\n　") + UserDeskTop     + MAIDTEXT("\n")
     +MAIDTEXT("ProgramMenu:\n　") + CommonProgramMenu + MAIDTEXT("\n　") + UserProgramMenu + MAIDTEXT("\n")
     +MAIDTEXT("AppData:\n　")     + CommonAppData     + MAIDTEXT("\n　") + UserAppData     + MAIDTEXT("\n")
     +MAIDTEXT("MyDocuments:\n　") + MyDocuments + MAIDTEXT("\n")
     +MAIDTEXT("CurrentDir:\n　")  + CurrentDir  + MAIDTEXT("\n")
     ;

    Shell::MessageBox( NULL, msg_text, MAIDTEXT("ディレクトリ設定"), MB_OK );

    {
      FileWrite hFile;

      const String dir = CommonAppData + MAIDTEXT("\\maid_installertester");
      const String name = dir + MAIDTEXT("\\savedata.dat");

      FileOperation::CreateDirectory(dir);
      hFile.Open( name, FileWrite::OPENOPTION_NEW );

      const std::string text = String::ConvertMAIDtoSJIS(msg_text);
      hFile.Write( text.c_str(), text.length() );
    }

    OnExit(0);
	}

  virtual void Finalize()
  {

  }
};



int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  MyApp app;
  
  return app.Run(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
}
