﻿/*
  boost regex と maid2を組み合わせるテストコード
  regex の使い方は http://freepg.fc2web.com/cpp/topic_boost_regex_001.html を参照
*/



#include"../../source/auxiliary/string.h"
//#include"../../source/framework/win32/shell.h"
//#include"../../source/storage/packfilecreater.h"
//#include"../../source/storage/xmlfile.h"
/*
#include"../../config/define.h"
#include"../../config/Win32.h"
*/
//#include"../../auxiliary/exception.h"

//#include"../../config/define.h"
//#include"../../auxiliary/streambuffer.h"
#include"../../auxiliary/macro.h"
#include<ostream>

#include<vector>
#include<list>
#include<boost/smart_ptr.hpp>


#include <boost/regex.hpp>

#include<stdio.h>
#include<vector>

using namespace Maid;

void main()
{
  {
    //  これはおまじないです。
    String::Initialize();
  }

  const std::wstring source = L"あいうえおかきくけこ";
  boost::wregex reg(L".*けこ");

  std::vector<boost::wregex> ff;


  if (boost::regex_match(source, reg )) {
	  printf( "発見\n" );
  } else {
	  printf( "未発見\n" );
  }
}

