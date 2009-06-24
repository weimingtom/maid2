#ifndef maid2_graphics_core_win32_id3d09object_h
#define maid2_graphics_core_win32_id3d09object_h


#include"../../../../config/define.h"
#include"../../../../auxiliary/thread.h"

#include"common.h"
#include<set>

namespace Maid { namespace Graphics {

  class DeviceD3D09;
  class ID3D09Object
  {
  public:
    ID3D09Object();
    virtual ~ID3D09Object();

    static void EscapeAll();
    static void RestoreAll( DeviceD3D09& Device );

  protected:
    virtual void  Escape()=0;
    virtual void  Restore( DeviceD3D09& Device )=0;

  private:
    static ThreadMutex  s_Mutex;  //  このクラスはいろんなスレッドから作成されるので、同期が必要
    static std::set<ID3D09Object*>  s_ObjectList;
  };

}}

#endif