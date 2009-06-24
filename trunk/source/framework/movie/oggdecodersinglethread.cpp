//#define LOCAL_PROFILE

#include"oggdecodersinglethread.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/debug/profile.h"
#include"../auxiliary/debug/assert.h"

namespace Maid
{

OggDecoderSingleThread::OggDecoderSingleThread( const IOggTimer& Timer )
  :m_Timer(Timer)
  ,m_TotalTime(0)
  ,m_SampleCount(0)
{

}

OggDecoderSingleThread::~OggDecoderSingleThread()
{
}

void OggDecoderSingleThread::Initialize( const Xiph::SPOGGSTREAM& pStream, const Xiph::SPDECODER& pDecoder, const Xiph::SPSAMPLECACHECHECKER& pChecker )
{
  Finalize();

  m_pStream  = pStream;
  m_pDecoder = pDecoder;
  m_pChecker = pChecker;
}

void OggDecoderSingleThread::Finalize()
{
  m_pStream.reset();
  m_pDecoder.reset();
  m_pChecker.reset();
  m_Cache.Clear();

  m_TotalTime = 0;
  m_SampleCount = 0;
}


void OggDecoderSingleThread::PageIn( const Xiph::OggPage& page )
{
  m_pStream->PageIn( page );
}


bool OggDecoderSingleThread::IsStreamEnd() const
{
  return m_pStream->IsEnd();
}

bool OggDecoderSingleThread::IsPacketEmpty() const
{
  return m_pStream->IsPacketEmpty();
}

bool OggDecoderSingleThread::IsCacheFull() const
{
  return m_pChecker->IsFull(m_Cache);
}




int OggDecoderSingleThread::PopCache( double EndTime, Xiph::SAMPLELIST& Out )
{
  MAID_PROFILE();
  const int ret = m_Cache.Pop( EndTime, Out );

  m_TotalTime   = m_Cache.GetTotalTime();
  m_SampleCount = m_Cache.GetSize();

  return ret;
}


void OggDecoderSingleThread::Seek( double time )
{
  MAID_ASSERT( time < 0, "負数は指定できません" << time );

  MAID_ASSERT( true, "未実装" );

}

int  OggDecoderSingleThread::GetPacketCount() const
{
  return m_pStream->GetPacketCount();
}


void OggDecoderSingleThread::PushBack( const Xiph::SAMPLE& sample )
{
  m_Cache.PushBack( sample );

  const double begin = sample.BeginTime;
  const double end   = sample.EndTime;

  m_TotalTime   += end - begin;
  m_SampleCount += 1;
}


void OggDecoderSingleThread::Update()
{
  if( IsCacheFull() ) { return ; }
  if( m_pStream->IsEnd() ) { return ; }
  if( m_pStream->IsPacketEmpty() ) { return ; }

  Xiph::OggPacket packet;

  const bool IsSuccess = m_pStream->PacketOut(packet);

  if( !IsSuccess ) { return ; }

  Xiph::SPBUFFER pBuffer;
  const double begin = m_pDecoder->GetTime();
  {
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
  {
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

#if 0
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

    {
      ThreadMutexLocker lock(m_StreamMutex);

      const bool IsSuccess = m_pStream->PacketOut(packet);

      if( !IsSuccess )
      {
        if( m_pStream->IsEnd() ){ m_State = STATE_ENDOFSTREAM; break; }
        else { m_State = STATE_PAGEWAIT; ThreadController::Sleep(0); continue; }
      }
      packet.Copy( m_PacketTmp, packet );
    }
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


#endif
}