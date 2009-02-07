#include"samplecache.h"
#include"../../auxiliary/debug/assert.h"


namespace Maid { namespace Xiph {

void SampleCache::PushBack( const SAMPLE& sample )
{
  MAID_ASSERT( sample.pBuffer.get()==NULL, "データがありません" );

  ThreadMutexLocker lock(m_Mutex);

  m_List.push_back( sample );
}


void SampleCache::Clear()
{
  ThreadMutexLocker lock(m_Mutex);
  m_List.clear();
}


double SampleCache::GetBeginTime() const
{
  ThreadMutexLocker lock(m_Mutex);
  if( m_List.empty() ) { return 0; }

  return m_List.front().BeginTime;
}


SAMPLE SampleCache::GetLastSample() const
{
  ThreadMutexLocker lock(m_Mutex);
  return m_List.back();
}


double SampleCache::GetTotalTime() const
{
  ThreadMutexLocker lock(m_Mutex);
  double ret = 0;

  for( SAMPLELIST::const_iterator ite=m_List.begin(); ite!=m_List.end(); ++ite )
  {
    const double begin = ite->BeginTime;
    const double end = ite->EndTime;

    ret += end - begin;
  }

  return ret;
}

size_t SampleCache::GetSize() const
{
  ThreadMutexLocker lock(m_Mutex);
  return m_List.size();
}

void SampleCache::PopFront( SAMPLE& Out )
{
  ThreadMutexLocker lock(m_Mutex);
  Out = m_List.front();

  m_List.pop_front();
}

void SampleCache::PopAll( SAMPLELIST& Out )
{
  ThreadMutexLocker lock(m_Mutex);
  m_List.swap( Out );
}

bool SampleCache::IsEmpty() const
{
  ThreadMutexLocker lock(m_Mutex);

  return m_List.empty();

}


}}