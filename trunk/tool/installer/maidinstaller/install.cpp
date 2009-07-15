#include"install.h"

#include"../../../source/auxiliary/xml.h"
#include"../../../source/auxiliary/debug/assert.h"
#include"../../../source/storage/fileio/filereadnormal.h"
#include"../../../source/storage/fileio/filewrite.h"
#include"../../../source/storage/fileio/fileoperation.h"
#include"../../../source/framework/win32/shell.h"
#include"../../../source/framework/win32/registry.h"
#include"../../../source/import/md5/md5.h"

#include"resource.h"
#include<io.h>
#include<direct.h>
#include<set>
#include"../define.h"

using namespace Maid;

/*!
 *	\class	Install Install.h
 *	\brief	インストールを行うクラス
\n			インストール自体は別スレッドで行っているので、メインスレッドから常時監視する形になります
 */

static const unt32 COPY_PACET = 1024*256;	//	一回のコピーで実行されるファイルサイズ
static const unt32 MD5CHECKSIZE = 1024*256;	//	一回のコピーで実行されるファイルサイズ
static unt08 COPYBUF[COPY_PACET];		//	パケットコピーで一時的に使われるバッファ
										//	MD5 のチェック時にも使われる






/*	全体処理の進み具合の目安

	・インストール設定の作成(00%-05%)
	・ファイルのコピー      (05%-90%)
	・アンインストール設定  (90%-95%)
	・レジストリの書き込み  (95%-100%)
*/
static const int TOTALPROGRESS_SETUPBEGIN = 0;
static const int TOTALPROGRESS_SETUPEND   = 5;
static const int TOTALPROGRESS_FILECOPYBEGIN  = TOTALPROGRESS_SETUPEND;
static const int TOTALPROGRESS_FILECOPYEND    = 90;
static const int TOTALPROGRESS_SHORTCUTBEGIN = TOTALPROGRESS_FILECOPYEND;
static const int TOTALPROGRESS_SHORTCUTEND   = 93;
static const int TOTALPROGRESS_REGISTORYBEGIN = TOTALPROGRESS_SHORTCUTBEGIN;
static const int TOTALPROGRESS_REGISTORYEND   = 96;
static const int TOTALPROGRESS_UNINSTALLBEGIN = TOTALPROGRESS_REGISTORYBEGIN;
static const int TOTALPROGRESS_UNINSTALLEND   = 100;

static const int TOTALPROGRESS_FINAL = TOTALPROGRESS_UNINSTALLEND;


float PER( int p )
{
	return float(p)/float(TOTALPROGRESS_FINAL);
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! インストールの開始
/*!
 *	この関数が成功したら GetStatus() で状態を見つつ 
\n	STATE_SUCCESS	がくると正常終了
\n	STATE_ERROR		がくると失敗して終了
 *
 *	\param	InstallData	[i ]	インストール設定
 *
 *	\return インストールが開始されると DENRESULT_OK
\n			されなかったら DENRESULT_OK 以外
 */
void Install::BeginInstall( const Install::INSTALLDATA& InstallData )
{
	m_NowState.Status = STATUS::THREADSTATUS_EXECUTING;

	m_InstallData = InstallData;

	m_Thread.SetFunc( MakeThreadObject( &Install::InstallThread, this) );
	m_Thread.Execute();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在の状態を取得する
/*!
 *	BeginInstall() が成功していること
 *
 *	\return 現在のインストール状況
\n			詳しくはヘッダ参照
 */
Install::STATUS Install::GetStatus()
{
	STATUS s;
	{
		ThreadMutexLocker Lock(m_Section);

		s = m_NowState;
	}

	return s;
}


void	Install::SetProgressText( const String& tex )
{
	ThreadMutexLocker Lock(m_Section);
	m_NowState.ProgressText = tex;
}

void	Install::SetNowProgress( float v )
{
	ThreadMutexLocker Lock(m_Section);
	m_NowState.NowProgress = v;
}

void	Install::SetTotalProgress( float v )
{
	ThreadMutexLocker Lock(m_Section);
	m_NowState.TotalProgress = v;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! インストールを中断する
/*!
 *	BeginInstall() が成功していること
 */
void   Install::OnCancel()
{
	m_Thread.Close();
}

Install::FUNCTIONRESULT Install::OnFileCopy( volatile Maid::ThreadController::BRIGEDATA& Brige )
{
  const INSTALLPROGRAM& pgr = *(m_InstallData.pProgram);

  //	・ファイルのコピー & MD5チェック     (05%-90%)
  SetProgressText( MAIDTEXT("インストールを開始します") );
  SetTotalProgress( PER(TOTALPROGRESS_FILECOPYBEGIN) );

  for( int i=0; i<(int)pgr.CopyFileList.size(); ++i )
  {
	  {
		  const float per = float(i) / float(pgr.CopyFileList.size());
		  const int len = TOTALPROGRESS_FILECOPYEND-TOTALPROGRESS_FILECOPYBEGIN;

		  SetTotalProgress( (float(len)*per + float(TOTALPROGRESS_SETUPEND))/float(TOTALPROGRESS_FINAL) );
	  }

	  const INSTALLPROGRAM::COPYFILE& dat = pgr.CopyFileList[i];

	  //	ファイルが存在していなかったらディスク交換を要求する
	  while( true )
	  {
		  if( FileOperation::IsFileExist(dat.SrcPath) ) { break; }

      const String Text = String::PrintFormat( "ディスク %d をドライブに入れてください", dat.DiskNo+1 );

      const int select = Shell::MessageBox( m_InstallData.hWnd, Text, MAIDTEXT("ディスクの交換"), MB_OKCANCEL );

		  if( select==IDOK )
		  {
			  continue;
		  }else
		  {	//	ＯＫが押されなかったらキャンセル扱いで終わる
			  return FUNCTIONRESULT_CANCEL;
		  }
	  }

	  while( true )
	  {
		  //	コピーの開始
		  switch( FileCopy( dat.DstPath, dat.SrcPath, Brige ) )
		  {
		  case FUNCTIONRESULT_OK: { }break;
		  case FUNCTIONRESULT_CANCEL: { return FUNCTIONRESULT_CANCEL;}break;
		  default: { return FUNCTIONRESULT_ERROR; }break;
		  }

		  //	MD5 チェック
		  if( pgr.IsMD5Check )
		  {
			  switch( CheckMD5( dat.DstPath, String::ToLower(dat.MD5), Brige ) )
			  {
			  case FUNCTIONRESULT_OK: { }break;
			  case FUNCTIONRESULT_CANCEL: { InstallCancel(); return FUNCTIONRESULT_CANCEL;}break;
			  case FUNCTIONRESULT_ERROR:
				  {
            const String Text    = MAIDTEXT("コピーしたファイルが破損しています\nもう一度インストールしますか？");
            const String Caption = MAIDTEXT("MD5エラー");

            const int select = Shell::MessageBox( m_InstallData.hWnd, Text, Caption, MB_OKCANCEL );
  					
					  //	ＯＫが押されなかったらキャンセル扱いで終わる
					  if( select==IDOK )	{ continue; }
					  else				{	return FUNCTIONRESULT_CANCEL;	}
				  }break;

			  default: { return FUNCTIONRESULT_ERROR; }break;
			  }
		  }
		  //	ここまでくればＯＫ
		  break;
	  }
  }


  {
	  //	アンインストーラーのコピー
    const String& path = m_InstallData.pProgram->UninstallerPath;

    FileOperation::CreateDirectory( String::GetDirectory(path) );


	  HRSRC hResource = FindResource( NULL, MAKEINTRESOURCE(ID_UNINSTALL1), L"uninstall" );

	  MAID_ASSERT( hResource==NULL, MAIDTEXT("uninstall.exe が見つかりませんでした") );
	  const DWORD ExeSize = SizeofResource( NULL,hResource);

	  HGLOBAL hGlobal = LoadResource( NULL, hResource );
	  void* pExeImage = LockResource( hGlobal );

    {
      FileWrite hFile;
      hFile.Open( path, FileWrite::OPENOPTION_NEW );
      hFile.Write( pExeImage, ExeSize );
    }

	  FreeResource( hGlobal );
  }

	SetTotalProgress( PER(TOTALPROGRESS_FILECOPYEND) );

  return FUNCTIONRESULT_OK;
}


Install::FUNCTIONRESULT Install::OnCreateShortcut( volatile Maid::ThreadController::BRIGEDATA& Brige )
{
  const INSTALLPROGRAM& pgr = *(m_InstallData.pProgram);

  //	ショートカットの作成
  SetProgressText( MAIDTEXT("ショートカットを作成しています") );
  SetTotalProgress( PER(TOTALPROGRESS_SHORTCUTBEGIN) );

  for( int i=0; i<(int)pgr.ShortcutList.size(); ++i )
  {
	  const INSTALLPROGRAM::SHORTCUT& dat = pgr.ShortcutList[i];
	  const String dir = String::GetDirectory(dat.CreatePath);

	  FileOperation::CreateDirectory( dir, m_ExecutingInfo.CreateDirectoryList );
	  Shell::CreateShortCut( dat.CreatePath, dat.TargetObject, dat.Parameters );
  }

  SetTotalProgress( PER(TOTALPROGRESS_SHORTCUTEND) );

  return FUNCTIONRESULT_OK;
}

Install::FUNCTIONRESULT Install::OnCreateRegistry( volatile Maid::ThreadController::BRIGEDATA& Brige )
{
  const INSTALLPROGRAM& pgr = *(m_InstallData.pProgram);

  SetProgressText( MAIDTEXT("インストール情報をレジストリに書き込んでいます") );
  SetTotalProgress( PER(TOTALPROGRESS_REGISTORYBEGIN) );

  for( int i=0; i<(int)pgr.RegistryList.size(); ++i )
  {
	  const INSTALLPROGRAM::REGISTRY& dat = pgr.RegistryList[i];

    Registry hReg;
    HKEY hKey = NULL;

    {
           if( dat.Handle==MAIDTEXT("HKEY_CLASSES_ROOT") ) { hKey = HKEY_CLASSES_ROOT; }
      else if( dat.Handle==MAIDTEXT("HKEY_CURRENT_USER") ) { hKey = HKEY_CURRENT_USER; }
      else if( dat.Handle==MAIDTEXT("HKEY_LOCAL_MACHINE")) { hKey = HKEY_LOCAL_MACHINE;}
      else if( dat.Handle==MAIDTEXT("HKEY_USERS")        ) { hKey = HKEY_USERS;        }
    }

    hReg.Create( hKey, dat.SubKey );

    for( int j=0; j<(int)dat.ValueList.size(); ++j )
    {
      const INSTALLPROGRAM::REGISTRY::VALUE& value = dat.ValueList[j];
      const std::string str = String::ConvertMAIDtoSJIS(value.Name);

      switch( value.Type )
      {
      case INSTALLPROGRAM::REGISTRY::VALUE::TYPE_DWORD:
        {
          const unt v = (unt)String::AtoI(value.Data);
          hReg.SetValue( value.Name, v );
        }break;

      case INSTALLPROGRAM::REGISTRY::VALUE::TYPE_STRING:
        {
          hReg.SetValue( value.Name, value.Data );
        }break;
      }
    }
  }

  SetTotalProgress( PER(TOTALPROGRESS_REGISTORYEND) );

  return FUNCTIONRESULT_OK;
}



Install::FUNCTIONRESULT Install::OnCreateUninstallInfo( volatile Maid::ThreadController::BRIGEDATA& Brige )
{
  //	アンインストールデータの作成
  SetProgressText( MAIDTEXT("アンインストール情報を作成しています") );
  SetTotalProgress( PER(TOTALPROGRESS_UNINSTALLBEGIN) );

  XMLWriter xml;

  {
    XMLWriterAutoDescend(xml, MAIDTEXT("Data") );

    {
      //  削除するファイル
      const INSTALLPROGRAM& pgr = *(m_InstallData.pProgram);
      for( int i=0; i<(int)pgr.CopyFileList.size(); ++i )
      {
        XMLWriterAutoDescend(xml, s_UNINSTALLER_DELETEFILE );
  	    const INSTALLPROGRAM::COPYFILE& dat = pgr.CopyFileList[i];
        xml.AddTag(s_UNINSTALLER_DELETEFILE_TARGET,dat.DstPath);
      }
      for( int i=0; i<(int)pgr.ShortcutList.size(); ++i )
      {
        XMLWriterAutoDescend(xml, s_UNINSTALLER_DELETEFILE );
  	    const INSTALLPROGRAM::SHORTCUT& dat = pgr.ShortcutList[i];
        xml.AddTag(s_UNINSTALLER_DELETEFILE_TARGET, dat.CreatePath);
      }

      {
        //  アンインストール設定ファイルも登録しておく
        XMLWriterAutoDescend(xml, s_UNINSTALLER_DELETEFILE );
        xml.AddTag(s_UNINSTALLER_DELETEFILE_TARGET, pgr.UninstallLogFileName );

      }
      {
        //  アンインストールexeも登録しておく
        XMLWriterAutoDescend(xml, s_UNINSTALLER_DELETEFILE );
        xml.AddTag(s_UNINSTALLER_DELETEFILE_TARGET, pgr.UninstallerPath );
      }
    }

    {
      //  削除するフォルダ
	    std::set<String> s;

	    for( int i=0; i<(int)m_ExecutingInfo.CreateDirectoryList.size(); ++i )
	    {
		    const String& fol = m_ExecutingInfo.CreateDirectoryList[i];
		    s.insert( String::ToLower(fol) );
	    }

	    for( std::set<String>::reverse_iterator ite = s.rbegin();
				    ite!=s.rend(); ++ite )
	    {
        XMLWriterAutoDescend(xml, s_UNINSTALLER_DELETEFOLDER );
        xml.AddTag(s_UNINSTALLER_DELETEFOLDER_TARGET, *ite );
      }
    }

    {
      //  削除するレジストリ
      const INSTALLPROGRAM& pgr = *(m_InstallData.pProgram);
      for( int i=0; i<(int)pgr.RegistryList.size(); ++i )
      {
  	    const INSTALLPROGRAM::REGISTRY& dat = pgr.RegistryList[i];
        XMLWriterAutoDescend( xml, s_UNINSTALLER_DELETEREGISTRY );

        xml.AddTag(s_UNINSTALLER_DELETEREGISTRY_HANDLE,dat.Handle);

        {
          xml.SetElementName( s_UNINSTALLER_DELETEREGISTRY_SUBKEY );
          xml.SetValueText( dat.SubKey );
          if( dat.IsKeyDelete ) { xml.AddAttribute( s_UNINSTALLER_DELETEREGISTRY_ATTR_DELETE, MAIDTEXT("yes") ); }
          xml.NextNode();
        }

        for( int j=0; j<(int)dat.ValueList.size(); ++j )
        {
          xml.AddTag(s_UNINSTALLER_DELETEREGISTRY_VALUE,dat.ValueList[j].Name);
        }
      }
    }
  }

  {
    const INSTALLPROGRAM& pgr = *(m_InstallData.pProgram);
    XMLWriterAutoDescend( xml, s_XML_UNINSTALLOPTION );

    const Maid::FUNCTIONRESULT ret = WriteUnInstallProgram( xml, pgr.UninstallOption );
    if( ret==Maid::FUNCTIONRESULT_ERROR ) { return Install::FUNCTIONRESULT_ERROR; }
  }

  {
    //  情報がそろったので、書き込み
    const INSTALLPROGRAM& pgr = *(m_InstallData.pProgram);

    std::string text;

    xml.Save( text );

    FileOperation::CreateDirectory( String::GetDirectory(pgr.UninstallLogFileName) );

    FileWrite hFile;

    hFile.Open( pgr.UninstallLogFileName, FileWrite::OPENOPTION_NEW );
    hFile.Write( text.c_str(), text.length() );
  }

  SetTotalProgress( PER(TOTALPROGRESS_UNINSTALLEND) );
  return FUNCTIONRESULT_OK;
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 実際にインストールを行うスレッド
/*!
 *	\param	Brige	[i ]	渡されるデータ
 */
unt32	Install::InstallThread( volatile ThreadController::BRIGEDATA& Brige )
{
	Shell::CoInitialize();

	{
		m_ExecutingInfo.TotalFileSize = 0.0f;
		m_ExecutingInfo.AccessFileSize = 0.0f;

		SetProgressText ( String() );
		SetNowProgress  ( 0 );
		SetTotalProgress( 0 );
	}


	{
		//	・インストール設定の作成(00%-05%)
		SetProgressText( MAIDTEXT("インストールするための情報を集めています") );
		SetTotalProgress( (float)TOTALPROGRESS_SETUPBEGIN );
		SetTotalProgress( PER(TOTALPROGRESS_SETUPEND) );
	}


  {
    const FUNCTIONRESULT ret = OnFileCopy(Brige);

    switch( ret )
    {
    case FUNCTIONRESULT_OK: { }break;
    case FUNCTIONRESULT_CANCEL: { InstallCancel(); return 0; }break;
    case FUNCTIONRESULT_ERROR:  { InstallError( MAIDTEXT("ファイルコピー中にエラー発生") ); return 0; }break;
    }
  }

  {
    const FUNCTIONRESULT ret = OnCreateShortcut(Brige);

    switch( ret )
    {
    case FUNCTIONRESULT_OK: { }break;
    case FUNCTIONRESULT_CANCEL: { InstallCancel(); return 0; }break;
    case FUNCTIONRESULT_ERROR:  { InstallError( MAIDTEXT("ショートカット作成中にエラー発生") ); return 0; }break;
    }

  }

  {
    const FUNCTIONRESULT ret = OnCreateRegistry(Brige);

    switch( ret )
    {
    case FUNCTIONRESULT_OK: { }break;
    case FUNCTIONRESULT_CANCEL: { InstallCancel(); return 0; }break;
    case FUNCTIONRESULT_ERROR:  { InstallError( MAIDTEXT("レジストリ登録中にエラー発生") ); return 0; }break;
    }
  }

  {
    const FUNCTIONRESULT ret = OnCreateUninstallInfo(Brige);

    switch( ret )
    {
    case FUNCTIONRESULT_OK: { }break;
    case FUNCTIONRESULT_CANCEL: { InstallCancel(); return 0; }break;
    case FUNCTIONRESULT_ERROR:  { InstallError( MAIDTEXT("アンインストール情報作成中にエラー発生") ); return 0; }break;
    }
  }


	SetProgressText(MAIDTEXT("インストールが完了しました"));
	SetNowProgress(1);
	SetTotalProgress( 1 );


	m_NowState.Status = STATUS::THREADSTATUS_SUCCESS;
	return 0;
}

void Install::InstallCancel()
{
	SetProgressText( MAIDTEXT("インストールはキャンセルされました") );
	m_NowState.Status = STATUS::THREADSTATUS_CANCEL;
}

void Install::InstallError( const String& comment )
{
	SetProgressText( MAIDTEXT("インストール中にエラーが発生しました\nエラー内容：") + comment );
	m_NowState.Status = STATUS::THREADSTATUS_ERROR;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルのコピー
/*!
 *	\param	DstFileName		[i ]	作成するディレクトリ名
 *	\param	SrcFileName		[i ]	作成するディレクトリ名
 *	\param	Brige			[i ]	外部スレッドからの命令
 *
 *	\return	コピーに成功したら FUNCTIONRESULT_OK
\n			外部からキャンセル命令がでたら FUNCTIONRESULT_CANCEL
\n			普通のエラーは FUNCTIONRESULT_ERROR
 */
Install::FUNCTIONRESULT Install::FileCopy( const String& DstFileName,  const String& SrcFileName, volatile ThreadController::BRIGEDATA& Brige  )
{
	//	ディレクトリを作る
	FileOperation::CreateDirectory( String::GetDirectory(DstFileName), m_ExecutingInfo.CreateDirectoryList );


	FileWrite hDst;
	FileReadNormal hSrc(SrcFileName);

	hDst.Open(DstFileName,FileWrite::OPENOPTION_NEW);
	hSrc.Open();

	SetProgressText( SrcFileName+MAIDTEXT("から\n") + DstFileName + MAIDTEXT("にコピーしています") );

	const unt32 FileSize = hSrc.GetSize();

	unt32	LimitSize = FileSize;
	unt32	WriteSize = 0;

	while( LimitSize!=0 )
	{
		if( Brige.IsExit )
		{	//	中止命令が来ていたら
			return FUNCTIONRESULT_CANCEL;
		}

    const unt32 Size = (unt32)std::min(COPY_PACET,LimitSize);

		hSrc.Read( COPYBUF, Size );
		hDst.Write( COPYBUF, Size );


		LimitSize -= Size;
		WriteSize += Size;

		m_ExecutingInfo.AccessFileSize += Size;

		{	//	浮動小数点で計算しておかないと、オーバーフローすることがある
			float write = (float)(int)WriteSize;
			float total = (float)(int)FileSize;

			SetNowProgress( write/total );
		}
	}

	return FUNCTIONRESULT_OK;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! MD5 チェックを行う
/*!
 *	\param	DstFileName		[i ]	チェックを行うファイル名
 *	\param	MD5Str			[i ]	↑のファイルのMD5（小文字であること）
 *	\param	Brige			[i ]	外部スレッドからの命令
 *
 *	\return	正しいファイルであったら FUNCTIONRESULT_OK
\n			外部からキャンセル命令がでたら FUNCTIONRESULT_CANCEL
\n			MD5 が合わなかったら FUNCTIONRESULT_ERROR
 */
Install::FUNCTIONRESULT	Install::CheckMD5( const String& DstFileName, const String& MD5Str, volatile ThreadController::BRIGEDATA& Brige )
{
	if( MD5Str.empty() ) { return FUNCTIONRESULT_OK; }

	FileReadNormal hSrc(DstFileName);

	hSrc.Open();


	SetProgressText( DstFileName + MAIDTEXT("のファイルチェックを行っています") );

	{
		MD5LIB::MD5_CTX context;

		MD5LIB::MD5Init( &context );

		unt32	LimitSize = hSrc.GetSize();
		unt32	ReadSize = 0;

		while( LimitSize!=0 )
		{
			if( Brige.IsExit )
			{	//	中止命令が来ていたら
				return FUNCTIONRESULT_CANCEL;
			}

      const unt32 Size = (unt32)std::min(MD5CHECKSIZE,LimitSize);

			hSrc.Read( COPYBUF, MD5CHECKSIZE );

			MD5LIB::MD5Update( &context, COPYBUF, Size );

			LimitSize -= Size;
			ReadSize += Size;
			m_ExecutingInfo.AccessFileSize += Size;

			{	//	浮動小数点で計算しておかないと、オーバーフローすることがある
				float32 write = (float32)(int32)ReadSize;
				float32 total = (float32)(int32)hSrc.GetSize();

				SetNowProgress( write/total );
			}
		}


		String NewMD5;
		{
			unsigned char digest[16];

			MD5LIB::MD5Final( digest, &context );

			for( unt32 i=0; i<16; ++i )
			{
				char buf[32];
				sprintf( buf, "%02x", digest[i] );

				NewMD5 += MAIDTEXT(buf);
			}
		}
		if( MD5Str != String::ToLower(NewMD5) )
		{
			return FUNCTIONRESULT_ERROR;
		}

		::Sleep(1);
	}


	return FUNCTIONRESULT_OK;
}
