#ifndef maid2_tool_installer_maiduninstaller_uninstallprogram_h
#define maid2_tool_installer_maiduninstaller_uninstallprogram_h

/*!
 *	\file
 *	\brief	起動時に設定されている情報にアクセスできるクラス
 */


#include"../../../source/auxiliary/string.h"
#include"../../../source/auxiliary/functionresult.h"
#include"../../../source/auxiliary/xml.h"

#include<vector>
#include<windows.h>

struct UNINSTALLPROGRAM
{
	struct DELETEFILE
	{
		Maid::String Text;  //  削除していいかの確認 MsgBox。empty() なら表示しない
		Maid::String TargetPath;
	};

	struct DELETEFOLDER
	{
		Maid::String Text;  //  削除していいかの確認 MsgBox。empty() なら表示しない
		Maid::String TargetPath;
    bool  IsAll;  //  フォルダ内のファイルも一括で消すか？
    DELETEFOLDER()
      :IsAll(false)
    {}
	};

  struct DELETEREGISTRY
  {
    Maid::String Handle;
		Maid::String SubKey;
    bool  IsKeyDelete;

	  std::vector<Maid::String> ValueList;

    DELETEREGISTRY()
      :IsKeyDelete(false)
    {}
  };

	std::vector<DELETEFILE>     DeleteFileList;
	std::vector<DELETEFOLDER>	  DeleteFolderList;
	std::vector<DELETEREGISTRY> DeleteRegistryList;
};

Maid::FUNCTIONRESULT LoadUnInstallProgram( const Maid::String& FileName, UNINSTALLPROGRAM& pgr );
Maid::FUNCTIONRESULT LoadUnInstallProgram( Maid::XMLReader& xml, UNINSTALLPROGRAM& pgr );

Maid::FUNCTIONRESULT WriteUnInstallProgram( Maid::XMLWriter& xml, const UNINSTALLPROGRAM& pgr );

#endif
