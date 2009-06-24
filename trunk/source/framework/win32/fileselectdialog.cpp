#include"fileselectdialog.h"
#include"shell.h"

static wchar_t s_FullPath[MAX_PATH];		
static wchar_t s_FileTitle[MAX_PATH];

namespace Maid
{



/*!
 *	\class FileSelectDialog fileselectdialog.h
 *	\brief GetOpenFileName() or GetSaveFileName()　のラッパークラス
 *
 *			当然ですが Windows 専用です
 *
*/

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!	
 *
 */
FileSelectDialog::FileSelectDialog()
{
	//m_ofnの初期化
	ZeroMemory( &m_ofn, sizeof(m_ofn) );
//	m_ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400;	//Win2000以外でも使えるようにする
	m_ofn.lStructSize = sizeof(m_ofn);	
	m_ofn.nMaxFile		 = MAX_PATH;
	m_ofn.nMaxFileTitle  = MAX_PATH;

	SetDefaultDirectory( Shell::GetCurrentDirectory() );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!	
 *
 */
FileSelectDialog::~FileSelectDialog()
{
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ダイアログが開いたときにデフォルトで開いているディレクトリの設定
/*!
 *
 *	\param  _strDirectry	[i ]	ディレクトリ名
 *
 */
void FileSelectDialog::SetDefaultDirectory( const String& dir )
{
	m_DefaultDirectry = String::ConvertMAIDtoUNICODE(dir);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 選択できる拡張子の設定
/*!
 *	\param  _szFilter	[i ]	拡張子名
 */
void FileSelectDialog::SetFileFilter( const wchar_t* filter )
{
	// フィルターは区切り文字に '\0' を使用しているため、stringは使えないので自分で計算する
	int i=2;
	while( true )
	{
		if( (filter[i-2]=='\0') && (filter[i-1]=='\0') ) { break; }
		i++;
	}

  m_Filter = std::wstring(filter, i );
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 「ファイルを開く」ダイアログボックスを出す
/*!
 	@param  hWnd		[i ]	オーナーハンドル
 	@param  Title		[i ]	ダイアログボックスのタイトルバーに表示される文字列
 	@param  DefExt		[i ]	ダイアログができたときにデフォルトで入っている文字列
 	@param  DefaultName	[i ]	最初からエディットボックスに入っているファイル名
 
	@return ファイルを指定して［OK］ボタンを押すと、trueが返ります
 			ダイアログボックスをキャンセルしたり閉じたりした場合は、false が返ります	
*/
bool FileSelectDialog::ShowOpenDialog( HWND hWnd, const String& Title, const String& DefExt, const String& DefaultName )
{
	BOOL ret;

	const std::wstring uni_tittle = String::ConvertMAIDtoUNICODE(Title);
	const std::wstring uni_ext    = String::ConvertMAIDtoUNICODE(DefExt);
	const std::wstring uni_name   = String::ConvertMAIDtoUNICODE(DefaultName);

	::lstrcpy( s_FileTitle, uni_name.c_str() );

	m_ofn.hwndOwner			= hWnd;
	m_ofn.lpstrFilter		= m_Filter.c_str();
	m_ofn.nFilterIndex	= 1;				// フィルターの初期位置
	m_ofn.lpstrFile			= s_FullPath;		// ファイル名用文字列バッファ
	m_ofn.lpstrFileTitle= s_FileTitle;		// ファイルのタイトル用文字列バッファ
	m_ofn.lpstrDefExt		= uni_ext.c_str();
	m_ofn.lpstrInitialDir	= m_DefaultDirectry.c_str();
	m_ofn.Flags				  = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	m_ofn.lpstrTitle		= uni_tittle.c_str(); // タイトル

	{
		const String dir = Shell::GetCurrentDirectory();
		ret = ::GetOpenFileName( &m_ofn );

		Shell::SetCurrentDirectory(dir);
	}

	if( ret==0 )
	{
		const DWORD d = ::CommDlgExtendedError();

		//	GetSaveFileName() はエラーだけど、 CommDlgExtendedError() はエラーでない
		//	== キャンセルが押された
		if( d==0 ) { return false; }

	}else
	{
		m_FullPath = String::ConvertUNICODEtoMAID(s_FullPath);
		m_FileName = String::ConvertUNICODEtoMAID(s_FileTitle);
		return true;
	}

	return false;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 「ファイルを保存する」ダイアログボックスを出す
/*!
 	@param  hWnd		[i ]	オーナーハンドル
 	@param  Title		[i ]	ダイアログボックスのタイトルバーに表示される文字列
 	@param  DefExt		[i ]	ダイアログができたときにデフォルトで入っている文字列
 	@param  DefaultName	[i ]	最初からエディットボックスに入っているファイル名
 
	@return ファイルを指定して［OK］ボタンを押すと、trueが返ります
 			ダイアログボックスをキャンセルしたり閉じたりした場合は、false が返ります	
*/
bool FileSelectDialog::ShowSaveDialog( HWND hWnd, const String& Title, const String& DefExt, const String& DefaultName )
{
	BOOL ret;

	const std::wstring uni_tittle = String::ConvertMAIDtoUNICODE(Title);
	const std::wstring uni_ext    = String::ConvertMAIDtoUNICODE(DefExt);
	const std::wstring uni_name   = String::ConvertMAIDtoUNICODE(DefaultName);

	::lstrcpy( s_FileTitle, uni_name.c_str() );
	m_ofn.hwndOwner			= hWnd;
	m_ofn.lpstrFilter		= m_Filter.c_str();
	m_ofn.nFilterIndex		= 1;				// フィルターの初期位置
	m_ofn.lpstrFile			= s_FullPath;		// ファイル名用文字列バッファ
	m_ofn.lpstrFileTitle	= s_FileTitle;	// ファイルのタイトル用文字列バッファ
	m_ofn.lpstrDefExt		= uni_ext.c_str();
	m_ofn.lpstrInitialDir	= m_DefaultDirectry.c_str();
	m_ofn.Flags				= OFN_FILEMUSTEXIST;
	m_ofn.lpstrTitle		= uni_tittle.c_str(); // タイトル

	{
		const String dir = Shell::GetCurrentDirectory();
		ret = ::GetSaveFileName( &m_ofn );
		Shell::SetCurrentDirectory(dir);
	}


	if( ret==0 )
	{
		const DWORD d = ::CommDlgExtendedError();

		//	GetSaveFileName() はエラーだけど、 CommDlgExtendedError() はエラーでない
		//	== キャンセルが押された
		if( d==0 ) { return false; }

	}else
	{
		m_FullPath = String::ConvertUNICODEtoMAID(s_FullPath);
		m_FileName = String::ConvertUNICODEtoMAID(s_FileTitle);

		if( IsFlag(m_ofn.Flags,OFN_EXTENSIONDIFFERENT) )
		{
			//	拡張子が違う

		}

		return true;
	}

	return ret==TRUE;
}

}