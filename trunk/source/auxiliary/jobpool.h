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
  class JobPool;
  class IJobFunction
  {
    friend JobPool;
  public:
    IJobFunction():m_IsSucess(true){}
    virtual ~IJobFunction(){}
    virtual void Execute()=0;

    bool IsSucess()const { return m_IsSucess; }
    const String& GetErrorMessage()const { return m_ErrorMSG; }
  protected:
    void OnFaile() { m_IsSucess = false; }
    void OnFaile(const String& ErrorMSG )
    {
      OnFaile();
      m_ErrorMSG = ErrorMSG; 
    }
  private:
    bool  m_IsSucess;
    String  m_ErrorMSG;
  };
  typedef boost::shared_ptr<IJobFunction>	SPJOBFUNCTION;

  class JobStatus
  {
    friend JobPool;
  public:
    JobStatus( const SPJOBFUNCTION& pFunc ):m_pFunction(pFunc),m_IsExecuting(true){}

    const SPJOBFUNCTION& GetFunction() const { return m_pFunction; }
    bool IsExecuting() const { return m_IsExecuting; }
  private:
    SPJOBFUNCTION	m_pFunction;
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

    SPJOBSTATUS AddJob( const SPJOBFUNCTION& pFunc );

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