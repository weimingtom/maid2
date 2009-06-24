//#define LOCAL_PROFILE

#include"streamdecodermultithread.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/profile.h"
#include"../../auxiliary/debug/assert.h"

namespace Maid
{

unt IStreamDecoderMultiThread::ThreadFunction( volatile ThreadController::BRIGEDATA& state )
{
  while( true )
  {
    if( state.IsExit ) { break; }

    if( m_IsSeeking )
    {
      ThreadMutexLocker lock(m_SampleMutex);
      m_Cache.Clear();
      m_SampleCount = 0;
      m_TotalTime = 0;
      m_BeginTime = 0;
    }

    if(   IsStreamEnd() 
      ||  IsPacketEmpty()
      ||  IsCacheFull()
      )
    {
      //  連続して寝るにつれて、時間が長くなる
      const unt count = std::min((unt)10,m_SleepCounter/10);
      ThreadController::Sleep(count);
      ++m_SleepCounter;
      continue;
    }

    m_SleepCounter = 0;

    STREAMSAMPLE sample;
    {
      ThreadMutexLocker lock1(m_DecoderMutex);
      m_Decoder.Update( sample );

      m_IsSeeking   = m_Decoder.IsSeeking();
      m_IsStreamEnd = m_Decoder.IsStreamEnd();
      m_PacketCount = m_Decoder.GetPacketCount();
    }


    if( sample.pSample.get()!=NULL )
    {
      ThreadMutexLocker lock2(m_SampleMutex);
      m_Cache.PushBack( sample );

      m_TotalTime   += sample.EndTime - sample.BeginTime;
      m_SampleCount += 1;
//      MAID_WARNING( "serial:" << this << " endtime:" << sample.BeginTime );
    }

    if( !m_IsSeeking )
    { //  もし遅れてたらスキップモード
      const double NowTime = m_pTimer->Get();

      if( sample.EndTime < NowTime )
      {
        BeginSeek( NowTime );
      }
    }
  }

  return 0;
}



}