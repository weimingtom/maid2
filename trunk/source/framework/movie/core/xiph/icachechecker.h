#ifndef maid2_framework_movie_core_xiph_icachechecker_h
#define maid2_framework_movie_core_xiph_icachechecker_h



#include"../../../config/define.h"

#include"oggdecoder.h"
#include"../decodersamplecache.h"

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

}}}

#endif
