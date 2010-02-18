/*
  ファイル非同期読み込み、非同期書き込みのサンプル
*/


#include"../../source/auxiliary/debug/assert.h"
#include"../../source/auxiliary/debug/warning.h"
#include"../../source/auxiliary/debug/trace.h"
#include"../../source/auxiliary/string.h"
#include"../../source/auxiliary/exception.h"


#include"../../source/storage/storage.h"
#include"../../source/storage/filewriter.h"
#include"../../source/storage/filereader.h"

using namespace Maid;


void main()
{
  {
    //  これはおまじないです。
    String::Initialize();
  }

  Storage storage;
  storage.Initialize(); //  これもおまじない

  { //  書き込みテスト
    FileWriter hFile;

    hFile.Create( MAIDTEXT("fileio_sample.text") );

    std::string dat( "テキストデータ" );
    hFile.Write( dat.c_str(), dat.length() );

    while( true )
    {
      if( !hFile.IsExecuting() ) { break; }
    }
  }

  { //  読み込みテスト
    FileReader hFile;

    hFile.Open( MAIDTEXT("fileio_sample.text") );
    while( true )
    {
      if( !hFile.IsExecuting() ) { break; }
    }


    char buf[100] = {0};
    hFile.Read( buf, hFile.GetSize() );

    while( true )
    {
      if( !hFile.IsExecuting() ) { break; }
    }

    printf( "size:%d, buf:%s\n", hFile.GetSize(), buf );
  }

}