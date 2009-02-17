#ifndef maid2_framework_streamdecoder_h
#define maid2_framework_streamdecoder_h

#include"../../config/define.h"
#include"../../auxiliary/thread.h"

#include"xiph/oggstream.h"
#include"xiph/icodec.h"
#include"mediatimer.h"
#include"isample.h"

#include<boost/smart_ptr.hpp>

namespace Maid
{
  class StreamDecoder
  {
  public:
    StreamDecoder();
    ~StreamDecoder();

    void Initialize( const Xiph::SPOGGSTREAM& pStream, const Xiph::SPCODEC& pCodec );
    void Finalize();
    void PageIn( const Xiph::OggPage& page );

    void BeginSeek( double TargetTime );
    bool IsSeeking() const;

    void Update( STREAMSAMPLE& out );

    int  GetPacketCount() const;
    bool IsStreamEnd() const;
    bool IsPacketEmpty() const;


  private:
    Xiph::SPOGGSTREAM  m_pStream;
    Xiph::SPCODEC      m_pCodec;

    double    m_TargetTime; //  シーク時に目標とする時間
  };

  typedef boost::shared_ptr<StreamDecoder> SPSTREAMDECODER;

/*
  class OggDecoderVideo 
    : public OggDecoder
  {
  public:
    OggDecoderVideo( const IOggTimer& Timer, unt32 ProcesserMask, size_t count )
      :OggDecoder(Timer,ProcesserMask)
      ,m_Count(count){}

    virtual bool IsCacheFull() const
    {
      return m_Count < GetCacheCount();
    }

  private:
    const size_t  m_Count;
  };

  class OggDecoderSound 
    : public OggDecoder
  {
  public:
    OggDecoderSound( const IOggTimer& Timer, unt32 ProcesserMask, double time )
      :OggDecoder(Timer,ProcesserMask)
      ,m_Time(time){}

    virtual bool IsCacheFull() const
    {
      return m_Time < GetCacheTime();
    }

  private:
    const double  m_Time;
  };
*/
}

#endif
