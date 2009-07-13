
#ifndef maid2_tool_installer_define_h
#define maid2_tool_installer_define_h


/////////////////////	全体で使う文字列

#define s_INSTALLSTATE_SUCCESS 0  //  インストールが成功した
#define s_INSTALLSTATE_CANCEL 1  //  ユーザーの意思によるインストールのキャンセル
#define s_INSTALLSTATE_ERROR 2  //  インストール中にエラーが起こった


/////////////////////	セットアップで使う文字列
#define s_CONFIG_NAME          (MAIDTEXT("maidconfig.exe"))



/////////////////////	インストールコンフィグで使う文字列

#define s_INSTALLER_NAME          (MAIDTEXT("maidinstaller.exe"))
#define s_INSTALLPROGRAM_FILENAME (MAIDTEXT("installprogram.xml"))

#define s_INSTALLAPP_DIRECTORY (MAIDTEXT("[ProgramFiles]\\maid_install_app"))



/////////////////////	インストーラで使う文字列
#define s_XML_UNINSTALLERPATH (MAIDTEXT("uninstallerpath"))
#define s_XML_UNINSTALLLOGFILENAME (MAIDTEXT("uninstalllogfilename"))
#define s_XML_MD5CHECK (MAIDTEXT("md5check"))

#define s_XML_COPYFILE (MAIDTEXT("copyfile"))
#define s_XML_COPYFILE_DISKNO (MAIDTEXT("diskno"))
#define s_XML_COPYFILE_DESTPATH (MAIDTEXT("destpath"))
#define s_XML_COPYFILE_SOURCEPATH (MAIDTEXT("sourcepath"))
#define s_XML_COPYFILE_MD5 (MAIDTEXT("md5"))

#define s_XML_SHORTCUT (MAIDTEXT("shortcut"))
#define s_XML_SHORTCUT_TARGETOBJECT (MAIDTEXT("targetobject"))
#define s_XML_SHORTCUT_CREATEPATH (MAIDTEXT("createpath"))
#define s_XML_SHORTCUT_PARAMETERS (MAIDTEXT("parameters"))

#define s_XML_REGISTRY (MAIDTEXT("registry"))
#define s_XML_REGISTRY_HANDLE (MAIDTEXT("handle"))
#define s_XML_REGISTRY_SUBKEY (MAIDTEXT("subkey"))
#define s_XML_REGISTRY_SUBKEY_ATTER_DELETE (MAIDTEXT("delete"))
#define s_XML_REGISTRY_VALUE (MAIDTEXT("value"))
#define s_XML_REGISTRY_VALUE_NAME (MAIDTEXT("name"))
#define s_XML_REGISTRY_VALUE_DATA (MAIDTEXT("data"))
#define s_XML_REGISTRY_VALUE_DATA_ATTER_TYPE (MAIDTEXT("type"))

#define s_XML_UNINSTALLOPTION (MAIDTEXT("uninstalloption"))









////////////////////	アンインストーラで使う文字列
#define s_UNINSTALLER_NAME    (MAIDTEXT("maiduninstaller.exe"))
#define s_UNINSTALLERINFONAME (MAIDTEXT("uninstall.xml"))


#define s_UNINSTALLER_DELETEFILE          (MAIDTEXT("deletefile"))
#define s_UNINSTALLER_DELETEFILE_TEXT     (MAIDTEXT("text"))
#define s_UNINSTALLER_DELETEFILE_TARGET   (MAIDTEXT("target"))
#define s_UNINSTALLER_DELETEFOLDER        (MAIDTEXT("deletefolder"))
#define s_UNINSTALLER_DELETEFOLDER_ATTR_FILEDELETE (MAIDTEXT("filedelete"))
#define s_UNINSTALLER_DELETEFOLDER_TEXT   (MAIDTEXT("text"))
#define s_UNINSTALLER_DELETEFOLDER_TARGET (MAIDTEXT("target"))


#define s_UNINSTALLER_DELETEREGISTRY        (MAIDTEXT("deleteregistry"))
#define s_UNINSTALLER_DELETEREGISTRY_ATTR_DELETE (MAIDTEXT("delete"))
#define s_UNINSTALLER_DELETEREGISTRY_HANDLE (MAIDTEXT("handle"))
#define s_UNINSTALLER_DELETEREGISTRY_SUBKEY (MAIDTEXT("subkey"))
#define s_UNINSTALLER_DELETEREGISTRY_VALUE  (MAIDTEXT("value"))

#endif
