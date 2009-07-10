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
    const String DeskTop     = Shell::GetUserDeskTopFolder(NULL);
    const String ProgramMenu = Shell::GetUserProgramMenuFolder(NULL);
    const String AppData     = Shell::GetUserApplicationDataFolder(NULL);
    const String MyDocuments = Shell::GetUserMyDocumentsFolder(NULL);
    const String CurrentDir  = Shell::GetCurrentDirectory();

    const String msg_text = 
      MAIDTEXT("DeskTop:\n　")     + DeskTop     + MAIDTEXT("\n")
     +MAIDTEXT("ProgramMenu:\n　") + ProgramMenu + MAIDTEXT("\n")
     +MAIDTEXT("AppData:\n　")     + AppData     + MAIDTEXT("\n")
     +MAIDTEXT("MyDocuments:\n　") + MyDocuments + MAIDTEXT("\n")
     +MAIDTEXT("CurrentDir:\n　")  + CurrentDir  + MAIDTEXT("\n")
     ;

    Shell::MessageBox( NULL, msg_text, MAIDTEXT("ディレクトリ設定"), MB_OK );

    {
      FileWrite hFile;

      const String dir = AppData + MAIDTEXT("\\maid_installertester");
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
