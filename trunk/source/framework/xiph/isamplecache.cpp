#include"isamplecache.h"
#include"../../auxiliary/debug/assert.h"


namespace Maid { namespace Xiph {

void ISampleCache::Add( double begin, double end, const SPBUFFER& pBuffer )
{
  MAID_ASSERT( pBuffer.get()==NULL, "データがありません" );

  SAMPLE sample;
  sample.BeginTime = begin;
  sample.EndTime = end;
  sample.pBuffer = pBuffer;


  ThreadMutexLocker lock(m_Mutex);

  m_List.push_back( sample );
}


void ISampleCache::Clear()
{
  m_List.clear();
}


}}