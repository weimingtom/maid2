#include"jobpool.h"

namespace Maid
{
JobPool*  GlobalPointer<JobPool>::s_pPointer;

JobPool::JobPool()
{

}

JobPool::~JobPool()
{
  Finalize();
}

void JobPool::Initialize() { Initialize(1); }

void JobPool::Initialize( int ThreadCount )
{
  Set();
  m_Thread.resize( ThreadCount );

  for( int i=0; i<ThreadCount; ++i )
  {
    ThreadController& thread = m_Thread[i];
    thread.SetFunc( MakeThreadObject(&JobPool::ThreadFunction,this) );
    thread.Execute();
  }
}

void JobPool::Finalize()
{
  {
    //  ThreadController::~ThreadController() でスレッドが終了するまで待ってるはずなので
    //  .clear() しておけば全部正常にとまる
    m_Thread.clear();
  }

  //  まだ処理してないジョブはどうするか疑問が残るけど
  //  全部処理してから終了する
  for( JOBLIST::iterator ite=m_Job.begin(); ite!=m_Job.end(); ++ite )
  {
    ExecuteJob( (*ite) );
  }
  m_Job.clear();
}

SPJOBSTATUS JobPool::AddJob( const SPJOBINPOUT& pInput, const SPJOBFUNCTION& pFunc, const SPJOBOUTPUT& pOutput )
{
  SPJOBSTATUS pStatus( new JobStatus(pInput,pFunc,pOutput) ); 

  {
    ThreadMutexLocker Lock(m_Mutex);
    m_Job.push_back(pStatus);
  }
  
  return pStatus;
}

void JobPool::ExecuteJob( const SPJOBSTATUS& pJob )
{
  MAID_ASSERT( pJob.get()==NULL, "仕事先が見つかりません" );
  pJob->m_pFunction->Execute( *(pJob->m_pInput), *(pJob->m_pOutput) );
  pJob->m_IsExecuting = false;
}


unt JobPool::ThreadFunction(volatile ThreadController::BRIGEDATA& brige)
{
  int sleeptime = 1;
  while( true )
  {
    if( brige.IsExit ) { break; }

    SPJOBSTATUS pJob;
    {
       ThreadMutexLocker Lock(m_Mutex);
       if( !m_Job.empty() )
       {
          pJob = m_Job.front();
          m_Job.pop_front();
       }
    }

    if( pJob.get()!=NULL )
    {
      ExecuteJob( pJob );
      sleeptime = 1;
    }else
    {
      //  仕事が無かった場合はスリープ時間を長くしていく
      sleeptime *= 2;
      if( sleeptime>100 ) { sleeptime = 100; }

      ThreadController::Sleep( sleeptime );
    }
  }

  return 0;
}
}

