#ifndef maid2_config_define_h
#define maid2_config_define_h


//	各種コンパイル設定を記述しておくヘッダ
//	ライブラリの先頭で必ず include しておくこと

#define GLOBAL_ASSERT   //!<	全部のソースでassertチェックを行う。
#define GLOBAL_WARNING  //!<	全部のソースでwarning出力を行う。
//#define GLOBAL_TRACE    //!<	全部のソースでトレースログの出力を使う。
//#define GLOBAL_PROFILE  //!<	全部のソースでプロファイルを行う。

#define CHECK_MEMORY_LEAK	//!<	メモリリークのチェックを行う

#define USE_DIRECT3DX9	//!<	d3dx9 の各種演算 を使用する


#define USE_INTELIPP		//!<	Intel Integrated Performance Primitives  を使用する





//////////	うっとおしい警告を表示しないようにする
#pragma warning(disable:4996)	//	標準Ｃランタイムの関数名が古いと言われる
#pragma warning(disable:4786)	//	map<vector,vector>とかでデバッグシンボルが長すぎる






#endif