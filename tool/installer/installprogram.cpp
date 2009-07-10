#include"installprogram.h"
#include"define.h"

#include"../../../source/auxiliary/xml.h"
#include"../../../source/storage/fileio/filereadnormal.h"
#include"../../../source/framework/win32/shell.h"
#include"../../../source/framework/win32/registry.h"

using namespace Maid;


Maid::FUNCTIONRESULT LoadCopyFile( XMLReader& xml, INSTALLPROGRAM& pgr );
Maid::FUNCTIONRESULT LoadShortcut( XMLReader& xml, INSTALLPROGRAM& pgr );
Maid::FUNCTIONRESULT LoadRegistry( XMLReader& xml, INSTALLPROGRAM& pgr );


Maid::FUNCTIONRESULT LoadInstallProgram( const Maid::String& FileName, INSTALLPROGRAM& pgr )
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

  return LoadInstallProgram( xml, pgr );
}


FUNCTIONRESULT LoadInstallProgram( XMLReader& xml, INSTALLPROGRAM& pgr )
{
  while( true )
  {
    if( xml.IsEndNode() ) { break; }

    const String NodeName = String::ToLower(xml.GetNodeName());

    if( NodeName==s_XML_UNINSTALLLOGFILENAME ) 
    {
      pgr.UninstallLogFileName = xml.GetString();
    }
    else if( NodeName==s_XML_UNINSTALLERPATH ) 
    {
      pgr.UninstallerPath = xml.GetString();
    }
    else if( NodeName==s_XML_MD5CHECK ) 
    {
      pgr.IsMD5Check = String::ToLower(xml.GetString())==MAIDTEXT("yes");
    }
    else if( NodeName==s_XML_COPYFILE ) 
    {
      const FUNCTIONRESULT ret = LoadCopyFile(xml,pgr);
      if( ret==FUNCTIONRESULT_ERROR )
      {
        ::MessageBox( NULL, L"LoadCopyFile()の失敗", L"エラー", MB_OK );	
        return FUNCTIONRESULT_ERROR; 
      }
    }
    else if( NodeName==s_XML_SHORTCUT )
    {
      const FUNCTIONRESULT ret = LoadShortcut(xml,pgr);
      if( ret==FUNCTIONRESULT_ERROR )
      {
        ::MessageBox( NULL, L"LoadShortcut()の失敗", L"エラー", MB_OK );	
        return FUNCTIONRESULT_ERROR; 
      }
    }
    else if( NodeName==s_XML_REGISTRY )
    {
      const FUNCTIONRESULT ret = LoadRegistry(xml,pgr);
      if( ret==FUNCTIONRESULT_ERROR )
      {
        ::MessageBox( NULL, L"LoadRegistry()の失敗", L"エラー", MB_OK );	
        return FUNCTIONRESULT_ERROR; 
      }
    }
    else if( NodeName==s_XML_UNINSTALLOPTION )
    {
      XMLReaderAutoDescend(xml);
      const FUNCTIONRESULT ret = LoadUnInstallProgram(xml,pgr.UninstallOption);
      if( ret==FUNCTIONRESULT_ERROR )
      {
        ::MessageBox( NULL, L"LoadUnInstallProgram()の失敗", L"エラー", MB_OK );	
        return FUNCTIONRESULT_ERROR; 
      }
    }

    xml.NextNode();
  }

  xml.DescendNode();

  return FUNCTIONRESULT_OK;
}


Maid::FUNCTIONRESULT LoadCopyFile( XMLReader& xml, INSTALLPROGRAM& pgr )
{
  xml.AscendNode();
  INSTALLPROGRAM::COPYFILE data;
  while( true )
  {
    if( xml.IsEndNode() ) { break; }

    const String NodeName = String::ToLower(xml.GetNodeName());
         if( NodeName==s_XML_COPYFILE_DISKNO     ) { data.DiskNo  = xml.GetInteger(); }
    else if( NodeName==s_XML_COPYFILE_DESTPATH   ) { data.DstPath = xml.GetString();  }
    else if( NodeName==s_XML_COPYFILE_SOURCEPATH ) { data.SrcPath = xml.GetString();  }
    else if( NodeName==s_XML_COPYFILE_MD5        ) { data.MD5     = xml.GetString();  }

    xml.NextNode();
  }
  xml.DescendNode();

  pgr.CopyFileList.push_back(data);

  return FUNCTIONRESULT_OK;
}




Maid::FUNCTIONRESULT LoadShortcut( XMLReader& xml, INSTALLPROGRAM& pgr )
{
  xml.AscendNode();
  INSTALLPROGRAM::SHORTCUT data;
  while( true )
  {
    if( xml.IsEndNode() ) { break; }

    const String NodeName = String::ToLower(xml.GetNodeName());
         if( NodeName==s_XML_SHORTCUT_TARGETOBJECT) { data.TargetObject  = xml.GetString(); }
    else if( NodeName==s_XML_SHORTCUT_CREATEPATH  ) { data.CreatePath = xml.GetString();  }
    else if( NodeName==s_XML_SHORTCUT_PARAMETERS  ) { data.Parameters = xml.GetString();  }

    xml.NextNode();
  }
  xml.DescendNode();

  pgr.ShortcutList.push_back(data);

  return FUNCTIONRESULT_OK;
}

Maid::FUNCTIONRESULT LoadRegistry( XMLReader& xml, INSTALLPROGRAM& pgr )
{
  xml.AscendNode();
  INSTALLPROGRAM::REGISTRY data;
  while( true )
  {
    if( xml.IsEndNode() ) { break; }

    const String NodeName = String::ToLower(xml.GetNodeName());
    if( NodeName==s_XML_REGISTRY_HANDLE )
    {
      data.Handle = xml.GetString();
    }
    else if( NodeName==s_XML_REGISTRY_SUBKEY ) 
    {
      data.SubKey   = xml.GetString();
      const String del = String::ToLower( xml.GetAttribute( MAIDTEXT("delete") ).GetStr() );
      data.IsKeyDelete = del==MAIDTEXT("yes");
    }
    else if( NodeName==s_XML_REGISTRY_VALUE  )
    {
      INSTALLPROGRAM::REGISTRY::VALUE v;
      xml.AscendNode();
      while( true )
      {
        if( xml.IsEndNode() ) { break; }
        const String NodeName = String::ToLower(xml.GetNodeName());
        if( NodeName==s_XML_REGISTRY_VALUE_NAME ) { v.Name = xml.GetString();  }
        else if( NodeName==s_XML_REGISTRY_VALUE_DATA )
        {
          v.Data = xml.GetString();

          const String NodeName = String::ToLower( xml.GetAttribute( MAIDTEXT("type") ).GetStr() );

               if( NodeName==MAIDTEXT("dword" ) ) { v.Type = INSTALLPROGRAM::REGISTRY::VALUE::TYPE_DWORD;  }
          else if( NodeName==MAIDTEXT("string") ) { v.Type = INSTALLPROGRAM::REGISTRY::VALUE::TYPE_STRING; }
        }
        xml.NextNode();
      }

      data.ValueList.push_back(v);

      xml.DescendNode();
    }

    xml.NextNode();
  }
  xml.DescendNode();

  pgr.RegistryList.push_back(data);

  return FUNCTIONRESULT_OK;
}





FUNCTIONRESULT WriteInstallProgram( XMLWriter& xml, INSTALLPROGRAM& pgr )
{
  xml.AddTag( s_XML_UNINSTALLERPATH, pgr.UninstallerPath );
  xml.AddTag( s_XML_UNINSTALLLOGFILENAME, pgr.UninstallLogFileName );

  if( pgr.IsMD5Check )
  {
    xml.AddTag( s_XML_MD5CHECK, pgr.IsMD5Check );
  }

  for( int i=0; i<(int)pgr.CopyFileList.size(); ++i )
  {
    XMLWriterAutoDescend( xml, s_XML_COPYFILE );

    const INSTALLPROGRAM::COPYFILE& dat = pgr.CopyFileList[i];

    xml.AddTag( s_XML_COPYFILE_DISKNO, dat.DiskNo );
    xml.AddTag( s_XML_COPYFILE_DESTPATH, dat.DstPath );
    xml.AddTag( s_XML_COPYFILE_SOURCEPATH, dat.SrcPath );
    xml.AddTag( s_XML_COPYFILE_MD5, dat.MD5 );
  }

  for( int i=0; i<(int)pgr.ShortcutList.size(); ++i )
  {
    XMLWriterAutoDescend( xml, s_XML_SHORTCUT );

    const INSTALLPROGRAM::SHORTCUT& dat = pgr.ShortcutList[i];

    xml.AddTag( s_XML_SHORTCUT_TARGETOBJECT, dat.TargetObject );
    xml.AddTag( s_XML_SHORTCUT_CREATEPATH, dat.CreatePath );
    if( !dat.Parameters.empty() )
    {
      xml.AddTag( s_XML_SHORTCUT_PARAMETERS, dat.Parameters );
    }
  }

  for( int i=0; i<(int)pgr.RegistryList.size(); ++i )
  {
    XMLWriterAutoDescend( xml, s_XML_REGISTRY );

    const INSTALLPROGRAM::REGISTRY& dat = pgr.RegistryList[i];

    xml.AddTag( s_XML_REGISTRY_HANDLE, dat.Handle );

    {
      if( dat.IsKeyDelete )
      {
        xml.AddAttribute( s_XML_REGISTRY_SUBKEY_ATTER_DELETE, MAIDTEXT("yes") );
      }
      xml.AddTag( s_XML_REGISTRY_SUBKEY, dat.SubKey );
    }

    for( int j=0; j<(int)dat.ValueList.size(); ++j )
    {
      XMLWriterAutoDescend( xml, s_XML_REGISTRY_VALUE );
      const INSTALLPROGRAM::REGISTRY::VALUE& val = dat.ValueList[i];

      xml.AddTag( s_XML_REGISTRY_VALUE_NAME, val.Name );

           if( val.Type==INSTALLPROGRAM::REGISTRY::VALUE::TYPE_STRING ) { xml.AddAttribute( s_XML_REGISTRY_VALUE_DATA_ATTER_TYPE, MAIDTEXT("String") ); }
      else if( val.Type==INSTALLPROGRAM::REGISTRY::VALUE::TYPE_DWORD  ) { xml.AddAttribute( s_XML_REGISTRY_VALUE_DATA_ATTER_TYPE, MAIDTEXT("DWORD") ); }
      xml.AddTag( s_XML_REGISTRY_VALUE_DATA, val.Data );
    }
  }

  {
    XMLWriterAutoDescend( xml, s_XML_UNINSTALLOPTION );
    const FUNCTIONRESULT ret = WriteUnInstallProgram( xml, pgr.UninstallOption );

    if( ret!=FUNCTIONRESULT_OK ) { return FUNCTIONRESULT_ERROR; }
  }

  return FUNCTIONRESULT_OK;
}