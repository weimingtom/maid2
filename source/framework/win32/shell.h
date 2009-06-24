/*!
 	@file
 	@brief	各種単発のシェル集
\n			ほとんどＣＯＭを使っているので CoInitilize を忘れないこと
 */


#ifndef maid2_framework_win32_shell_h
#define maid2_framework_win32_shell_h

#include"../../config/define.h"
#include"../../config/Win32.h"
#include"../../auxiliary/string.h"
#include"../../auxiliary/exception.h"

#include<vector>
#include<list>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  namespace Shell
  {
    void CoInitialize();
    void CoUninitialize();

    void InitCommonControls();

    String GetProgramFilesFolder( HWND hWnd );
    String GetDeskTopFolder( HWND hWnd );
    String GetProgramMenuFolder( HWND hWnd );
    String GetApplicationDataFolder( HWND hWnd );
    String GetMyDocumentsFolder( HWND hWnd );
    String GetWindowsFolder();
    String GetCurrentDirectory();
    String GetExeFileName();
    String GenerateUniqueName();

    bool IsExistFile( const String& FilePath );
    bool IsExistDirectory( const String& FilePath );
    size_t	GetFileSize( const String& FilePath );

    void SetCurrentDirectory( const String& FilePath );

    void CreateShortCut( const String& ShortCutPath, const String& TargetPath, const String& CommandLine );

    void CopyFile( const String& SrcPath, const String& DstPath );
    void DeleteFile( const String& FileName );
    void DeleteDirectory( const String& DirectoryName );
    String WinAPI_SHGetSpecialFolderLocation( HWND hWnd, int nFolder );

    String GetLastErrorString();

    String DeviceCapsStringOut();


    enum WINDOWSVERSION
    {
      WINDOWSVERSION_95=0x010000,
      WINDOWSVERSION_98,
      WINDOWSVERSION_ME,

      WINDOWSVERSION_NT=0x020000,
      WINDOWSVERSION_2000_PROFESSIONAL,
      WINDOWSVERSION_2000_SERVER,
      WINDOWSVERSION_XP_HOME,
      WINDOWSVERSION_XP_PROFESSIONAL,
      WINDOWSVERSION_XP_DOTNETSERVER,

      WINDOWSVERSION_VISTA,
      WINDOWSVERSION_7,

      WINDOWSVERSION_NOTFOUND=0xFF,
    };


    WINDOWSVERSION	GetWindowsVersion();
    bool			IsWindowsVersion9x();
    bool			IsWindowsVersionNT();


    String BrowseForFolder( HWND hWnd, const String& Title, const String& DefaultFolder );



    class FindObject
    {
    public:
      FindObject();
      FindObject( WIN32_FIND_DATA& rData );

      enum TYPE
      {
	      TYPE_FILE,			//!<	普通のファイル
	      TYPE_DIRECTORY,		//!<	ディレクトリ
	      TYPE_UNKOWN,		//!<	わかりません
      };

      TYPE    GetFileType() const ;
      String  GetFileName() const ;

    private:
      WIN32_FIND_DATA	m_FindData;
    };

    class IPickupFileFilter
    {
    public:
      virtual ~IPickupFileFilter(){}
      virtual bool IsPassage( const FindObject& Data ){return true;}
    };

    typedef boost::shared_ptr<IPickupFileFilter> SPPICKUPFILEFILTER;
    typedef std::list<FindObject>			FINDOBJECTLIST;

    FINDOBJECTLIST PickupFile( const String& Path );
    FINDOBJECTLIST PickupFile( const String& Path, const SPPICKUPFILEFILTER& pFilter );

  }
}

#endif