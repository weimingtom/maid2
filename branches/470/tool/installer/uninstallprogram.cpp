#include"uninstallprogram.h"

#include"../../../source/storage/fileio/filereadnormal.h"
#include"define.h"

using namespace Maid;


Maid::FUNCTIONRESULT LoadDeleteFile( XMLReader& xml, UNINSTALLPROGRAM& pgr );
Maid::FUNCTIONRESULT LoadDeleteFolder( XMLReader& xml, UNINSTALLPROGRAM& pgr );
Maid::FUNCTIONRESULT LoadDeleteRegistry( XMLReader& xml, UNINSTALLPROGRAM& pgr );


Maid::FUNCTIONRESULT LoadUnInstallProgram( const Maid::String& FileName, UNINSTALLPROGRAM& pgr )
{
  Maid::XMLDocument doc;
  {
    FileReadNormal hFile(FileName);

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

  XMLReader xml;

  xml.Reset( doc );
  XMLReaderAutoDescend(xml);

  return LoadUnInstallProgram( xml, pgr );
}

Maid::FUNCTIONRESULT LoadUnInstallProgram( XMLReader& xml, UNINSTALLPROGRAM& pgr )
{
  while( true )
  {
    if( xml.IsEndNode() ) { break; }

    const String NodeName = String::ToLower(xml.GetNodeName());

    if( NodeName==s_UNINSTALLER_DELETEFILE ) 
    {
      const FUNCTIONRESULT ret = LoadDeleteFile(xml,pgr);
      if( ret==FUNCTIONRESULT_ERROR )
      {
        ::MessageBox( NULL, L"LoadDeleteFile()の失敗", L"エラー", MB_OK );	
        return FUNCTIONRESULT_ERROR; 
      }
    }
    else if( NodeName==s_UNINSTALLER_DELETEFOLDER )
    {
      const FUNCTIONRESULT ret = LoadDeleteFolder(xml,pgr);
      if( ret==FUNCTIONRESULT_ERROR )
      {
        ::MessageBox( NULL, L"LoadDeleteFolder()の失敗", L"エラー", MB_OK );	
        return FUNCTIONRESULT_ERROR; 
      }
    }
    else if( NodeName==s_UNINSTALLER_DELETEREGISTRY )
    {
      const FUNCTIONRESULT ret = LoadDeleteRegistry(xml,pgr);
      if( ret==FUNCTIONRESULT_ERROR )
      {
        ::MessageBox( NULL, L"LoadDeleteRegistry()の失敗", L"エラー", MB_OK );	
        return FUNCTIONRESULT_ERROR; 
      }
    }

    xml.NextNode();
  }

  return FUNCTIONRESULT_OK;
}



Maid::FUNCTIONRESULT LoadDeleteFile( XMLReader& xml, UNINSTALLPROGRAM& pgr )
{
  UNINSTALLPROGRAM::DELETEFILE data;

  xml.AscendNode();
  while( true )
  {
    if( xml.IsEndNode() ) { break; }

    const String NodeName = String::ToLower(xml.GetNodeName());
    if( NodeName==s_UNINSTALLER_DELETEFILE_TARGET )
    {
      data.TargetPath = xml.GetString();
    }
    else if( NodeName==s_UNINSTALLER_DELETEFILE_TEXT )
    {
      data.Text = xml.GetString();
    }
    xml.NextNode();
  }
  xml.DescendNode();

  pgr.DeleteFileList.push_back(data);

  return FUNCTIONRESULT_OK;
}

Maid::FUNCTIONRESULT LoadDeleteFolder( XMLReader& xml, UNINSTALLPROGRAM& pgr )
{
  UNINSTALLPROGRAM::DELETEFOLDER data;
  const String str = xml.GetAttribute( s_UNINSTALLER_DELETEFOLDER_ATTR_FILEDELETE ).GetStr();
  data.IsAll = String::ToLower(str)==MAIDTEXT("yes");

  xml.AscendNode();

  while( true )
  {
    if( xml.IsEndNode() ) { break; }

    const String NodeName = String::ToLower(xml.GetNodeName());
    if( NodeName==s_UNINSTALLER_DELETEFOLDER_TEXT )
    {
      data.Text = xml.GetString();
    }
    else if( NodeName==s_UNINSTALLER_DELETEFOLDER_TARGET )
    {
      data.TargetPath = xml.GetString();
    }
    xml.NextNode();
  }
  xml.DescendNode();

  pgr.DeleteFolderList.push_back(data);

  return FUNCTIONRESULT_OK;
}


Maid::FUNCTIONRESULT LoadDeleteRegistry( XMLReader& xml, UNINSTALLPROGRAM& pgr )
{
  xml.AscendNode();
  UNINSTALLPROGRAM::DELETEREGISTRY data;
  while( true )
  {
    if( xml.IsEndNode() ) { break; }

    const String NodeName = String::ToLower(xml.GetNodeName());
    if( NodeName==s_UNINSTALLER_DELETEREGISTRY_HANDLE )
    {
      data.Handle = xml.GetString();
    }
    else if( NodeName==s_UNINSTALLER_DELETEREGISTRY_SUBKEY ) 
    {
      data.SubKey   = xml.GetString();
      const String del = String::ToLower( xml.GetAttribute( s_UNINSTALLER_DELETEREGISTRY_ATTR_DELETE ).GetStr() );
      data.IsKeyDelete = del==MAIDTEXT("yes");
    }
    else if( NodeName==s_UNINSTALLER_DELETEREGISTRY_VALUE )
    {
      const String str = xml.GetString();
      data.ValueList.push_back(str);
    }

    xml.NextNode();
  }
  xml.DescendNode();

  pgr.DeleteRegistryList.push_back(data);

  return FUNCTIONRESULT_OK;
}

FUNCTIONRESULT WriteUnInstallProgram( XMLWriter& xml, const UNINSTALLPROGRAM& pgr )
{
  {
    for( int i=0; i<(int)pgr.DeleteFileList.size(); ++i )
    {
      const UNINSTALLPROGRAM::DELETEFILE& dat = pgr.DeleteFileList[i];

      XMLWriterAutoDescend( xml, s_UNINSTALLER_DELETEFILE );

      if( !dat.TargetPath.empty() ) { xml.AddTag( s_UNINSTALLER_DELETEFILE_TARGET, dat.TargetPath ); }
      if( !dat.Text.empty() ) { xml.AddTag( s_UNINSTALLER_DELETEFILE_TEXT, dat.Text ); }
    }
  }
  {
    for( int i=0; i<(int)pgr.DeleteFolderList.size(); ++i )
    {
      const UNINSTALLPROGRAM::DELETEFOLDER& dat = pgr.DeleteFolderList[i];

      if( dat.IsAll )
      {
        xml.AddAttribute( s_UNINSTALLER_DELETEFOLDER_ATTR_FILEDELETE, MAIDTEXT("yes") );
      }

      XMLWriterAutoDescend( xml, s_UNINSTALLER_DELETEFOLDER );

      if( !dat.TargetPath.empty() ) { xml.AddTag( s_UNINSTALLER_DELETEFOLDER_TARGET, dat.TargetPath ); }
      if( !dat.Text.empty() ) { xml.AddTag( s_UNINSTALLER_DELETEFOLDER_TEXT, dat.Text ); }
    }
  }
  {
    for( int i=0; i<(int)pgr.DeleteRegistryList.size(); ++i )
    {
      const UNINSTALLPROGRAM::DELETEREGISTRY& dat = pgr.DeleteRegistryList[i];

      if( dat.IsKeyDelete )
      {
        xml.AddAttribute( s_UNINSTALLER_DELETEREGISTRY_ATTR_DELETE, MAIDTEXT("yes") );
      }

      XMLWriterAutoDescend( xml, s_UNINSTALLER_DELETEREGISTRY );

      if( !dat.Handle.empty() ) { xml.AddTag( s_UNINSTALLER_DELETEREGISTRY_HANDLE, dat.Handle ); }
      if( !dat.SubKey.empty() ) { xml.AddTag( s_UNINSTALLER_DELETEREGISTRY_SUBKEY, dat.SubKey ); }

      for( int j=0; j<(int)dat.ValueList.size(); ++j )
      {
        XMLWriterAutoDescend( xml, s_UNINSTALLER_DELETEREGISTRY_VALUE );
        const String& name = dat.ValueList[i];

        if( !name.empty() ) { xml.AddTag( s_UNINSTALLER_DELETEREGISTRY_VALUE, name ); }
      }
    }
  }

  return FUNCTIONRESULT_OK;
}
