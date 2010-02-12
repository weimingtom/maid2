//#define LOCAL_PROFILE

#include"oggdecodermultithread.h"
#include"../../../../auxiliary/debug/warning.h"
#include"../../../../auxiliary/debug/profile.h"
#include"../../../../auxiliary/debug/assert.h"

namespace Maid { namespace Movie { namespace Xiph {


OggDecoderMultiThread::OggDecoderMultiThread( const SPOGGSTREAM& pStream, const SPCODEC& pCodec, const SPCACHECHECKER& pCheck, unt32 Mask )
  :m_Decoder(pStream,pCodec)
  ,m_pChecker(pCheck)
  ,m_IsSourceFull(false)
  ,m_IsSampleFull(false)
  ,m_IsDecodeEnd(false)
  ,m_ThreadMask(Mask)
{

}

void OggDecoderMultiThread::Initialize()
{
  m_Decoder.Initialize();

  m_Thread.SetFunc( MakeThreadObject(&OggDecoderMultiThread::ThreadFunction,this) );
  m_Thread.Execute();

  if( m_ThreadMask!=0 ) { m_Thread.SetProcesserMask(m_ThreadMask); }
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
  m_IsSourceFull = m_pChecker->IsSourceFull(m_Decoder);
}


void OggDecoderMultiThread::FlushSample( double time, DECODERSAMPLELIST& Out )
{
  {
    ThreadMutexLocker lock(m_CacheMutex);
    m_Cache.Pop( time, Out );
    m_IsSampleFull = m_pChecker->IsSampleFull(m_Cache);
  }
}

bool OggDecoderMultiThread::IsSourceFull() const
{
  return m_IsSourceFull;
}

bool OggDecoderMultiThread::IsSampleFull() const
{
  return m_IsSampleFull;
}

bool OggDecoderMultiThread::IsDecodeEnd() const
{
  return m_IsDecodeEnd;
}

double OggDecoderMultiThread::GetTime() const
{
  OggDecoderMultiThread* pThis = const_cast<OggDecoderMultiThread*>(this);
  ThreadMutexLocker lock(pThis->m_DecodeMutex);
  return pThis->m_Decoder.GetTime();
}

void OggDecoderMultiThread::BeginSkipMode( double targettime )
{
  {
    ThreadMutexLocker lock(m_DecodeMutex);
    m_Decoder.BeginSkipMode( targettime ); 
  }
  {
    DECODERSAMPLELIST tmp;
    ThreadMutexLocker lock(m_CacheMutex);
    m_Cache.Pop( targettime, tmp );
    m_IsSampleFull = m_pChecker->IsSampleFull(m_Cache);
  }
}


unt OggDecoderMultiThread::ThreadFunction( volatile ThreadController::BRIGEDATA& state )
{
  while( true )
  {
    if( state.IsExit ) { break; }
    if( IsDecodeEnd() ) { break; }
    if( IsSampleFull() ) { ThreadController::Sleep(1); continue; }

    std::list<DECODERSAMPLE>  sample_list;

    {
      ThreadMutexLocker lock(m_DecodeMutex);
      for( int i=0; i<1; ++i )
      {
        DECODERSAMPLE sample;
        m_Decoder.UpdateTime(sample);
        if( sample.pSample.get()!=NULL ) { sample_list.push_back(sample); }
      }
      m_IsDecodeEnd = m_Decoder.IsDecodeEnd();
      m_IsSourceFull = m_pChecker->IsSourceFull(m_Decoder);
    }

    {
      ThreadMutexLocker lock(m_CacheMutex);
      for( std::list<DECODERSAMPLE>::iterator ite=sample_list.begin(); ite!=sample_list.end(); ++ite )
      {
        m_Cache.PushBack( *ite );
      }
      m_IsSampleFull = m_pChecker->IsSampleFull(m_Cache);
    }
  }

  return 0;
}








}}}