//#define LOCAL_PROFILE

#include"oggdecodermultithread.h"
#include"../../../../auxiliary/debug/warning.h"
#include"../../../../auxiliary/debug/profile.h"
#include"../../../../auxiliary/debug/assert.h"

namespace Maid { namespace Movie { namespace Xiph {


OggDecoderMultiThread::OggDecoderMultiThread( const SPOGGSTREAM& pStream, const SPCODEC& pCodec, const SPCACHECHECKER& pCheck )
  :m_Decoder(pStream,pCodec)
  ,m_pChecker(pCheck)
{

}

void OggDecoderMultiThread::Initialize()
{
  m_Decoder.Initialize();

  m_Thread.SetFunc( MakeThreadObject(&OggDecoderMultiThread::ThreadFunction,this) );
  m_Thread.Execute();
}

void OggDecoderMultiThread::Finalize()
{
  m_Thread.Close();
  m_Decoder.Finalize();
  m_Cache.Clear();
}


void OggDecoderMultiThread::AddSource( const SPSTORAGESAMPLE& buffer )
{
  ThreadMutexLocker lock(m_DecodeMutex);
  m_Decoder.AddSource( buffer );
}


void OggDecoderMultiThread::FlushSample( double time, DECODERSAMPLELIST& Out )
{
  {
    ThreadMutexLocker lock(m_CacheMutex);
    m_Cache.Pop( time, Out );
  }
}

bool OggDecoderMultiThread::IsSourceFull() const
{
  OggDecoderMultiThread* pThis = const_cast<OggDecoderMultiThread*>(this);

  bool ret = false;
  {
    ThreadMutexLocker lock(pThis->m_DecodeMutex);
    ret = m_pChecker->IsSourceFull(m_Decoder);
  }

  return ret;
}

bool OggDecoderMultiThread::IsSampleFull() const
{
  OggDecoderMultiThread* pThis = const_cast<OggDecoderMultiThread*>(this);

  bool ret = false;
  {
    ThreadMutexLocker lock(pThis->m_CacheMutex);
    ret = m_pChecker->IsSampleFull(m_Cache);
  }

  return ret;
}

bool OggDecoderMultiThread::IsDecodeEnd() const
{
  OggDecoderMultiThread* pThis = const_cast<OggDecoderMultiThread*>(this);
  ThreadMutexLocker lock(pThis->m_DecodeMutex);
  return pThis->m_Decoder.IsDecodeEnd();
}

double OggDecoderMultiThread::GetTime() const
{
  OggDecoderMultiThread* pThis = const_cast<OggDecoderMultiThread*>(this);
  ThreadMutexLocker lock(pThis->m_DecodeMutex);
  return pThis->m_Decoder.GetTime();
}

void OggDecoderMultiThread::BeginSkipMode( double targettime )
{
  ThreadMutexLocker lock(m_DecodeMutex);
  m_Decoder.BeginSkipMode( targettime ); 
}


unt OggDecoderMultiThread::ThreadFunction( volatile ThreadController::BRIGEDATA& state )
{
  while( true )
  {
    if( state.IsExit ) { break; }
    if( IsDecodeEnd() ) { break; }
    if( IsSampleFull() ) { Sleep(1); continue; }

    DECODERSAMPLE sample;
    {
      ThreadMutexLocker lock(m_DecodeMutex);
      m_Decoder.UpdateTime(sample);
    }

    if( sample.pSample.get()!=NULL )
    {
      ThreadMutexLocker lock(m_CacheMutex);
      m_Cache.PushBack( sample );
    }
  }

  return 0;
}








}}}