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



void IStreamDecoderMultiThread::BeginSeek( double TargetTime )
{
  ThreadMutexLocker lock(m_DecoderMutex);

  m_Decoder.BeginSeek( TargetTime );

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