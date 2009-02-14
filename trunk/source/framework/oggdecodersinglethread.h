#ifndef maid2_framework_oggdecodersinglethread_h
#define maid2_framework_oggdecodersinglethread_h

#include"../config/define.h"
#include"../auxiliary/thread.h"

#include"xiph/oggstream.h"
#include"xiph/samplecache.h"
#include"xiph/idecoder.h"
#include"oggtimer.h"

#include<vector>
#include<list>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class OggDecoderSingleThread
  {
  public:
    OggDecoderSingleThread( const IOggTimer& Timer );
    ~OggDecoderSingleThread();

    void Initialize( const Xiph::SPOGGSTREAM& pStream, const Xiph::SPDECODER& pDecoder, const Xiph::SPSAMPLECACHECHECKER& pChecker );
    void Finalize();
    void PageIn( const Xiph::OggPage& page );
    int  PopCache( double EndTime, Xiph::SAMPLELIST& Out );

    void Seek( double time );

    int  GetPacketCount() const;

    void Update();

    bool IsStreamEnd() const;
    bool IsPacketEmpty() const;
    bool IsCacheFull() const;


  private:
    const IOggTimer&  m_Timer;


    Xiph::SPOGGSTREAM  m_pStream;
    Xiph::SPDECODER    m_pDecoder;
    Xiph::SampleCache  m_Cache;
    Xiph::SPSAMPLECACHECHECKER m_pChecker;

    mutable ThreadMutex m_SampleMutex;
    //  同期待ちが頻繁に起こらないようにするキャッシュ
    double m_TotalTime;
    size_t m_SampleCount;

  private:
    void PushBack( const Xiph::SAMPLE& sample );

    void OnStateDecoding();
    void OnStatePageWait();
    void OnStateCacheFull();
    void OnStateEndOfStream();
    void OnStateSeek(volatile ThreadController::BRIGEDATA& state);
  };
}

#endif
