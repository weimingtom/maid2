/*!
 	@file
 	@brief	ファイルに保存 or ファイルを開くダイアログを表示します。 
 */

#ifndef maid2_framework_win32_fileselectdialog_h
#define maid2_framework_win32_fileselectdialog_h

#include"../../config/define.h"
#include"../../config/win32.h"
#include"../../auxiliary/string.h"

#include<boost/smart_ptr.hpp>
#include<string>
#include<commdlg.h>

namespace Maid
{
	class FileSelectDialog
	{
	public:
		FileSelectDialog();
		~FileSelectDialog();

		bool ShowOpenDialog( HWND hWnd, const String& Title, const String& DefExt, const String& DefaultName=String() );
		bool ShowSaveDialog( HWND hWnd, const String& Title, const String& DefExt, const String& DefaultName=String() );
		void SetDefaultDirectory( const String& directry );
		void SetFileFilter( const wchar_t* filter );

		const String&	GetFilePath() { return m_FullPath; }
		const String&	GetFileName() { return m_FileName; }

	private:
		OPENFILENAME	m_ofn;
		std::wstring	m_Filter;
		std::wstring	m_DefaultDirectry;
		String	m_FullPath;		//　選択されたパス
		String	m_FileName;		//　選択されたファイル名

	};
}


#endif
