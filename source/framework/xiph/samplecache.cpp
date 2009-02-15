#include"samplecache.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"


namespace Maid { namespace Xiph {

SampleCache::SampleCache()
{

}

SampleCache::~SampleCache()
{
  Clear();
}


void SampleCache::PushBack( const SAMPLE& sample )
{
  MAID_ASSERT( sample.pBuffer.get()==NULL, "データがありません" );

  m_List.push_back( sample );
}


void SampleCache::Clear()
{
  m_List.clear();
}

double SampleCache::GetBeginTime() const
{
  if( GetSize()==0 ) { return -1; }

  return m_List.front().BeginTime;
}


double SampleCache::GetTotalTime() const
{
  if( GetSize()==0 ) { return 0; }

  double total = 0;
  for( SAMPLELIST::const_iterator ite=m_List.begin(); ite!=m_List.end(); ++ite )
  {
    const double begin = ite->BeginTime;
    const double end = ite->EndTime;

    total += end - begin;
  }

  return total;
}

size_t SampleCache::GetSize() const
{
  return m_List.size();
}


//  指定した時間までのデータを全部取得する
int SampleCache::Pop( double TargetTime, SAMPLELIST& Out )
{
  if( GetSize()==0 ) { return 0; }

  SAMPLELIST::iterator ite  = m_List.begin();
  while( true )
  {
    if( ite==m_List.end() ) { break; }

    const double begin = ite->BeginTime;
    const double end   = ite->EndTime;

    if( TargetTime < begin ){ break; }
    ++ite;
  }

  Out.splice( Out.end(), m_List, m_List.begin(), ite );

  return m_List.size();
}


}}