#include"../../source/auxiliary/debug/assert.h"
#include"../../source/auxiliary/debug/warning.h"
#include"../../source/auxiliary/debug/trace.h"
#include"../../source/auxiliary/string.h"
#include"../../source/auxiliary/exception.h"


#include"../../source/storage/storage.h"

using namespace Maid;


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