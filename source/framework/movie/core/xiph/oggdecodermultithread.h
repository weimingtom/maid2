#ifndef maid2_framework_movie_core_xiph_oggdecodermultithread_h
#define maid2_framework_movie_core_xiph_oggdecodermultithread_h

#include"../../../../config/define.h"
#include"../../../../auxiliary/thread.h"

#include"oggdecoder.h"
#include"../decodersamplecache.h"

#include<boost/smart_ptr.hpp>

namespace Maid { namespace Movie { namespace Xiph {


  class ICacheChecker
  {
  public:
    virtual ~ICacheChecker(){}
    virtual bool IsSourceFull( const OggDecoder& decoder ) const = 0;
    virtual bool IsSampleFull( const DecoderSampleCache& cache ) const = 0;
  };

  typedef boost::shared_ptr<ICacheChecker> SPCACHECHECKER;


  class CacheCheckerTheora
    : public ICacheChecker
  {
  public:
    CacheCheckerTheora( int src, size_t sample )
      :m_Source(src)
      ,m_Sample(sample)
      {}

    virtual bool IsSourceFull( const OggDecoder& decoder ) const
    {
      return m_Source <= decoder.GetPacketCount();
    }

    virtual bool IsSampleFull( const DecoderSampleCache& cache ) const
    {
      return m_Sample <= cache.GetSize();
    }

  private:
    const int  m_Source;
    const size_t  m_Sample;
  };

  class CacheCheckerVorbis 
    : public ICacheChecker
  {
  public:
    CacheCheckerVorbis( int src, double time )
      :m_Source(src)
      ,m_Time(time)
      {}

    virtual bool IsSourceFull( const OggDecoder& decoder ) const
    {
      return m_Source <= decoder.GetPacketCount();
    }

    virtual bool IsSampleFull( const DecoderSampleCache& cache ) const
    {
      return m_Time <= cache.GetTotalTime();
    }

  private:
    const int  m_Source;
    const double  m_Time;
  };


  class OggDecoderMultiThread
    : public IDecoder
  {
  public:
    OggDecoderMultiThread( const SPOGGSTREAM& pStream, const SPCODEC& pCodec, const SPCACHECHECKER& pCheck );

    virtual void Initialize();
    virtual void Finalize();

    virtual void AddSource( const SPSTORAGESAMPLE& buffer );

    virtual void FlushSample( double time, DECODERSAMPLELIST& pOut );
    virtual bool IsSourceFull() const;
    virtual bool IsSampleFull() const;
    virtual bool IsDecodeEnd() const;

    virtual double GetTime() const;
    virtual void BeginSkipMode( double targettime );

  private:
    OggDecoder  m_Decoder;
    DecoderSampleCache  m_Cache;  //  これを継承して時間なり、枚数なり見る

    SPCACHECHECKER m_pChecker;

    ThreadMutex m_DecodeMutex; //  m_Decoder の更新用
    ThreadMutex m_CacheMutex; //  m_Decoder の更新用

    bool  m_IsSourceFull;
    bool  m_IsSampleFull;
    bool  m_IsDecodeEnd;

  private:
    unt ThreadFunction( volatile ThreadController::BRIGEDATA& state );
    ThreadController  m_Thread;
  };

  typedef boost::shared_ptr<OggDecoderMultiThread> SPOGGDECODERMULTITHREAD;

}}}

#endif
