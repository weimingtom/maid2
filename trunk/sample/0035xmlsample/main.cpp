/*
  ファイル読み込み、書き込みのサンプル
*/


#include"../../source/auxiliary/debug/assert.h"
#include"../../source/auxiliary/debug/warning.h"
#include"../../source/auxiliary/debug/trace.h"
#include"../../source/auxiliary/string.h"
#include"../../source/auxiliary/exception.h"
#include"../../source/auxiliary/jobpool.h"


#include"../../source/storage/storage.h"
#include"../../source/storage/filewriter.h"
#include"../../source/storage/filereader.h"


#include"../../source/auxiliary/xml/xmlwriter.h"
#include"../../source/auxiliary/xml/xmlreader.h"
#include"../../source/storage/xmlfile.h"

using namespace Maid;



void WriteXML( XMLWriter& write )
{
  write.AscendNode(MAIDTEXT("top"));
    write.AddTag( MAIDTEXT("child1"), MAIDTEXT("value") );

    write.AscendNode(MAIDTEXT("tag"));
      write.AddTag( MAIDTEXT("child3"), 100 );

    write.DescendNode();

    write.AddTag( MAIDTEXT("child2"), 0.5f );

  write.DescendNode();
}


void PrintXML( XMLReader& read )
{
    int level = 0;

    while( true )
    {
      if( read.IsEndNode() )
      {
        if( level==0 ) { break; }
        read.DescendNode();
        String text = MAIDTEXT("</") + read.GetNodeName() + MAIDTEXT(">\n");
        printf( "%s\n", String::ConvertMAIDtoSJIS(text).c_str() );
        read.NextNode();
        --level;
        continue;
      }

      const String NodeName = read.GetNodeName();
      String Type;

      switch( read.GetNodeType() )
      {
      case XMLReader::NODETYPE_EMPTY: {read.NextNode();}break;
      case XMLReader::NODETYPE_ELEMENT:
        {
          Type += MAIDTEXT("<") + NodeName + MAIDTEXT(">") + read.GetString() + MAIDTEXT("</") + NodeName + MAIDTEXT(">");
          read.NextNode();
        }break;

      case XMLReader::NODETYPE_TAG: 
        {
          Type += MAIDTEXT("<") + NodeName + MAIDTEXT(">\n");
          read.AscendNode();
          ++level;
        }break;

      }
      printf( "%s\n", String::ConvertMAIDtoSJIS(Type).c_str() );
    }
}

void main()
{
  {
    //  これはおまじないです。
    String::Initialize();
  }

  Storage storage;
  storage.Initialize(); //  これもおまじない

  JobPool jobpool;
  jobpool.Initialize();


  std::string xmltext;

	{
    XMLWriter write;

    WriteXML( write );


    write.Save( xmltext );


    printf( "%s", xmltext.c_str() );
    printf( "\n----------------------\n" );
	}

  {
    XMLDocument doc;
    XMLReader read;

    doc.Parse(xmltext);

    read.Reset(doc);
    PrintXML( read );

    printf( "\n----------------------\n" );
  }

  {
    XMLFileWriter hFile;

    WriteXML( hFile );

    hFile.Write( MAIDTEXT("sample.xml") );

    while( true )
    {
      if( hFile.IsWritting() ) { ::Sleep(1); continue; }
      break;
    }
  }

  {
    XMLFileReader hFile;

    hFile.Load( MAIDTEXT("sample.xml") );

    while( true )
    {
      if( hFile.IsLoading() ) { ::Sleep(1); continue; }
      break;
    }

    PrintXML( hFile.GetReader() );
  }
}