#include"../../../source/framework/application.h"
#include"../../../source/auxiliary/xml.h"
#include"../../../source/storage/fileio/filereadnormal.h"
#include"../../../source/framework/win32/shell.h"
#include"../../../source/framework/win32/registry.h"


#include"../define.h"
#include"../uninstallprogram.h"


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
		const String UninstallInfo = static_cast<OSDevice&>(GetOSDevice()).GetCmdLine(1);

    if( UninstallInfo.empty() )
    {
      ::MessageBox( NULL, L"設定ファイルが指定されていません", L"エラー", MB_OK );	
      OnExit(0);
      return; 
    }

		if( ::MessageBox( NULL, L"アンインストールしますか？", L"確認", MB_YESNO )!=IDYES ) 
		{
			OnExit(0);
			return; 
		}

    UNINSTALLPROGRAM  program;

    {
      const FUNCTIONRESULT ret = LoadUnInstallProgram( UninstallInfo, program );
      if( ret==FUNCTIONRESULT_ERROR )
      {
        ::MessageBox( NULL, L"設定ファイルの読み込みに失敗しました", L"エラー", MB_OK );	
        OnExit(0);
        return; 
      }
    }

    {
      //  レジストリの削除
      for( int i=0; i<(int)program.DeleteRegistryList.size(); ++i )
      {
        const UNINSTALLPROGRAM::DELETEREGISTRY& reg = program.DeleteRegistryList[i];

        HKEY hKey = NULL;

        {
               if( reg.Handle==MAIDTEXT("HKEY_CLASSES_ROOT") ) { hKey = HKEY_CLASSES_ROOT; }
          else if( reg.Handle==MAIDTEXT("HKEY_CURRENT_USER") ) { hKey = HKEY_CURRENT_USER; }
          else if( reg.Handle==MAIDTEXT("HKEY_LOCAL_MACHINE")) { hKey = HKEY_LOCAL_MACHINE;}
          else if( reg.Handle==MAIDTEXT("HKEY_USERS")        ) { hKey = HKEY_USERS;        }
        }

        if( reg.IsKeyDelete )
        {
          Registry::DeleteKey( hKey, reg.SubKey );
        }else
        {
          for( int j=0; j<(int)reg.ValueList.size(); ++j )
          {
            const String& name = reg.ValueList[j];
            Registry::DeleteValue( hKey, reg.SubKey, name );
          }
        }
      }
    }

    {
      //  ファイルの削除
      for( int i=0; i<(int)program.DeleteFileList.size(); ++i )
      {
        const UNINSTALLPROGRAM::DELETEFILE& dat = program.DeleteFileList[i];

        if( !dat.Text.empty() )
        {
          const int ret = Shell::MessageBox( NULL, dat.Text, MAIDTEXT("確認"), MB_YESNO );
          if( ret!=IDYES ) { continue; }
        }

				Shell::DeleteFile( dat.TargetPath );
      }
    }

    {
      //  フォルダの削除
      for( int i=0; i<(int)program.DeleteFolderList.size(); ++i )
      {
        const UNINSTALLPROGRAM::DELETEFOLDER& dat = program.DeleteFolderList[i];

        if( !dat.Text.empty() )
        {
          const int ret = Shell::MessageBox( NULL, dat.Text, MAIDTEXT("確認"), MB_YESNO );
          if( ret!=IDYES ) { continue; }
        }

        if( dat.IsAll )
        {
          std::list<String> FileList;
          std::list<String> FolderList;
          SerachFile( dat.TargetPath, FileList, FolderList );

          FolderList.sort();

          for( std::list<String>::iterator ite=FileList.begin(); ite!=FileList.end(); ++ite )
          {
				    Shell::DeleteFile( *ite );
          }

          for( std::list<String>::iterator ite=FolderList.begin(); ite!=FolderList.end(); ++ite )
          {
				    Shell::DeleteDirectory( *ite );
          }
        }

				Shell::DeleteDirectory( dat.TargetPath );
      }
    }

    ::Sleep(1000);
 
		::MessageBox( NULL, L"アンインストールが終わりました", L"確認", MB_OK );	

    OnExit(0);
	}

  virtual void Finalize()
  {

  }

private:
  void	SerachFile( const String& Dir, std::list<String>& FileList, std::list<String>& FolderList )
  {
	  Shell::FINDOBJECTLIST	CandidateFile;

    if( Dir.empty() ) { CandidateFile = Shell::PickupFile( MAIDTEXT("*") ); }
    else { CandidateFile = Shell::PickupFile( Dir+MAIDTEXT("\\*.*") ); }

	  for( Shell::FINDOBJECTLIST::iterator ite=CandidateFile.begin(); ite!=CandidateFile.end(); ++ite )
	  {
		  const String str = ite->GetFileName();

		  if( str[0]=='.' ) { continue; }

		  switch( ite->GetFileType() )
		  {
		  case Shell::FindObject::TYPE_FILE:
			  {
          if( Dir.empty() ) { FileList.push_back( String::ToLower(str) ); }	//<- ついでに小文字化しておく
          else { FileList.push_back( String::ToLower(Dir+MAIDTEXT("\\")+str) ); }	//<- ついでに小文字化しておく
          
			  }break;

		  case Shell::FindObject::TYPE_DIRECTORY:
			  {
          String fullpath;

          if( !Dir.empty() ) 
          {
            fullpath = String::ToLower(Dir+MAIDTEXT("\\")+str);
            FolderList.push_back(fullpath);
          }

          SerachFile( fullpath, FileList, FolderList );
  				
			  }break;
		  }
	  }
  }


};



int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
  MyApp app;
  
  return app.Run(hInstance,hPrevInstance,lpCmdLine,nCmdShow);
}
