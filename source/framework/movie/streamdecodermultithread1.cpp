//#define LOCAL_PROFILE

#include"streamdecodermultithread.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/profile.h"
#include"../../auxiliary/debug/assert.h"

namespace Maid
{

IStreamDecoderMultiThread::IStreamDecoderMultiThread()
  :m_pTimer(NULL)
  ,m_SleepCounter(0)
  ,m_TotalTime(0)
  ,m_SampleCount(0)
  ,m_PacketCount(0)
  ,m_IsSeeking(false)
  ,m_IsStreamEnd(false)
  ,m_BeginTime(0)
{

}

void IStreamDecoderMultiThread::Initialize( const IMediaTimer& Timer, const Xiph::SPOGGSTREAM& pStream, const Xiph::SPCODEC& pCodec )
{
  Finalize();

  m_pTimer  = &Timer;

  m_Decoder.Initialize( pStream, pCodec );

  m_Thread.SetFunc( MakeThreadObject(&IStreamDecoderMultiThread::ThreadFunction,this) );
  m_Thread.Execute();

}

void IStreamDecoderMultiThread::Finalize()
{
  ThreadMutexLocker lock1(m_DecoderMutex);
  ThreadMutexLocker lock2(m_SampleMutex);
  m_Thread.Close();
  m_Decoder.Finalize();
  m_Cache.Clear();
}


void IStreamDecoderMultiThread::PageIn( const Xiph::OggPage& page )
{
  ThreadMutexLocker lock(m_DecoderMutex);
  m_Decoder.PageIn( page );

  m_PacketCount = m_Decoder.GetPacketCount();
}


int IStreamDecoderMultiThread::PopCache( double EndTime, STREAMSAMPLELIST& Out )
{
  if( m_SampleCount==0 ) { return 0; }
  if( EndTime<m_BeginTime ) { return 0; }

  int ret;
  {
    ThreadMutexLocker lock(m_SampleMutex);
    ret = m_Cache.Pop( EndTime, Out );

    m_TotalTime   = m_Cache.GetTotalTime();
    m_SampleCount = m_Cache.GetSize();
    m_BeginTime   = m_Cache.GetBeginTime();
  }

  return ret;
}




void IStreamDecoderMultiThread::BeginSeek( double TargetTime )
{
  ThreadMutexLocker lock1(m_DecoderMutex);
  ThreadMutexLocker lock2(m_SampleMutex);

  m_Decoder.BeginSeek( TargetTime );
  m_Cache.Clear();

  m_IsSeeking = true;
}

bool IStreamDecoderMultiThread::IsSeeking() const
{
  return m_IsSeeking;
}

int  IStreamDecoderMultiThread::GetPacketCount() const
{
  return m_PacketCount;
}

bool  IStreamDecoderMultiThread::IsPacketEmpty() const
{
  return GetPacketCount()==0;
}

bool IStreamDecoderMultiThread::IsStreamEnd() const
{
  return m_IsStreamEnd;
}

size_t IStreamDecoderMultiThread::GetCacheCount() const
{
  return m_SampleCount;
}

double IStreamDecoderMultiThread::GetCacheTime() const
{
  return m_TotalTime;
}



}