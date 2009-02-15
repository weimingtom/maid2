#ifndef maid2_framework_oggdecoder_h
#define maid2_framework_oggdecoder_h

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
  class OggDecoder
  {
  public:
    OggDecoder( const IOggTimer& Timer, unt32 ProcesserMask );
    virtual ~OggDecoder();

    void Initialize( const Xiph::SPOGGSTREAM& pStream, const Xiph::SPDECODER& pDecoder );
    void Finalize();
    void PageIn( const Xiph::OggPage& page );

    enum STATE
    {
      STATE_EMPTY,        //  初期化すらされていない
      STATE_DECOING,      //  デコード中
      STATE_PAGEWAIT,     //  新しいページがくるのを待っている（パケット切れ）
      STATE_CACHEFULL,    //  キャッシュが満タンなので待っている
      STATE_ENDOFSTREAM,  //  デコード終了
    };

    STATE GetState() const;
    int PopCache( double EndTime, Xiph::SAMPLELIST& Out );

    void Seek( double time );
    bool IsSeeking() const;
    int  GetPacketCount() const;

    double GetCacheTime() const;
    size_t GetCacheCount() const;

    virtual bool IsCacheFull() const=0;

  private:
    void Sleep();

  private:
    ThreadMutex m_StreamMutex;
    const IOggTimer&  m_Timer;
    const unt32 m_ProcesserMask;


    Xiph::SPOGGSTREAM  m_pStream;
    Xiph::SPDECODER    m_pDecoder;
    Xiph::SampleCache  m_Cache;

    STATE m_State;

    unt m_SleepCounter; //  一定量たまるとsleepする

    std::vector<unsigned char>  m_PacketTmp;

    double m_SeekTime;  //  シーク目標時間（負数でシークしないor終わっている)

    int m_PacketCount;  //  処理できるパケットが後何個あるか？


    mutable ThreadMutex m_SampleMutex;
    //  同期待ちが頻繁に起こらないようにするキャッシュ
    double m_TotalTime;
    size_t m_SampleCount;
    double m_BeginTime;

    double m_PrevCheckTime; //  スキップ処理用に前回確認した時間
  private:

    void PushBack( const Xiph::SAMPLE& sample );

    void OnStateDecoding();
    void OnStatePageWait();
    void OnStateCacheFull();
    void OnStateEndOfStream();
    void OnStateSeek(volatile ThreadController::BRIGEDATA& state);

    unt ThreadFunction( volatile ThreadController::BRIGEDATA& state );
    ThreadController  m_Thread;
  };

  typedef boost::shared_ptr<OggDecoder> SPOGGDECODER;


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

}

#endif
