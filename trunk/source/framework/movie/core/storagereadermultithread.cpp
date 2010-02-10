#include"storagereadermultithread.h"



namespace Maid { namespace Movie { 

StorageReaderMultiThread::StorageReaderMultiThread( const SPSTORAGEREADER& pReader, size_t count )
  :m_pReader(pReader)
  ,m_CACHECOUNT(count)
  ,m_IsCacheEmpty(true)
  ,m_IsCacheFull(false)
{

}

void StorageReaderMultiThread::Initialize()
{
  m_pReader->Initialize();

  m_Thread.SetFunc( MakeThreadObject(&StorageReaderMultiThread::ThreadFunction,this) );
  m_Thread.Execute();
}

void StorageReaderMultiThread::Finalize()
{
  m_Thread.Close();
  m_pReader->Finalize();
}

void StorageReaderMultiThread::Read( SPSTORAGESAMPLE& pSample )
{
  while( true )
  {
    if( IsEnd() ) { return; }
    if( !IsCacheEmpty() ) { break; }
  }

  {
    ThreadMutexLocker lock(m_CacheMutex);
    pSample = m_Cache.front();
    m_Cache.pop_front();
    UpdateStatus();
  }
}

void StorageReaderMultiThread::UpdateStatus()
{
  ThreadMutexLocker lock(m_CacheMutex);
  m_IsCacheEmpty = m_Cache.empty();
  m_IsCacheFull  = m_CACHECOUNT<=m_Cache.size();
}


bool StorageReaderMultiThread::IsCacheEmpty() const
{
  return m_IsCacheEmpty;
}

bool StorageReaderMultiThread::IsCacheFull() const
{
  return m_IsCacheFull;
}


bool StorageReaderMultiThread::IsEnd() const
{
  if( !m_pReader->IsEnd() ) { return false; }

  return IsCacheEmpty();
}

unt StorageReaderMultiThread::ThreadFunction( volatile ThreadController::BRIGEDATA& state )
{
  while( true )
  {
    if( state.IsExit ) { break; }
    if( m_pReader->IsEnd() ) { break; }

    if( IsCacheFull() ) { ThreadController::Sleep(1); continue; }

    std::list<SPSTORAGESAMPLE>  dat;
    const int loop = std::max(1,(int)m_CACHECOUNT/3);
    for( int i=0; i<loop; ++i )
    {
      if( m_pReader->IsEnd() ) { break; }
      SPSTORAGESAMPLE p;
      m_pReader->Read( p );
      if( p.get()!=NULL ) { dat.push_back(p); }
    }
    {
      ThreadMutexLocker lock(m_CacheMutex);
      m_Cache.splice( m_Cache.end(), dat );
      UpdateStatus();
    }
  }

  return 0;
}



}}


