//#define LOCAL_PROFILE

#include"oggdecoder.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/debug/profile.h"
#include"../auxiliary/debug/assert.h"

namespace Maid
{

OggDecoder::OggDecoder( const IOggTimer& Timer )
  :m_State(STATE_EMPTY)
  ,m_IsNewPageIn(false)
  ,m_SleepCounter(0)
  ,m_Timer(Timer)
  ,m_SeekTime(-1)
  ,m_PacketCount(0)
  ,m_TotalTime(0)
  ,m_SampleCount(0)
  ,m_PrevCheckTime(0)
{

}

OggDecoder::~OggDecoder()
{
}

void OggDecoder::Initialize( const Xiph::SPOGGSTREAM& pStream, const Xiph::SPDECODER& pDecoder, const Xiph::SPSAMPLECACHECHECKER& pChecker )
{
  Finalize();

  m_pStream  = pStream;
  m_pDecoder = pDecoder;
  m_pChecker = pChecker;

  m_State = STATE_DECOING;

  m_PacketTmp.reserve(30*1024);
  m_Thread.SetFunc( MakeThreadObject(&OggDecoder::ThreadFunction,this) );
  m_Thread.Execute();
}

void OggDecoder::Finalize()
{
  if( m_State==STATE_EMPTY ) { return ; }

  m_Thread.Close();

  m_pStream.reset();
  m_pDecoder.reset();
  m_pChecker.reset();
  m_Cache.Clear();
  m_IsNewPageIn = false;

  m_State = STATE_EMPTY;
  m_TotalTime = 0;
  m_SampleCount = 0;
}


void OggDecoder::PageIn( const Xiph::OggPage& page )
{
  MAID_PROFILE();
  ThreadMutexLocker lock(m_StreamMutex);

  m_pStream->PageIn( page );

  m_PacketCount = m_pStream->GetPacketCount();


  m_IsNewPageIn = true;
}


OggDecoder::STATE OggDecoder::GetState() const
{
  return m_State;
}

int OggDecoder::PopCache( double EndTime, Xiph::SAMPLELIST& Out )
{
  MAID_PROFILE();
  ThreadMutexLocker lock(m_SampleMutex);
  const int ret = m_Cache.Pop( EndTime, Out );

  m_TotalTime   = m_Cache.GetTotalTime();
  m_SampleCount = m_Cache.GetSize();

  return ret;
}


void OggDecoder::Seek( double time )
{
  MAID_ASSERT( time < 0, "負数は指定できません" << time );

  m_SeekTime = time;
}

bool OggDecoder::IsSeeking() const
{
  return 0<=m_SeekTime;
}

int  OggDecoder::GetPacketCount() const
{
  return m_PacketCount;
}


void OggDecoder::PushBack( const Xiph::SAMPLE& sample )
{
  {
    ThreadMutexLocker lock(m_SampleMutex);
    m_Cache.PushBack( sample );
  }

  const double begin = sample.BeginTime;
  const double end   = sample.EndTime;

  m_TotalTime   += end - begin;
  m_SampleCount += 1;
}

bool OggDecoder::IsFull() const
{
  ThreadMutexLocker lock(m_SampleMutex);
  return m_pChecker->IsFull(m_Cache);
}

unt OggDecoder::ThreadFunction( volatile ThreadController::BRIGEDATA& state )
{
  while( true )
  {
    if( state.IsExit ) { break; }

    if( IsSeeking() )
    {
       OnStateSeek( state );
       m_SeekTime = -1;
    }

    switch( m_State )
    {
    case STATE_EMPTY: { }break;
    case STATE_DECOING:   { OnStateDecoding();  }break;
    case STATE_PAGEWAIT:  { OnStatePageWait();  }break;
    case STATE_CACHEFULL: { OnStateCacheFull(); }break;
    case STATE_ENDOFSTREAM: { OnStateEndOfStream(); }break;
    }
  }

  return 0;
}

void OggDecoder::OnStateDecoding()
{
  if( IsFull() )
  {
    m_State = STATE_CACHEFULL;
    return ;
  }

  m_SleepCounter = 0;

  Xiph::OggPacket packet;

  {
    MAID_PROFILE();
    ThreadMutexLocker lock(m_StreamMutex);

    Xiph::OggPacket p;
    const bool IsSuccess = m_pStream->PacketOut(p);
    m_PacketCount = m_pStream->GetPacketCount();

    if( !IsSuccess )
    {
      if( m_pStream->IsEnd() )
      {
        m_State = STATE_ENDOFSTREAM;
        return ;
      }else
      {
        m_IsNewPageIn = false;
        m_State = STATE_PAGEWAIT;
        return ;
      }
    }
    //  パケットのデータをスレッドで共有しないことによって
    //  同期してる時間を減らす
    packet.Copy( m_PacketTmp, p );
  }


  Xiph::SPBUFFER pBuffer;
  const double begin = m_pDecoder->GetTime();
  {
    MAID_PROFILE();
    m_pDecoder->Decode( packet, pBuffer );
  }
  const double end = m_pDecoder->GetTime();

  if( pBuffer.get()!=NULL )
  {
    Xiph::SAMPLE  sample;
    sample.BeginTime = begin;
    sample.EndTime   = end;
    sample.pBuffer   = pBuffer;

    PushBack( sample );
  }

  //  デコード時刻 < 現在時刻 のケースだと、デコードが遅れていることになるので
  //  適当にすっ飛ばす


  if( 1.0 < end - m_PrevCheckTime )
  {
    m_PrevCheckTime = end;

    const double dec = end;
    const double now = m_Timer.Get();
    const double sa = now - dec;

    if( 0.1 < sa )
    {
      const double time = std::min(sa,2.0);
      m_pDecoder->BeginSeekMode( now + time );
    }
  }
}

void OggDecoder::OnStatePageWait()
{
MAID_PROFILE();

  if( !m_IsNewPageIn )
  {
    Sleep();
  }else
  {
    m_State = STATE_DECOING;
  }
}

void OggDecoder::OnStateCacheFull()
{
MAID_PROFILE();

  if( IsFull() )
  {
    Sleep();
  }else
  {
    m_State = STATE_DECOING;
  }
}




void OggDecoder::OnStateEndOfStream()
{
  Sleep();
}


void OggDecoder::OnStateSeek( volatile ThreadController::BRIGEDATA& state )
{
  //  目標時間までシークする
  //  目標地点にジャンプする方法が難しいので、順々に読み取って、たどり着いたら終了ということにする
  const double target = m_SeekTime;
  m_pDecoder->BeginSeekMode( target );
  {
    ThreadMutexLocker lock(m_SampleMutex);
    m_Cache.Clear();
  }


  while( true )
  {
    if( state.IsExit ) { break; }

    Xiph::OggPacket packet;

    bool IsContinue = false;
    {
      ThreadMutexLocker lock(m_StreamMutex);

      const bool IsSuccess = m_pStream->PacketOut(packet);

      if( !IsSuccess )
      {
        if( m_pStream->IsEnd() ){ m_State = STATE_ENDOFSTREAM; break; }
        else { m_State = STATE_PAGEWAIT; IsContinue = true; }
      }else
      {
        packet.Copy( m_PacketTmp, packet );
      }
    }
    if( IsContinue )
    {
      ThreadController::Sleep(1);
      continue;
    }

    m_SleepCounter = 0;
    m_State = STATE_DECOING;

    Xiph::SPBUFFER pBuffer;
    const double begin = m_pDecoder->GetTime();

    {
      m_pDecoder->Decode( packet, pBuffer );
    }

    //  セットアップが終わるまではグタグタやる
    if( !m_pDecoder->IsSetupped() ) { continue; }

    const double end = m_pDecoder->GetTime();

    if( begin<=target && target<end )
    {
      if( pBuffer.get()!=NULL )
      {
        Xiph::SAMPLE  sample;
        sample.BeginTime = begin;
        sample.EndTime   = end;
        sample.pBuffer   = pBuffer;

        PushBack( sample );
      }
      break;
    }
  }
}


void OggDecoder::Sleep()
{
  {
    //  連続して寝るにつれて、時間が長くなる
    unt count = std::min((unt)10,m_SleepCounter/10);
    ThreadController::Sleep(count);
  }

  ++m_SleepCounter;
}


}