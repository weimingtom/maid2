/*!
 *	\file
 *	\brief	インストール作業を行うクラス
 *
 */


#ifndef maid2_tool_installer_maidinstaller_install_h
#define maid2_tool_installer_maidinstaller_install_h

#include"../../../source/auxiliary/thread.h"
#include"../../../source/auxiliary/string.h"

#include"../installprogram.h"
#include<string>
#include<vector>


class Install
{
public:
	struct INSTALLDATA
	{
		const INSTALLPROGRAM*	pProgram;
		HWND				hWnd;       //	ウィンドウハンドル（エラーが起きたときなどに使用）

    INSTALLDATA( const INSTALLPROGRAM* pgr, HWND h )
      :pProgram(pgr)
      ,hWnd(h)
    {
    }
    INSTALLDATA()
      :pProgram(NULL)
      ,hWnd(NULL)
    {}
	};

	void BeginInstall( const INSTALLDATA& InstallData );

	struct STATUS
	{
		enum THREADSTATUS
		{
			THREADSTATUS_EXECUTING,	//!<	実行中
			THREADSTATUS_ERROR,     //!<	エラーが起きた
			THREADSTATUS_SUCCESS,		//!<	成功して終わった
			THREADSTATUS_CANCEL,		//!<	キャンセル命令がでて終了した
		};

		THREADSTATUS	Status;

		Maid::String ProgressText;
		float NowProgress;
		float TotalProgress;
	};

	STATUS GetStatus();
	void   OnCancel();

private:
	unt		InstallThread( volatile Maid::ThreadController::BRIGEDATA& Brige );

private:
	void InstallCancel();
	void InstallError( const Maid::String& comment );

	enum FUNCTIONRESULT
	{
		FUNCTIONRESULT_OK,
		FUNCTIONRESULT_CANCEL,
		FUNCTIONRESULT_ERROR,
	};

	FUNCTIONRESULT	FileCopy( const Maid::String& DstFileName, const Maid::String& SrcFileName, volatile Maid::ThreadController::BRIGEDATA& Brige );
	FUNCTIONRESULT	CheckMD5( const Maid::String& DstFileName, const Maid::String& MD5Str, volatile Maid::ThreadController::BRIGEDATA& Brige );

	void	SetProgressText( const Maid::String& );
	void	SetNowProgress( float );
	void	SetTotalProgress( float );

  FUNCTIONRESULT OnFileCopy( volatile Maid::ThreadController::BRIGEDATA& Brige );
  FUNCTIONRESULT OnCreateShortcut( volatile Maid::ThreadController::BRIGEDATA& Brige );
  FUNCTIONRESULT OnCreateRegistry( volatile Maid::ThreadController::BRIGEDATA& Brige );
  FUNCTIONRESULT OnCreateUninstallInfo( volatile Maid::ThreadController::BRIGEDATA& Brige );

private:
	INSTALLDATA	m_InstallData;

private:
	Maid::ThreadMutex	m_Section;
	Maid::ThreadController	m_Thread;

	STATUS	m_NowState;


	struct ExecutingInfo	//	インストール実行中に参照される情報
	{
		float	TotalFileSize;	//	インストールする合計容量//	4Gを超えることもあるのでfloat型にしておく
		float	AccessFileSize;	//	いままでに書き込んだ(読み込んだ)サイズ

		std::vector<Maid::String> CreateDirectoryList;
	};

	ExecutingInfo	m_ExecutingInfo;
};

#endif

