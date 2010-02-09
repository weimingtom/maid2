#ifndef maid2_framework_movie_core_decodersamplecache_h
#define maid2_framework_movie_core_decodersamplecache_h

#include"../../../config/define.h"
#include"../../../auxiliary/thread.h"

#include"idecoder.h"

#include<boost/smart_ptr.hpp>
#include<list>

namespace Maid { namespace Movie { 


  class DecoderSampleCache
  {
  public:
    DecoderSampleCache();
    ~DecoderSampleCache();
    void PushBack( const DECODERSAMPLE& sample );
    void Clear();
    int Pop( double EndTime, DECODERSAMPLELIST& Out );

    double GetBeginTime() const;
    double GetTotalTime() const;
    size_t GetSize() const;

  private:
    DECODERSAMPLELIST  m_List;
  };


}}
 
#endif