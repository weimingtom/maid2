#include"storagereadermultithread.h"



namespace Maid { namespace Movie { 

StorageReaderMultiThread::StorageReaderMultiThread( const SPSTORAGEREADER& pReader, size_t count )
  :m_pReader(pReader)
  ,m_CACHECOUNT(count)
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
  //  ThreadController::Sleep(1);
  }

  {
    ThreadMutexLocker lock(m_CacheMutex);
    pSample = m_Cache.front();
    m_Cache.pop_front();
  }
}

bool StorageReaderMultiThread::IsCacheEmpty() const
{
  return m_Cache.empty();
}

bool StorageReaderMultiThread::IsCacheFull() const
{
  return m_CACHECOUNT<=m_Cache.size();
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

    {
      SPSTORAGESAMPLE p;
      m_pReader->Read( p );

      if( p.get()!=NULL )
      {
        ThreadMutexLocker lock(m_CacheMutex);
        m_Cache.push_back(p);
      }
    }
  }

  return 0;
}



}}


