#ifndef maid2_framework_streamsamplecache_h
#define maid2_framework_streamsamplecache_h

#include"../../config/define.h"
#include"../../auxiliary/thread.h"

#include"isample.h"

#include <boost/smart_ptr.hpp>
#include<list>

namespace Maid {


  class StreamSampleCache
  {
  public:
    StreamSampleCache();
    ~StreamSampleCache();
    void PushBack( const STREAMSAMPLE& sample );
    void Clear();

    double GetBeginTime() const;
    double GetTotalTime() const;
    size_t GetSize() const;
    int Pop( double EndTime, STREAMSAMPLELIST& Out );

  private:
    STREAMSAMPLELIST  m_List;
  };
}
 
#endif