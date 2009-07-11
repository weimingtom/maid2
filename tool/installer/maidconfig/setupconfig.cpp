
#include"setupconfig.h"

#include"../../../source/framework/win32/shell.h"
#include"../../../source/auxiliary/debug/assert.h"
#include"../../../source/storage/fileio/filereadnormal.h"
#include"../define.h"

#include<map>

using namespace Maid;


#define	s_TABLE_PROGRAMFILES  (MAIDTEXT("programfiles"))
#define	s_TABLE_WINDOWS (MAIDTEXT("windows"))
#define	s_TABLE_USERPROGRAMMENU (MAIDTEXT("userprogrammenu"))
#define	s_TABLE_USERDESKTOP (MAIDTEXT("userdesktop"))
#define	s_TABLE_USERAPPDATA (MAIDTEXT("userappdata"))
#define	s_TABLE_COMMONPROGRAMMENU (MAIDTEXT("commonprogrammenu"))
#define	s_TABLE_COMMONDESKTOP (MAIDTEXT("commondesktop"))
#define	s_TABLE_COMMONAPPDATA (MAIDTEXT("commonappdata"))

#define	s_TABLE_UNINSTALLER (MAIDTEXT("uninstaller"))
#define	s_TABLE_UNINSTALLER_COMMAND (MAIDTEXT("uninstallercommand"))
#define	s_TABLE_SOURCEFOLDER (MAIDTEXT("sourcefolder"))
#define	s_TABLE_TARGETFOLDER (MAIDTEXT("targetfolder"))



#define s_XML_MUTEXNAME (MAIDTEXT("mutexname"))
#define s_XML_CAPTION   (MAIDTEXT("caption"))
#define s_XML_GUID (MAIDTEXT("guid"))
#define s_XML_UNINSTALLTITTLE (MAIDTEXT("uninstalltittle"))
#define s_XML_TARGETREGISTRY (MAIDTEXT("targetregistry"))
#define s_XML_DEFAULTTARGET (MAIDTEXT("defaultfolder"))
#define s_XML_LASTEXECUTE (MAIDTEXT("lastexecute"))
#define s_XML_LASTEXECUTE_CHECKCOMMENT (MAIDTEXT("checkcomment"))
#define s_XML_LASTEXECUTE_EXECUTEPATH (MAIDTEXT("executepath"))
#define s_XML_LASTEXECUTE_PARAMETERS (MAIDTEXT("parameters"))
#define s_XML_INSTALLTYPE (MAIDTEXT("installtype"))
#define s_XML_INSTALLTYPE_INSTALLTYPENAME (MAIDTEXT("name"))
#define s_XML_INSTALLTYPE_INSTALLTYPECOMMENT (MAIDTEXT("comment"))
#define s_XML_INSTALLTYPE_CONFIG (MAIDTEXT("config"))




FUNCTIONRESULT SetupConfig::Initialize( const String& FilePath )
{
	{
		//	固定パスの設定
    const String ProgramFiles  = Shell::GetProgramFilesFolder(NULL);
		const String Windows       = Shell::GetWindowsFolder();
		const String UserDesktop   = Shell::GetUserDeskTopFolder(NULL);
		const String UserProgramMenu = Shell::GetUserProgramMenuFolder(NULL);
		const String UserAppData     = Shell::GetUserApplicationDataFolder(NULL);

		const String ComDesktop     = Shell::GetCommonDeskTopFolder(NULL);
		const String ComProgramMenu = Shell::GetCommonProgramMenuFolder(NULL);
		const String ComAppData     = Shell::GetCommonApplicationDataFolder(NULL);

		String Source  = String::GetDirectory(FilePath);

		while( true )
		{
			const unt32 c = Source[Source.length()-1];
			if( c!='\\' && c!='/' ) { break; }

			Source.resize( Source.length()-1 );
		}

		AddConvertTable( s_TABLE_PROGRAMFILES,    ProgramFiles );
		AddConvertTable( s_TABLE_WINDOWS,         Windows );
		AddConvertTable( s_TABLE_USERPROGRAMMENU, UserProgramMenu );
		AddConvertTable( s_TABLE_USERDESKTOP,     UserDesktop );
		AddConvertTable( s_TABLE_USERAPPDATA,     UserAppData );
		AddConvertTable( s_TABLE_COMMONPROGRAMMENU, ComProgramMenu );
		AddConvertTable( s_TABLE_COMMONDESKTOP,     ComDesktop );
		AddConvertTable( s_TABLE_COMMONAPPDATA,     ComAppData );

		AddConvertTable( s_TABLE_UNINSTALLER,  Windows + MAIDTEXT("\\") + s_UNINSTALLER_NAME );

		AddConvertTable( s_TABLE_SOURCEFOLDER,  Source);
	}


  Maid::XMLDocument doc;
  {
    FileReadNormal hFile(FilePath);

    if( hFile.Open()==IFileRead::OPENRESULT_ERROR )
    {
      ::MessageBox( NULL, L"設定ファイルが見つかりません", L"エラー", MB_OK );	
      return FUNCTIONRESULT_ERROR; 
    }

    std::string str;

    str.resize( hFile.GetSize() );
    hFile.Read( (void*)str.c_str(), hFile.GetSize() );

    if( doc.Parse(str)==FUNCTIONRESULT_ERROR )
    {
      ::MessageBox( NULL, L"設定ファイルの読み込みに失敗", L"エラー", MB_OK );	
      return FUNCTIONRESULT_ERROR; 
    }
  }

  {
    XMLReader xml;

    xml.Reset( doc );
    XMLReaderAutoDescend(xml);

	  while( true )
	  {
      if( xml.IsEndNode() ) { break; }

		  const String NodeName = xml.GetNodeName();

		  if( NodeName==s_XML_MUTEXNAME )
		  {
			  m_MutexName = xml.GetString();
		  }
		  else if( NodeName==s_XML_CAPTION )
		  {
			  m_WindowCaption = xml.GetString();
		  }
		  else if( NodeName==s_XML_GUID )
		  {
			  m_GUID = String::ToLower(xml.GetString());
		  }
		  else if( NodeName==s_XML_UNINSTALLTITTLE )
		  {
			  m_UninstallTittle = xml.GetString();
		  }
		  else if( NodeName==s_XML_DEFAULTTARGET )
		  {
			  m_DefaultTarget = ConvertText(xml.GetString());
		  }
		  else if( NodeName==s_XML_LASTEXECUTE )
		  {
			  XMLReaderAutoDescend(xml);
			  ReadLastExec( xml );
		  }
		  else if( NodeName==s_XML_INSTALLTYPE )
		  {
			  XMLReaderAutoDescend(xml);
			  ReadInstall( xml );
		  }
		  xml.NextNode();
	  } 
  }

  return FUNCTIONRESULT_OK;
}



void SetupConfig::ReadLastExec( XMLReader& reader )
{
	while( true )
	{
		if( reader.IsEndNode() ) { break; }

		const String Name = reader.GetNodeName();

		if( Name==s_XML_LASTEXECUTE_CHECKCOMMENT )
		{
			m_ExecuteInfo.CheckComment = reader.GetString();
		}
		else if( Name==s_XML_LASTEXECUTE_EXECUTEPATH )
		{
			m_ExecuteInfo.ExecutePath = reader.GetString();
		}
		else if( Name==s_XML_LASTEXECUTE_PARAMETERS )
		{
			m_ExecuteInfo.Parameters = reader.GetString();
		}

		reader.NextNode();
	}
}


void SetupConfig::ReadInstall( XMLReader& xml )
{
	INSTALLTYPE dat;
	while( true )
	{
		if( xml.IsEndNode() ) { break; }

		const String Name = xml.GetNodeName();

		if( Name==s_XML_INSTALLTYPE_INSTALLTYPENAME )
		{
			dat.Name = xml.GetString();
		}
		else if( Name==s_XML_INSTALLTYPE_INSTALLTYPECOMMENT )
		{
			dat.Comment = xml.GetString();
		}
		else if( Name==s_XML_INSTALLTYPE_CONFIG )
    {
		  XMLReaderAutoDescend(xml);
      LoadInstallProgram( xml, dat.Program );
    }
		xml.NextNode();
	}

	m_InstallType.push_back(dat);
}




String SetupConfig::GetMutexName()const		{ return m_MutexName; }
String SetupConfig::GetWindowCaption()const	{ return m_WindowCaption; }
String SetupConfig::GetGUID()const				{ return m_GUID; }
String SetupConfig::GetUninstallTittle()const	{ return m_UninstallTittle; }
String SetupConfig::GetDefaultTarget()const{ return m_DefaultTarget; }




const SetupConfig::INSTALLTYPE& SetupConfig::GetInstallType( int no )const
{
	MAID_ASSERT( !(no<(int)m_InstallType.size()), "範囲オーバー" );
	return m_InstallType[no];
}

int			SetupConfig::GetInstallTypeCount()const
{
	return (int)m_InstallType.size();
}


SetupConfig::EXECUTEINFO SetupConfig::GetExecuteInfo()const
{
  EXECUTEINFO ret = m_ExecuteInfo;

  ConvertTextThis(ret.CheckComment);
  ConvertTextThis(ret.ExecutePath);
  ConvertTextThis(ret.Parameters);

	return ret;
}


INSTALLPROGRAM SetupConfig::CreateInstallProgram( int no, const Maid::String& InstallFoler, bool IsMD5Check )
{
	AddConvertTable( s_TABLE_TARGETFOLDER,  InstallFoler );
	AddConvertTable( s_TABLE_UNINSTALLER_COMMAND,  MAIDTEXT("\"") + InstallFoler + MAIDTEXT("\\") + s_UNINSTALLERINFONAME + MAIDTEXT("\"") );

  INSTALLPROGRAM ret = GetInstallType(no).Program;

  const String UninstallerPath = s_INSTALLAPP_DIRECTORY + MAIDTEXT("\\") + s_UNINSTALLER_NAME;
  const String UninstallLog    = MAIDTEXT("[TargetFolder]\\") + s_UNINSTALLERINFONAME;

  ret.UninstallerPath = ConvertText(UninstallerPath);
  ret.UninstallLogFileName = ConvertText(UninstallLog);
  ret.IsMD5Check = IsMD5Check;


  {
    for( int i=0; i<(int)ret.CopyFileList.size(); ++i )
    {
      INSTALLPROGRAM::COPYFILE& dat = ret.CopyFileList[i];
      ConvertTextThis(dat.SrcPath);
      ConvertTextThis(dat.DstPath);
    }

    for( int i=0; i<(int)ret.ShortcutList.size(); ++i )
    {
      INSTALLPROGRAM::SHORTCUT& dat = ret.ShortcutList[i];
      ConvertTextThis(dat.TargetObject);
      ConvertTextThis(dat.CreatePath);
      ConvertTextThis(dat.Parameters);
    }

    for( int i=0; i<(int)ret.RegistryList.size(); ++i )
    {
      INSTALLPROGRAM::REGISTRY& dat = ret.RegistryList[i];
      ConvertTextThis(dat.Handle);
      ConvertTextThis(dat.SubKey);

      for( int j=0; j<(int)dat.ValueList.size(); ++j )
      {
        INSTALLPROGRAM::REGISTRY::VALUE& v = dat.ValueList[j];
        ConvertTextThis(v.Name);
        ConvertTextThis(v.Data);
      }
    }

    {
      INSTALLPROGRAM::REGISTRY reg;

      reg.Handle = MAIDTEXT("HKEY_LOCAL_MACHINE");
      reg.SubKey = MAIDTEXT("SoftWare\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\") + GetGUID();
      reg.IsKeyDelete = true;

      {
        INSTALLPROGRAM::REGISTRY::VALUE val;

        val.Name = MAIDTEXT("DisplayName");
        val.Data = GetUninstallTittle();
        val.Type = INSTALLPROGRAM::REGISTRY::VALUE::TYPE_STRING;
        reg.ValueList.push_back(val);
      }
      {
        INSTALLPROGRAM::REGISTRY::VALUE val;

        val.Name = MAIDTEXT("UninstallString");
        val.Data = MAIDTEXT("\"") + ret.UninstallerPath + MAIDTEXT("\" \"") + ret.UninstallLogFileName + MAIDTEXT("\"");
        val.Type = INSTALLPROGRAM::REGISTRY::VALUE::TYPE_STRING;
        reg.ValueList.push_back(val);
      }

      ret.RegistryList.push_back(reg);
    }
  }

  {
    UNINSTALLPROGRAM& un = ret.UninstallOption;

    for( int i=0; i<(int)un.DeleteFileList.size(); ++i )
    {
      UNINSTALLPROGRAM::DELETEFILE& dat = un.DeleteFileList[i];
      ConvertTextThis(dat.Text);
      ConvertTextThis(dat.TargetPath);
    }

    for( int i=0; i<(int)un.DeleteFolderList.size(); ++i )
    {
      UNINSTALLPROGRAM::DELETEFOLDER& dat = un.DeleteFolderList[i];
      ConvertTextThis(dat.Text);
      ConvertTextThis(dat.TargetPath);
    }

    for( int i=0; i<(int)un.DeleteRegistryList.size(); ++i )
    {
      UNINSTALLPROGRAM::DELETEREGISTRY& dat = un.DeleteRegistryList[i];
      ConvertTextThis(dat.Handle);
      ConvertTextThis(dat.SubKey);

      for( int j=0; j<(int)dat.ValueList.size(); ++j )
      {
        ConvertTextThis(dat.ValueList[j]);
      }
    }
  }



  return ret;
}


String SetupConfig::ConvertText( const String& src )const
{
	String ret;

	for( int i=0; i<(int)src.length(); ++i )
	{
		unt32 c = src[i];

		if( c!='[' )
		{
			ret += c;
		}else
		{
			String name;

			++i;
			for( ; i<(int)src.length(); ++i )
			{
				unt32 c = src[i];
				if( c==']' )
				{
					CANVERTTABLE::const_iterator ite = m_ConvertTable.find(String::ToLower(name));

					MAID_ASSERT( ite==m_ConvertTable.end(), MAIDTEXT("変換テーブルが見つかりませんでした ") << name );
					ret += ite->second;
					break;
				}else
				{
					name += c;
				}
			}
		}
	}

	return ret;
}

void SetupConfig::ConvertTextThis( String& t )const
{
	t = ConvertText(t);
}

void SetupConfig::AddConvertTable( const String& index, const String& data )
{
	m_ConvertTable[index] = data;
}

