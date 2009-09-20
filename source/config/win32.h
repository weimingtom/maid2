#ifndef maid2_config_win32_h
#define maid2_config_win32_h

#include"define.h"

//  とりあえずターゲットは windowsXP(SP無) 以降にする
//  詳しくは↓を参照
//  http://msdn.microsoft.com/en-us/library/aa383745(VS.85).aspx

#ifndef WINVER
  #define WINVER 0x0500
#endif

#ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0500
#endif

#ifndef _WIN32_IE
  #define _WIN32_IE 0x0500
#endif

  #define NOMINMAX  //  windows.h 内部にある min, max を使えないようにする。
                    //  std::min, std::max を使う

  // Windows ヘッダーから使用されていない部分を除外します。
  #define WIN32_LEAN_AND_MEAN


  //  そして最後にくると！
  #include<windows.h>
#undef CreateFont //  マクロ置換されてしまってコンパイルがこける

#endif
