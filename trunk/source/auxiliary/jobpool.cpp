#include"jobpool.h"

namespace Maid
{

  /*!
      @class  JobPool jobpool.h
      @brief  処理したいことを別スレッドでやっておいてくれるクラス
  \n          やっておいてくれるけど、投げたほうは毎回終わるまで問い合わせないといけないので注意
  \n          JobCacheTemplate を使うといろいろベンリです。
  */


JobPool::JobPool()
{

}

JobPool::~JobPool()
{
  Finalize();
}

//! 処理を行うスレッドを１つで初期化する
/*!
 */
void JobPool::Initialize() { Initialize(1); }

//! 処理を行うスレッドを任意個で初期化する
/*!
 */
void JobPool::Initialize( int ThreadCount )
{
  Set();
  m_Thread.resize( ThreadCount );

  for( int i=0; i<ThreadCount; ++i )
  {
    ThreadController& thread = m_Thread[i];
    thread.SetFunc( MakeThreadObject(&JobPool::ThreadFunction,this) );
    thread.Execute();
    thread.SetProcesserMask( 1 << i );
  }
}

//! 終わる
/*!
 */
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


//!   ジョブを加える
/*!
		 	@param  pInput  [i ]	引数
		 	@param  pFunc		[i ]	処理内容
		 	@param  pOutput	[i ]	処理結果
		 
		  @return 実行状態を調べるためのクラス
 */
SPJOBSTATUS JobPool::AddJob( const SPJOBINPOUT& pInput, const SPJOBFUNCTION& pFunc, const SPJOBOUTPUT& pOutput )
{
  SPJOBSTATUS pStatus( new JobStatus(pInput,pFunc,pOutput) ); 

  {
    //  投げてきたスレッドと、処理するスレッドが同じ＝＝先に処理しないと止まる
    if( m_Thread.size()==1 )
    {
      if( ThreadController::GetCurrentID()==m_Thread[0].GetID() )
      {
         ExecuteJob( pStatus );
         return pStatus;
      }
    }
  }


  #pragma COMPILERMSG("TODO:ひとつのキューを取り合うよりも、あいてるスレッドにキューを投げるほうがいいかも。デッドロックする？")	
  {
    ThreadMutexLocker Lock(m_Mutex);
    m_Job.push_back(pStatus);
  }
  
  return pStatus;
}









/// ここから先の関数は m_Thread のスレッドで行われる



















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
      if( sleeptime>10 ) { sleeptime = 10; }

      ThreadController::Sleep( sleeptime );
    }
  }

  return 0;
}
}

