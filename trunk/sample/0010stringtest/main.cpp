/*
  Maid2　で使用している文字列は独自文字コードになっています
  なので、それを扱うためのサンプル
*/



#include"../../source/auxiliary/string.h"




using namespace Maid;

void main()
{
  {
    //  これはおまじないです。
    String::Initialize();
  }



	String str_m;

	//	文字列を代入するとき
	str_m = MAIDTEXT( "aあいうo" );

	//	いつもの連結も可能
	str_m += MAIDTEXT( "かきkけこ" );

	{
		//	こうゆう書き方でもＯＫ
		String tmp(MAIDTEXT( "さしすせそ" ));
		str_m += tmp;
	}


	{
		//	出力時に変換するならこうする
		std::string str_s;
		str_s = String::ConvertMAIDtoSJIS( str_m );
	}
}

