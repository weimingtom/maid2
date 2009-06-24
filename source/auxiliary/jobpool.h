/*!
  @file
  @brief	スレッドをたくさん管理してるクラス
 */

#ifndef maid2_auxiliary_jobpool_h
#define maid2_auxiliary_jobpool_h

#include"../config/define.h"
#include"thread.h"
#include"globalpointer.h"

#include<vector>
#include<list>

namespace Maid
{
  class IJobInput
  {
  public:
    virtual ~IJobInput(){}
  };
  typedef boost::shared_ptr<IJobInput>	SPJOBINPOUT;

  class IJobOutput
  {
  public:
    virtual ~IJobOutput(){}
  };

  typedef boost::shared_ptr<IJobOutput>	SPJOBOUTPUT;


  class IJobFunction
  {
  public:
    virtual ~IJobFunction(){}
    virtual void Execute( const IJobInput& Input, IJobOutput& result )=0;
  };

  typedef boost::shared_ptr<IJobFunction>	SPJOBFUNCTION;


  class JobPool;
  class JobStatus
  {
    friend JobPool;
  public:
    JobStatus( const SPJOBINPOUT& pInput, const SPJOBFUNCTION& pFunc, const SPJOBOUTPUT& pOutput )
      :m_pInput(pInput),m_pFunction(pFunc),m_pOutput(pOutput),m_IsExecuting(true){}

    bool IsExecuting() const { return m_IsExecuting; }
    const SPJOBINPOUT& GetInput() const { return m_pInput; }
    const SPJOBOUTPUT& GetOutput() const { return m_pOutput; }
  private:
    SPJOBINPOUT   m_pInput;
    SPJOBFUNCTION	m_pFunction;
    SPJOBOUTPUT 	m_pOutput;
    bool          m_IsExecuting;
  };

  typedef boost::shared_ptr<JobStatus>	      SPJOBSTATUS;
    
  class JobPool : public GlobalPointer<JobPool>
  {
  public:
    JobPool();
    virtual ~JobPool();

    void Initialize();
    void Initialize( int ThreadCount );
    void Finalize();

    SPJOBSTATUS AddJob( const SPJOBINPOUT& pInput, const SPJOBFUNCTION& pFunc, const SPJOBOUTPUT& pOutput );

  private:
    void ExecuteJob( const SPJOBSTATUS& pJob );

  private:
    //  あらゆるスレッドからアクセスするメンバ
    typedef std::list<SPJOBSTATUS> JOBLIST;
    ThreadMutex m_Mutex;
    JOBLIST m_Job;

  private:
    unt ThreadFunction(volatile ThreadController::BRIGEDATA& brige);
    std::vector<ThreadController> m_Thread;
  };


}

#endif