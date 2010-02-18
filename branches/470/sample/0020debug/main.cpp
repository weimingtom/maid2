//#define LOCAL_PROFILE

/*
  例外、警告、トレース、アサートサンプル

  警告は製品でも入れておくようにしておきます
*/


#include"../../source/auxiliary/debug/assert.h"
#include"../../source/auxiliary/debug/warning.h"
#include"../../source/auxiliary/debug/trace.h"
#include"../../source/auxiliary/debug/profile.h"
#include"../../source/auxiliary/string.h"
#include"../../source/auxiliary/exception.h"

#include<windows.h>

using namespace Maid;





class Test
{
public:
  Test()
  {
    MAID_PROFILE();
    ::Sleep(1000);

  }


};

void reigai()
{
  MAID_THROWEXCEPTION("reigai");
}
void main()
{
  {
    //  これはおまじないです。
    String::Initialize();
  }

  {
    {
      //  プロファイルのテスト
      MAID_PROFILE();

      ::Sleep(1000);
      MAID_PROFILE();
      ::Sleep(1000);
      Test t;
    }
    MAID_PROFILE_PRINTLOG( "log.txt" );
  }


  { //  例外のテスト
    try {
      reigai();
    }catch( Exception& e )
    {
      e.WriteLog();
    }
  }

  //  各種デバック表示はこんな感じで使います。
  //  それぞれにおいて、 << で連結することが出来ます
  MAID_WARNING( "警告文" );
  MAID_TRACE( "トレース文" );
  MAID_ASSERT( true, "文字assert" << 100 );

}