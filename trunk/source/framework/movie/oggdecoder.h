#ifndef maid2_framework_oggdecoder_h
#define maid2_framework_oggdecoder_h

#include"../config/define.h"
#include"../auxiliary/thread.h"

#include"xiph/oggstream.h"
#include"xiph/samplecache.h"
#include"xiph/icodec.h"
#include"oggtimer.h"

#include<vector>
#include<list>
#include<boost/smart_ptr.hpp>

namespace Maid
{
  struct OGGSAMPLE
  {
    double BeginTime; //  開始時間（単位は秒）
    double EndTime;   //  終了時間（単位は秒）
    Xiph::SPBUFFER pBuffer;

    //  BeginTime <= x < EndTime の範囲になる
  };

  class OggDecoder
  {
  public:
    OggDecoder();
    ~OggDecoder();

    void Initialize( const IOggTimer& Timer, const Xiph::SPOGGSTREAM& pStream, const Xiph::SPCODEC& pCodec );
    void Finalize();
    void PageIn( const Xiph::OggPage& page );

    void BeginSeek( double TargetTime );
    bool IsSeeking() const;

    void Update( OGGSAMPLE& out );

    int  GetPacketCount() const;
    bool IsStreamEnd() const;
    bool IsPacketEmpty() const;


  private:
    const IOggTimer*   m_pTimer;

    Xiph::SPOGGSTREAM  m_pStream;
    Xiph::SPCODEC      m_pCodec;

    double    m_TargetTime; //  シーク時に目標とする時間
  };

  typedef boost::shared_ptr<OggDecoder> SPOGGDECODER;

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
