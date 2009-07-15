#include"../../../source/config/win32.h"
#include"../../../source/storage/fileio/fileoperation.h"
#include"../../../source/storage/fileio/filewrite.h"
#include"../../../source/framework/win32/shell.h"


#include"../define.h"

#include"resource.h"
#include<set>

using namespace Maid;



int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  const String UserAppData     = Shell::GetTemporaryFolder();
  const String path = UserAppData;

  const String MaidConfigExePath = path + MAIDTEXT("\\") + s_CONFIG_NAME;
  const String ConfigXMLDir = String::GetDirectory(Shell::GetExeFileName());
  const String ConfigXMLFileName = ConfigXMLDir + MAIDTEXT("\\config.xml");

  std::vector<String> CreateDirList;

  {
	  //	コンフィグexeのコピー
    FileOperation::CreateDirectory( path, CreateDirList );

	  HRSRC hResource = FindResource( NULL, MAKEINTRESOURCE(IDR_config1), L"config" );

	  const DWORD ExeSize = SizeofResource( NULL,hResource);

	  HGLOBAL hGlobal = LoadResource( NULL, hResource );
	  void* pExeImage = LockResource( hGlobal );

    {
      FileWrite hFile;
      hFile.Open( MaidConfigExePath, FileWrite::OPENOPTION_NEW );
      hFile.Write( pExeImage, ExeSize );
    }

	  FreeResource( hGlobal );
  }

  {
    //  で、起動
    const String verb = MAIDTEXT("open");
    const String ExecuteFileName = MaidConfigExePath;
    const String Param = MAIDTEXT("\"") + ConfigXMLFileName + MAIDTEXT("\"");
    const String Directry = ConfigXMLDir;
    const int ShowCom = SW_SHOWNORMAL;

    const FUNCTIONRESULT ret = Maid::Shell::ExecuteApplication( NULL, verb, ExecuteFileName, Param, Directry, ShowCom );
    if( ret==FUNCTIONRESULT_ERROR )
    {
      Maid::Shell::MessageBox( NULL, MAIDTEXT("アプリケーションの起動に失敗"), MAIDTEXT("確認"), MB_OK );
      return 0;
    }
  }

  return 0;
}
