#include"shell.h"
#include"com_ptr.h"
#include"registry.h"

#include<shlobj.h>
#include<shlwapi.h>
#define INITGUID
#include<initguid.h>
#include<dxdiag.h>
#include<io.h>
#include<shellapi.h>

#include<direct.h>
#include<stdio.h>

#include<string>

#pragma comment ( lib, "shell32.lib" )
#pragma comment ( lib, "shlwapi.lib" )
#pragma comment ( lib, "comctl32.lib" )


namespace Maid
{
  namespace Shell
  {
void CoInitialize()
{
  const HRESULT ret = ::CoInitialize( NULL );

  if( FAILED(ret) )
  {
    MAID_THROWEXCEPTION(MAIDTEXT("CoInitialize に失敗"));
  }
}

void CoUninitialize()
{
  ::CoUninitialize();
}


void InitCommonControls()
{
  ::InitCommonControls();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 各種システムディレクトリ名の取得
/*!
    @param  hWnd    [i ]  SHGetSpecialFolderLocation関数が表示するかもしれない
\n                      メッセージボックスのオーナーにしたいウィンドウハンドル
    @param  nFolder [i ]  調べたい引数 SHGetSpecialFolderLocation のヘルプを参照のこと
 
    @return パス名
 */
String WinAPI_SHGetSpecialFolderLocation( HWND hWnd, int nFolder )
{
	TCHAR szPath[_MAX_PATH] = {0};
  LPITEMIDLIST pidl;

  IMalloc *pMalloc;
  SHGetMalloc( &pMalloc );

  if( SUCCEEDED(::SHGetSpecialFolderLocation(hWnd,nFolder,&pidl)) )
  { 
    ::SHGetPathFromIDList(pidl,szPath);	 // パスに変換する
    pMalloc->Free(pidl);				 // 取得したIDLを解放する (CoTaskMemFreeでも可)
  }

  pMalloc->Release();

	return String::ConvertUNICODEtoMAID(szPath);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! Program Files ディレクトリを探す
/*!
    @param  hWnd  [i ]  SHGetSpecialFolderLocation関数が表示するかもしれない
\n                      メッセージボックスのオーナーにしたいウィンドウハンドル

    @return	Program Files のパス名
*/
String GetProgramFilesFolder( HWND hWnd )
{
  return WinAPI_SHGetSpecialFolderLocation( hWnd, CSIDL_PROGRAM_FILES );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デスクトップ ディレクトリを探す
/*!
    @param  hWnd  [i ]  SHGetSpecialFolderLocation関数が表示するかもしれない
\n                      メッセージボックスのオーナーにしたいウィンドウハンドル

    @return デスクトップ のパス名
 */
String GetUserDeskTopFolder( HWND hWnd )
{
  return WinAPI_SHGetSpecialFolderLocation( hWnd, CSIDL_DESKTOPDIRECTORY );
}

String GetCommonDeskTopFolder( HWND hWnd )
{
  return WinAPI_SHGetSpecialFolderLocation( hWnd, CSIDL_COMMON_DESKTOPDIRECTORY );
}




/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! Program Menu ディレクトリを探す
/*!
    @param  hWnd  [i ]  SHGetSpecialFolderLocation関数が表示するかもしれない
\n                      メッセージボックスのオーナーにしたいウィンドウハンドル
 
    @return Program Menu のパス名
 */
String GetUserProgramMenuFolder( HWND hWnd )
{
	return WinAPI_SHGetSpecialFolderLocation( hWnd, CSIDL_PROGRAMS );
}

String GetCommonProgramMenuFolder( HWND hWnd )
{
  return WinAPI_SHGetSpecialFolderLocation( hWnd, CSIDL_COMMON_PROGRAMS );
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! AppData ディレクトリを探す
/*!
    @param  hWnd  [i ]  SHGetSpecialFolderLocation関数が表示するかもしれない
\n                      メッセージボックスのオーナーにしたいウィンドウハンドル

    @return Program Menu のパス名
 */
String GetUserApplicationDataFolder( HWND hWnd )
{
	return WinAPI_SHGetSpecialFolderLocation( hWnd, CSIDL_APPDATA );
}

String GetCommonApplicationDataFolder( HWND hWnd )
{
	return WinAPI_SHGetSpecialFolderLocation( hWnd, CSIDL_COMMON_APPDATA );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! MyDocument ディレクトリを探す
/*!
    @param  hWnd  [i ]  SHGetSpecialFolderLocation関数が表示するかもしれない
\n                      メッセージボックスのオーナーにしたいウィンドウハンドル

    @return Program Menu のパス名
 */
String GetMyDocumentsFolder( HWND hWnd )
{
	return WinAPI_SHGetSpecialFolderLocation( hWnd, CSIDL_MYDOCUMENTS );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! Windows ディレクトリを探す
/*!
    @return Windows のパス名
 */
String GetWindowsFolder()
{
  const UINT len =  ::GetWindowsDirectory( NULL, 0 );

  std::vector<wchar_t> Text( len );

  ::GetWindowsDirectory( &(Text[0]), len );
  Text.push_back('\0');

  return String::ConvertUNICODEtoMAID(&(Text[0]));
}

String GetTemporaryFolder()
{
  const UINT len =  ::GetTempPath( 0, NULL );

  std::vector<wchar_t> Text( len );

  ::GetTempPath( len, &(Text[0]) );
  Text.pop_back();  //  <- このＡＰＩは文字列の最後に '/' がついてしまうので除去する
  Text.push_back('\0');

  return String::ConvertUNICODEtoMAID(&(Text[0]));
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在のカレントディレクトリ名を取得する
/*!
    @return Windows のパス名
 */
String GetCurrentDirectory()
{
  wchar_t buf[MAX_PATH];
  ::GetCurrentDirectory( MAX_PATH, buf );

  return String::ConvertUNICODEtoMAID(buf);
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! このプログラムを実行しているexeファイル名を取得する
/*!
 *	\return	Windows のパス名
 */
String GetExeFileName()
{
  wchar_t szPath[MAX_PATH];
  ::GetModuleFileName( NULL, szPath, MAX_PATH );
  return String::ConvertUNICODEtoMAID(szPath);
}

void SetCurrentDirectory( const String& FilePath )
{
  ::SetCurrentDirectory( Maid::String::ConvertMAIDtoUNICODE(FilePath).c_str() );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ショートカットの作成
/*!
    @param  ShortCutPath  [i ]  作成するショートカットファイル名
    @param  TargetPath    [i ]  ショートカットを作られる実体のファイル名
    @param  CommandLine   [i ]  コマンドライン

    @exception CException ショートカットの作成に失敗した場合
 */
FUNCTIONRESULT CreateShortCut( const String& ShortCutPath, const String& TargetPath, const String& CommandLine )
{
  const std::wstring unicode_shorcut = String::ConvertMAIDtoUNICODE(ShortCutPath);
  const std::wstring unicode_target  = String::ConvertMAIDtoUNICODE(TargetPath);
  const std::wstring unicode_command = String::ConvertMAIDtoUNICODE(CommandLine);

  com_ptr<IShellLink> pShellLink;


  pShellLink.CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink );
  pShellLink->SetPath(unicode_target.c_str());

  if( !CommandLine.empty() )
  {
    const HRESULT ret = pShellLink->SetArguments( unicode_command.c_str() ); 
    if( FAILED(ret) ) { return FUNCTIONRESULT_ERROR; }
  }

  {
    com_ptr<IPersistFile> pPersistFile;

    OLECHAR SCPath[500];    //保存先のパスはUnicode
    ZERO( SCPath, sizeof(SCPath) );

//    ::MultiByteToWideChar(CP_ACP, 0, unicode_shorcut.c_str(), -1, SCPath, MAX_PATH );

    pShellLink.QueryInterface(IID_IPersistFile, pPersistFile);

    const HRESULT ret = pPersistFile->Save(unicode_shorcut.c_str(),TRUE);
    if( FAILED(ret) ) { return FUNCTIONRESULT_ERROR; }
  }

  return FUNCTIONRESULT_OK;
}


bool IsExistFile( const String& FilePath )
{
  const std::wstring src = String::ConvertMAIDtoUNICODE(FilePath);

  return ::PathFileExists(src.c_str())!=0;
}

bool IsExistDirectory( const String& FilePath )
{
  const std::wstring src = String::ConvertMAIDtoUNICODE(FilePath);

  return ::PathIsDirectory(src.c_str())!=0;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルサイズの取得
/*!
    @return ファイルサイズ
            ファイルが存在しない場合は 0
 */
unt	GetFileSize( const String& FilePath )
{
  const std::wstring path = String::ConvertMAIDtoUNICODE(FilePath);

  unt ret =0;

  HANDLE hFile;
  DWORD  FileSizeHigh;

  hFile = ::CreateFile(
    path.c_str(),
    0,
    FILE_SHARE_READ,
    NULL,
    OPEN_EXISTING,
    FILE_ATTRIBUTE_NORMAL,
    NULL);

  ret = ::GetFileSize(hFile, &FileSizeHigh);

  ::CloseHandle(hFile);

  return ret;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルのコピー
/*!
    コピー先にディレクトリがない場合、失敗します

    @param  SrcPath [i ]  コピー元ファイル名
    @param  DstPath [i ]  コピー先ファイル名

    @exception Exception コピーに失敗した場合
 */
FUNCTIONRESULT CopyFile( const String& SrcPath, const String& DstPath )
{
  const std::wstring src = String::ConvertMAIDtoUNICODE(SrcPath);
  const std::wstring dst = String::ConvertMAIDtoUNICODE(DstPath);

  if( ::CopyFile( src.c_str(), dst.c_str(), FALSE )==0 ) 
  {
    return FUNCTIONRESULT_ERROR;
  }
  return FUNCTIONRESULT_OK;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルの削除
/*!
    @param  FileName  [i ]	削除するファイル名

    @exception Exception 削除に失敗した場合
 */
FUNCTIONRESULT DeleteFile( const String& FileName )
{
  const std::wstring src = String::ConvertMAIDtoUNICODE(FileName);

  if( ::PathFileExists(src.c_str())==0 ) { return FUNCTIONRESULT_OK; }

  if( ::DeleteFile(src.c_str())==0 ) 
  {
    return FUNCTIONRESULT_ERROR;
  }
  return FUNCTIONRESULT_OK;
}

FUNCTIONRESULT DeleteDirectory( const String& DirectoryName )
{
  const std::wstring src = String::ConvertMAIDtoUNICODE(DirectoryName);
  if( ::PathIsDirectory(src.c_str())==0 ) { return FUNCTIONRESULT_OK; }

  if( ::RemoveDirectory(src.c_str())==0 ) 
  {
    return FUNCTIONRESULT_ERROR;
  }
  return FUNCTIONRESULT_OK;
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//!	ユニークな名前を作成する
/*!
    @return	名前
 */
String GenerateUniqueName()
{
  GUID g;
  ::CoCreateGuid(&g);

  char buf[256];

  sprintf( buf, "%8x%4x%4x%s", g.Data1, g.Data2, g.Data3, g.Data4 );

  const String ret = String::ConvertSJIStoMAID(buf);

  return ret;
}

FUNCTIONRESULT ExecuteApplication( HWND hWnd, const String& Verb, const String& ExecuteFileName, const String& Param, const String& Directry, int ShowCom )
{
  const std::wstring uni_Verb = String::ConvertMAIDtoUNICODE(Verb);
  const std::wstring uni_Execute = String::ConvertMAIDtoUNICODE(ExecuteFileName);
  const std::wstring uni_Param = String::ConvertMAIDtoUNICODE(Param);
  const std::wstring uni_Directry = String::ConvertMAIDtoUNICODE(Directry);

  const int ret = (int)ShellExecute( hWnd,
                                uni_Verb.empty()?     NULL : uni_Verb.c_str(),
                                uni_Execute.empty()?  NULL : uni_Execute.c_str(),
                                uni_Param.empty()?    NULL : uni_Param.c_str(),
                                uni_Directry.empty()? NULL : uni_Directry.c_str(),
                                ShowCom
    );


  if( ret<=32 )
  {
    return FUNCTIONRESULT_ERROR;
  }
  return FUNCTIONRESULT_OK;
}

FUNCTIONRESULT ExecuteApplicationWait( HWND hWnd, const String& Verb, const String& ExecuteFileName, const String& Param, const String& Directry, int ShowCom, DWORD& ReturnCode )
{
  const std::wstring uni_Verb = String::ConvertMAIDtoUNICODE(Verb);
  const std::wstring uni_Execute = String::ConvertMAIDtoUNICODE(ExecuteFileName);
  const std::wstring uni_Param = String::ConvertMAIDtoUNICODE(Param);
  const std::wstring uni_Directry = String::ConvertMAIDtoUNICODE(Directry);

  SHELLEXECUTEINFO info={0};
  info.cbSize = sizeof(SHELLEXECUTEINFO);
  info.hwnd = hWnd;
  info.lpVerb = uni_Verb.c_str();
  info.lpFile = uni_Execute.c_str();
  info.lpParameters = uni_Param.c_str();
  info.lpDirectory  = uni_Directry.c_str();
  info.nShow = ShowCom;
  info.fMask = SEE_MASK_NOCLOSEPROCESS;

  if( !ShellExecuteEx(&info) || (int)info.hInstApp <= 32 ) { return FUNCTIONRESULT_ERROR; }

	WaitForSingleObject( info.hProcess, INFINITE );

  {
    DWORD ret = 0;
    const BOOL check = GetExitCodeProcess( info.hProcess, &ret );
    if( check==FALSE ) { return FUNCTIONRESULT_ERROR; }
    ReturnCode = ret;
  }
  return FUNCTIONRESULT_OK;
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 直前に呼び出したＡＰＩのエラーコードを文字列化する
/*!
 	@return	エラーメッセージ
 */
String GetLastErrorString()
{
  LPVOID pMsgBuf;

  ::FormatMessage( 
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      GetLastError(),
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト言語
      (LPTSTR) &pMsgBuf,
      0,
      NULL 

  );

  const String str = String::ConvertUNICODEtoMAID((wchar_t*)pMsgBuf);

  LocalFree( pMsgBuf );

  return str;
}

int MessageBox( HWND hWnd, const String& Text, const String& Caption, UINT Type )
{
  const std::wstring text_sjis = String::ConvertMAIDtoUNICODE(Text);
  const std::wstring caption_sjis = String::ConvertMAIDtoUNICODE(Caption);

  return ::MessageBox( hWnd, text_sjis.c_str(), caption_sjis.c_str(), Type );
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! Windowsの種類を取得する
/*!
 *	\return	Windowsの種類
 */
WINDOWSVERSION	GetWindowsVersion()
{
  OSVERSIONINFO	ver   = { sizeof(OSVERSIONINFO) };
  OSVERSIONINFOEX	verex = { sizeof(OSVERSIONINFOEX) };
  ::GetVersionEx( &ver );
  ::GetVersionEx( (LPOSVERSIONINFOW)&verex );

  if( ver.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS )
  {
    if( ver.dwMajorVersion==4 )
    {
      switch( ver.dwMinorVersion )
      {
      case  0: { return WINDOWSVERSION_95; }break;
      case 10: { return WINDOWSVERSION_98; }break;
      case 90: { return WINDOWSVERSION_ME; }break;
      }
    }

  }else if( ver.dwPlatformId==VER_PLATFORM_WIN32_NT )
  {
    if( ver.dwMajorVersion==5 )
    {
      switch( ver.dwMinorVersion )
      {
      case 0:
        {
          switch( verex.wProductType )
          {
          case VER_NT_WORKSTATION: { return WINDOWSVERSION_2000_PROFESSIONAL; }
          case VER_NT_SERVER:		 { return WINDOWSVERSION_2000_SERVER; }
          }
        }break;
      case 1:
        {
          if( IsFlag(verex.wSuiteMask,VER_SUITE_PERSONAL) )
          {
            return WINDOWSVERSION_XP_HOME; 
          }else
          {
            switch( verex.wProductType )
            {
            case VER_NT_WORKSTATION: { return WINDOWSVERSION_XP_PROFESSIONAL; }
            case VER_NT_SERVER:		 { return WINDOWSVERSION_XP_DOTNETSERVER; }
            }
          }
        }break;
      }
    }
    else if( ver.dwMajorVersion==6 )
    {
      switch( ver.dwMinorVersion )
      {
      case 0: { return WINDOWSVERSION_VISTA; }break;
      case 1: { return WINDOWSVERSION_7; }break;
      }    
    }
    else if( ver.dwMajorVersion<5 )
    {
      return WINDOWSVERSION_NT;
    }

  }

  return WINDOWSVERSION_NOTFOUND;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! Windows9X 系であるか？
/*!
    @return	9x系であるなら true
 */
bool  IsWindowsVersion9x()
{
  return GetWindowsVersion()<WINDOWSVERSION_NT;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! WindowsNT 系であるか？
/*!
    @return	NT系であるなら true
 */
bool  IsWindowsVersionNT()
{
  return !IsWindowsVersion9x();
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! BrowseForFolder() の内部で呼ばれるコールバックプロシージャ
/*!
    今のところ最初から選択されているフォルダの設定しかしていない

    @param  hWnd	[i ]	ダイアログボックスのウィンドウハンドル
    @param  uMsg	[i ]	各種メッセージ
    @param  lParam	[i ]	メッセージの詳細
    @param  lpData	[i ]	メッセージの詳細

    @return 常に０
 */
int CALLBACK SHBrowseProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
{
  switch( uMsg )
  {
  case BFFM_INITIALIZED:
    {
      if( lpData!=NULL )
      {
        SendMessage( hWnd, BFFM_SETSELECTION, TRUE, lpData);
      }
    }
    break;
  }

    return 0;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! フォルダ選択ダイアログの表示
/*!
  @param  hWnd			[i ]	親ウィンドウ
  @param  Title			[i ]	タイトル
  @param  DefaultFolder	[i ]	最初に選択されている（デフォルトで空文字）

  @return 選択したフォルダパス
          選択しなくても "" が返る
 */
String BrowseForFolder( HWND hWnd, const String& Title, const String& DefaultFolder )
{
  const std::wstring unicode_lparam = String::ConvertMAIDtoUNICODE(DefaultFolder);
  const std::wstring unicode_tittle = String::ConvertMAIDtoUNICODE(Title);

  BROWSEINFO  bi;

  ZeroMemory( &bi, sizeof(bi) );
  bi.hwndOwner = hWnd;
  bi.lParam = (LPARAM)unicode_lparam.c_str();
  bi.lpfn   = SHBrowseProc;
  bi.lpszTitle = unicode_tittle.c_str();
  bi.ulFlags   = BIF_RETURNONLYFSDIRS;

  ITEMIDLIST  *lpidlist = ::SHBrowseForFolder(&bi);    // ダイアログ表示
  if( lpidlist==NULL ) { 	return String();  }				//キャンセルを押したとき

  wchar_t    buf[MAX_PATH];
  ::SHGetPathFromIDList(lpidlist, buf);    // ITEMIDLIST から絶対パスを取得。

  //解放
  IMalloc *lpimalloc;
  ::SHGetMalloc(&lpimalloc);
  if( lpimalloc!=NULL )
  {
    lpimalloc->Free(lpidlist);
    lpimalloc->Release();
  }

  return String::ConvertUNICODEtoMAID(buf);
}







/*!
    @class CFindObject CFindObject.h
    @brief ファイル情報クラス
 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
FindObject::FindObject()
{
  ZERO( &m_FindData, sizeof(m_FindData) );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 	@param	rData	[i ]	初期化情報
 */
FindObject::FindObject( WIN32_FIND_DATA& rData )
{
  m_FindData = rData;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 設定されたデータのファイルタイプの取得
/*!
    @return FILETYPE を見てください
 */
FindObject::TYPE FindObject::GetFileType() const 
{
  const DWORD dwAttribute = m_FindData.dwFileAttributes;

  if( IsFlag(dwAttribute,FILE_ATTRIBUTE_DIRECTORY) )	{ return TYPE_DIRECTORY; }
  if( IsFlag(dwAttribute,FILE_ATTRIBUTE_ARCHIVE)	 )	{ return TYPE_FILE; }
  if( IsFlag(dwAttribute,FILE_ATTRIBUTE_NORMAL)	 )	{ return TYPE_FILE; }

  return TYPE_UNKOWN;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 設定されたデータのファイル名の取得
/*!
    @return ファイル名
 */
String	FindObject::GetFileName() const 
{
  return String::ConvertUNICODEtoMAID(m_FindData.cFileName);
}



FINDOBJECTLIST PickupFile( const String& Path )
{
  return PickupFile( Path, SPPICKUPFILEFILTER( new IPickupFileFilter ) );
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定したパターンに合うファイルを抜き出す
/*!
  @param	Path	[i ]	抜き出したいファイルパターン（ *,? も使用可能）
  @param	pFilter	[i ]	比較するフィルタ

  @return	抜き出したファイル名リスト

  @exception Exception コピーに失敗した場合
 */
FINDOBJECTLIST PickupFile( const String& Path, const SPPICKUPFILEFILTER& pFilter )
{
  FINDOBJECTLIST FileList;
  HANDLE hFind;

  WIN32_FIND_DATA	Filedata;
  ZERO( &Filedata, sizeof(Filedata) );

  const std::wstring unicode_path = String::ConvertMAIDtoUNICODE(Path);


  hFind = ::FindFirstFile( unicode_path.c_str(), &Filedata );
  if( hFind==INVALID_HANDLE_VALUE ) { return FINDOBJECTLIST(); }

  if( pFilter->IsPassage( FindObject(Filedata) ) )
  {
      FileList.push_back( Filedata );
  }

  const std::wstring BeginName = Filedata.cFileName;	//	ほっとくとずっと回ってしまうのでチェック文字を作る
  while( true )
  {
    if( ::FindNextFile( hFind, &Filedata )==0 ) { break; }

    if( pFilter->IsPassage( FindObject(Filedata) ) )
    {
	    FileList.push_back( Filedata );
    }

    if( BeginName==Filedata.cFileName ) { break; }
  }

  ::FindClose( hFind );


  return FileList;
}









std::wstring GetValue( const com_ptr<IDxDiagContainer>&	pContainer, const std::wstring& KeyName )
{
  HRESULT ret;
    VARIANT var;

  WCHAR wszPropValue[256];
    VariantInit( &var );

    ret = pContainer->GetProp( KeyName.c_str(), &var );
    if( SUCCEEDED(ret) )
    {
        switch( var.vt )
        {
        case VT_UI4:{ swprintf( wszPropValue, L"%d", var.ulVal );	}break;
        case VT_I4:	{ swprintf( wszPropValue, L"%d", var.lVal );	}break;
        case VT_BOOL:{ swprintf( wszPropValue, L"%s", (var.boolVal) ? L"true" : L"false" );	}break;
        case VT_BSTR:
          {
                    wcsncpy( wszPropValue, var.bstrVal, 255 );
                    wszPropValue[255] = 0;
          }break;
        }

        VariantClear( &var );
  }

  return wszPropValue;
}

std::wstring GetDxDiagSystemInfo( const com_ptr<IDxDiagContainer>& pDxDiagRoot )
{
  HRESULT ret;
  com_ptr<IDxDiagContainer>	pContainer;
  {
	  IDxDiagContainer* p;
	  ret = pDxDiagRoot->GetChildContainer( L"DxDiag_SystemInfo", &p );
	  if( FAILED(ret) ) { return std::wstring(); }
	  pContainer.reset( p );
  }

  std::wstring	str;

  str += L"PhysicalMemory " + GetValue( pContainer, L"ullPhysicalMemory" ) + L"\r\n";
  str += L"UsedPageFile   " + GetValue( pContainer, L"ullUsedPageFile" ) + L"\r\n";
  str += L"AvailPageFile  " + GetValue( pContainer, L"ullAvailPageFile" ) + L"\r\n";

  str += L"ProcessorEnglish          " + GetValue( pContainer, L"szProcessorEnglish" ) + L"\r\n";
  str += L"SystemManufacturerEnglish " + GetValue( pContainer, L"szSystemManufacturerEnglish" ) + L"\r\n";
  str += L"SystemModelEnglish        " + GetValue( pContainer, L"szSystemModelEnglish" ) + L"\r\n";
  str += L"BIOSEnglish               " + GetValue( pContainer, L"szBIOSEnglish" ) + L"\r\n";
  str += L"PhysicalMemoryEnglish     " + GetValue( pContainer, L"szPhysicalMemoryEnglish" ) + L"\r\n";

  return str;
}


std::wstring GetDxDiagDisplayDevices( const com_ptr<IDxDiagContainer>&	pDxDiagRoot )
{
  HRESULT ret;
  com_ptr<IDxDiagContainer>	pContainer;
  {
    IDxDiagContainer* p;
    ret = pDxDiagRoot->GetChildContainer( L"DxDiag_DisplayDevices", &p );
    if( FAILED(ret) ) { return std::wstring(); }
    pContainer.reset( p );
  }

  std::wstring	str;

    DWORD ChildCount;

  ret = pContainer->GetNumberOfChildContainers( &ChildCount );
    if( FAILED(ret) ) { return std::wstring(); }

  for( DWORD i=0; i<ChildCount; ++i )
  {
    WCHAR wszChildName[256];

    ret = pContainer->EnumChildContainerNames( i, wszChildName, 256 );
    if( FAILED(ret) ) { continue; }

    com_ptr<IDxDiagContainer>	pChild;
    {
      IDxDiagContainer* p;
      ret = pContainer->GetChildContainer( wszChildName, &p );
      if( FAILED(ret) ) { continue; }
      pChild.reset( p );
    }

    str += L"   device    \r\n";
    str += L"Description            " + GetValue( pChild, L"szDescription" ) + L"\r\n";
    str += L"Manufacturer           " + GetValue( pChild, L"szManufacturer" ) + L"\r\n";
    str += L"ChipType               " + GetValue( pChild, L"szChipType" ) + L"\r\n";

    str += L"DisplayMemoryLocalized " + GetValue( pChild, L"szDisplayMemoryLocalized" ) + L"\r\n";
    str += L"DisplayMemoryEnglish   " + GetValue( pChild, L"szDisplayMemoryEnglish" ) + L"\r\n";
    str += L"DriverVersion          " + GetValue( pChild, L"szDriverVersion" ) + L"\r\n";
  }

  return str;

}

String DeviceCapsStringOut()
{
  String		ReturnText;

  try
  {
    {
      OSVERSIONINFO	ver = { sizeof(OSVERSIONINFO) };
      ::GetVersionEx( &ver );

      Registry	hReg;
      String		strData;

      strData += MAIDTEXT("Windows  Caps-------------------------------------------------------------------\r\n");

      //	Win9x系　かWinNT系で キーの位置が違う
      String subkey;
      if( ver.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS )
      {
        subkey = MAIDTEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion");
      }else if( ver.dwPlatformId==VER_PLATFORM_WIN32_NT )
      {
        subkey = MAIDTEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
      }
  		
      hReg.Open( HKEY_LOCAL_MACHINE, subkey );

      {
        String		str;
        hReg.GetValue( MAIDTEXT("ProductName"), str );
        if( !str.empty() )	//	OS名の取得
        {
          strData += str;
          strData += MAIDTEXT("\r\n");
        }
      }

      {
        String		str;
        hReg.GetValue( MAIDTEXT("CSDVersion"), str );
        if( !str.empty() )	//	OS名の取得
        {
          strData += str;
          strData += MAIDTEXT("\r\n");
        }
      }

      //	ビルド番号、バージョンの取得
      char	mess[256];
      sprintf( mess, "Bulid %d Ver %d.%d\r\n", ver.dwBuildNumber<3000?ver.dwBuildNumber:0, ver.dwMajorVersion, ver.dwMinorVersion );
      strData += String::ConvertSJIStoMAID(mess);


      //	DirectX のバージョンの取得
      hReg.Open( HKEY_LOCAL_MACHINE, MAIDTEXT("SOFTWARE\\Microsoft\\DirectX") );

      {
        String		str;
        hReg.GetValue( MAIDTEXT("Version"), str );

        if( !str.empty() )
        {
          strData += MAIDTEXT("DirectX Ver ");
          strData += str;
          strData += MAIDTEXT("\r\n");
        }
      }

      ReturnText += strData;
    }
    {
      std::wstring		strData;
      //	dxdiag から拾える情報を拾う
      //	
      com_ptr<IDxDiagProvider>	pDxDiagProvider;
      com_ptr<IDxDiagContainer>	pDxDiagRoot;


      pDxDiagProvider.CoCreateInstance( CLSID_DxDiagProvider, NULL, CLSCTX_INPROC_SERVER, IID_IDxDiagProvider );

      {
        // Fill out a DXDIAG_INIT_PARAMS struct and pass it to IDxDiagContainer::Initialize
        // Passing in TRUE for bAllowWHQLChecks, allows dxdiag to check if drivers are 
        // digital signed as logo'd by WHQL which may connect via internet to update 
        // WHQL certificates.    
        DXDIAG_INIT_PARAMS dxDiagInitParam;
        ZeroMemory( &dxDiagInitParam, sizeof(DXDIAG_INIT_PARAMS) );

        dxDiagInitParam.dwSize                  = sizeof(DXDIAG_INIT_PARAMS);
        dxDiagInitParam.dwDxDiagHeaderVersion   = DXDIAG_DX9_SDK_VERSION;
        // WHQLを確認するかどうか
        // ネットを経由するので基本的にはfalseにしておく
        dxDiagInitParam.bAllowWHQLChecks        = false;
        dxDiagInitParam.pReserved               = NULL;

        HRESULT		ret;
        ret = pDxDiagProvider->Initialize( &dxDiagInitParam );
        if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDxDiagProvider::Initialize") ); }

        {
          IDxDiagContainer* p;
          ret = pDxDiagProvider->GetRootContainer( &p );
          if( FAILED(ret) ) {	MAID_THROWEXCEPTION(MAIDTEXT("IDxDiagContainer::GetRootContainer") ); }

          pDxDiagRoot.reset( p );
        }

        /*

          必要な項目の以下のとおり
          ここに書いてあるのだけ拾ってくる
          文字列はすべて unicode になっているので注意

        DxDiag_SystemInfo.ullPhysicalMemory = 536330240
        DxDiag_SystemInfo.ullUsedPageFile = 236158976
        DxDiag_SystemInfo.ullAvailPageFile = 1072644096

        DxDiag_SystemInfo.szProcessorEnglish = AMD Athlon(tm) Processor,  MMX,  3DNow, ~1.3GHz
        DxDiag_SystemInfo.szSystemManufacturerEnglish = VIA
        DxDiag_SystemInfo.szSystemModelEnglish = 7VRX                            
        DxDiag_SystemInfo.szBIOSEnglish = Default System BIOS
        DxDiag_SystemInfo.szPhysicalMemoryEnglish = 512MB RAM

        DxDiag_DisplayDevices.0.szDescription = Xabre
        DxDiag_DisplayDevices.0.szManufacturer = SiS
        DxDiag_DisplayDevices.0.szChipType = Xabre  80 Rev 01
        DxDiag_DisplayDevices.0.szDisplayMemoryLocalized = 64.0 MB
        DxDiag_DisplayDevices.0.szDisplayMemoryEnglish = 64.0 MB
        DxDiag_DisplayDevices.0.szDriverVersion = 6.14.0010.3530

        */

        {
          strData += GetDxDiagSystemInfo( pDxDiagRoot );
          strData += GetDxDiagDisplayDevices( pDxDiagRoot );
        }
      }

      std::vector<char> ret;
      {
        const int len = ::WideCharToMultiByte(CP_THREAD_ACP,0,strData.c_str(),-1,NULL,0,NULL,NULL);
        if( 0<len )
        {
          ret.resize(len);
          ::WideCharToMultiByte(CP_THREAD_ACP,0,strData.c_str(),-1, &ret[0],len,NULL,NULL);
          ret.push_back( '\0' );
        }
      }

      ReturnText += String::ConvertSJIStoMAID( &ret[0] );
    }
  }catch(...)	{}

  if( ReturnText.empty() ) { ReturnText = MAIDTEXT("Unkwon"); }

  return ReturnText;
}




	}
}