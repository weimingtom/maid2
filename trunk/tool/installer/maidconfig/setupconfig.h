#ifndef maid2_tool_installer_maidconfig_setupconfig_h
#define maid2_tool_installer_maidconfig_setupconfig_h


/*!
 *	\file
 *	\brief	起動時に設定されている情報にアクセスできるクラス
 */


#include"../../../source/auxiliary/string.h"
#include"../../../source/auxiliary/xml.h"
#include"../installprogram.h"

#include<string>
#include<vector>
#include<map>

class SetupConfig
{
public:
  Maid::FUNCTIONRESULT Initialize( const Maid::String& FilePath );

	Maid::String GetMutexName()const;
	Maid::String GetWindowCaption()const;
	Maid::String GetGUID()const;
	Maid::String GetUninstallTittle()const;
	Maid::String GetDefaultTarget()const;

  struct INSTALLTYPE
  {
    Maid::String  Name;
    Maid::String  Comment;
    INSTALLPROGRAM  Program;
  };

	const INSTALLTYPE& GetInstallType( int no )const;
	int			GetInstallTypeCount()const;

	struct EXECUTEINFO
	{
		Maid::String CheckComment;
		Maid::String ExecutePath;
		Maid::String Parameters;
	};

	EXECUTEINFO GetExecuteInfo()const;

  INSTALLPROGRAM CreateInstallProgram( int no, const Maid::String& InstallFoler, bool IsMD5Check );


private:
	Maid::String ConvertText( const Maid::String& t )const;
	void ConvertTextThis( Maid::String& t )const;
	void AddConvertTable( const Maid::String& index, const Maid::String& data );

	void ReadLastExec( Maid::XMLReader& reader );
	void ReadInstall( Maid::XMLReader& reader );
private:
	Maid::String	m_MutexName;
	Maid::String	m_WindowCaption;
	Maid::String	m_GUID;
	Maid::String	m_UninstallTittle;
	Maid::String	m_DefaultTarget;

	std::vector<INSTALLTYPE>	m_InstallType;
	EXECUTEINFO	m_ExecuteInfo;

	typedef std::map<Maid::String,Maid::String> CANVERTTABLE;
	CANVERTTABLE	m_ConvertTable;
	Maid::String	m_InstallFolder;
};

#endif
