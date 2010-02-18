#ifndef maid2_tool_installer_maidinstaller_installprogram_h
#define maid2_tool_installer_maidinstaller_installprogram_h

/*!
 *	\file
 *	\brief	起動時に設定されている情報にアクセスできるクラス
 */


#include"../../../source/auxiliary/string.h"
#include"../../../source/auxiliary/functionresult.h"
#include"uninstallprogram.h"

#include<vector>
#include<windows.h>

struct INSTALLPROGRAM
{
	struct COPYFILE
	{
		int DiskNo;
		Maid::String SrcPath;
		Maid::String DstPath;
		Maid::String MD5;

    COPYFILE()
      :DiskNo(0)
    {}
	};

	struct SHORTCUT
	{
		Maid::String TargetObject;
		Maid::String CreatePath;
		Maid::String Parameters;
	};

  struct REGISTRY
  {
    Maid::String Handle;
		Maid::String SubKey;
    bool  IsKeyDelete;

    struct VALUE
    {
		  Maid::String Name;
		  Maid::String Data;

      enum TYPE
      {
        TYPE_DWORD,
        TYPE_STRING,
      };

      TYPE  Type;
    };
	  std::vector<VALUE> ValueList;

    REGISTRY()
      :IsKeyDelete(false)
    {}
  };

  bool  IsMD5Check;
  Maid::String  UninstallerPath;
  Maid::String  UninstallLogFileName;
	std::vector<COPYFILE>	CopyFileList;
	std::vector<SHORTCUT>	ShortcutList;
	std::vector<REGISTRY> RegistryList;

  UNINSTALLPROGRAM  UninstallOption;

  INSTALLPROGRAM()
    :IsMD5Check(false)
  {}
};

Maid::FUNCTIONRESULT LoadInstallProgram( const Maid::String& FileName, INSTALLPROGRAM& pgr );
Maid::FUNCTIONRESULT LoadInstallProgram( Maid::XMLReader& xml, INSTALLPROGRAM& pgr );
Maid::FUNCTIONRESULT WriteInstallProgram( Maid::XMLWriter& xml, const INSTALLPROGRAM& pgr );


#endif
