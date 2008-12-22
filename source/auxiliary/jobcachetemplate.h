#ifndef maid2_auxiliary_jobcachetemplate_h
#define maid2_auxiliary_jobcachetemplate_h



#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"

#include"jobpool.h"
#include<map>

namespace Maid
{
  //  Jobで実行したデータを共有するための補助クラス
  /*
    JobCacheTemplate<入力パラメータ,処理アルゴリズム,出力データ(キャッシュされます)>
  */

  template<class INPUT, class FUNC, class OUTPUT>
  class JobCacheTemplate : public GlobalPointer<JobPool>
  {
  public:
    struct INFO
    {
      int         ReferenceCount;
      SPJOBSTATUS pStatus;
      SPJOBOUTPUT pOut;
    };
    typedef std::map<INPUT,INFO> INFOMAP;

    JobCacheTemplate():m_State(STATE_EMPTY){}
    JobCacheTemplate( const JobCacheTemplate<INPUT,FUNC,OUTPUT>& rha )
      :m_State(STATE_EMPTY)
    {
      if( rha.IsEmpty() ) { return ; }
      Start( rha.m_Input );
    }

    virtual ~JobCacheTemplate()
    {
      Reset();
    }
 
    JobCacheTemplate<INPUT,FUNC,OUTPUT>& operator = ( const JobCacheTemplate<INPUT,FUNC,OUTPUT>& rha )
    {
      Reset();
      if( rha.IsEmpty() ) { return ; }
      Start( rha.m_Input );
    }

  public:
    void Start( const INPUT& input )
    {
      Reset();
      INFOMAP::iterator ite = s_InfoMap.find(input);

      if( ite==s_InfoMap.end() )
      { //  もし初めての読み込みだったら、ファイトです！
        boost::shared_ptr<INPUT > pInput ( new INPUT(input)  );
        boost::shared_ptr<FUNC  > pFunc  ( new FUNC   );
        boost::shared_ptr<OUTPUT> pOutput( new OUTPUT );

        SPJOBSTATUS pObj = GlobalPointer<JobPool>::Get()->AddJob( pInput, pFunc, pOutput );

        INFO& info = s_InfoMap[input];

        info.ReferenceCount = 1;
        info.pStatus   = pObj;
      }
      else
      {
        //  ２回目以降はキャッシュのみ
        ite->second.ReferenceCount += 1;
      }

      m_Input = input;
      m_State = STATE_LOADING;

      //  とりあえず調べておく
      StateCheck();
    }

    void Reset()
    {
      if( m_State==STATE_EMPTY ) { return ; }

      INFOMAP::iterator ite = s_InfoMap.find(m_Input);

      m_Input = INPUT();
      m_Output= OUTPUT();
      m_State = STATE_EMPTY;
      ite->second.ReferenceCount -= 1;

      if( ite->second.ReferenceCount==0 )
      {
        s_InfoMap.erase( ite );
      }

    }

    bool IsExecuting() const
    {
      if( m_State==STATE_EMPTY ) { return false; }  //  読み込みしてないならfalseでしょう
      const_cast<JobCacheTemplate<INPUT,FUNC,OUTPUT>*>(this)->StateCheck();
      return m_State!=STATE_WORKING;
    }

    bool IsEmpty() const { return m_State==STATE_EMPTY; }

    const INPUT& GetInput() const
    {
      return m_Input; 
    }

    const OUTPUT& GetOutput() const
    {
      const_cast<JobCacheTemplate<INPUT,FUNC,OUTPUT>*>(this)->StateCheck();
      MAID_ASSERT( IsExecuting(), "まだ処理が終わってません" );
      return m_Output; 
    }

  private:
    void StateCheck()
    {
      if( m_State!=STATE_LOADING ) { return; }

      INFOMAP::iterator ite = s_InfoMap.find(m_Input);
      INFO& info = ite->second;

      //  info.pObject が存在している == まだ稼動準備中ということになる
      if( info.pStatus.get()!=NULL )
      {
        //  実行中ならまだまだ
        if( info.pStatus->IsExecuting() ) { return ; }

        //  ここにきたら処理が完了している
        info.pOut = info.pStatus->GetOutput();
        info.pStatus.reset();
      }

      m_Output = static_cast<OUTPUT&>(*info.pOut);
      m_State = STATE_WORKING;
    }


    static INFOMAP s_InfoMap;

    enum STATE
    {
      STATE_EMPTY,	//	まだ何もしてない
      STATE_LOADING,	//	読み込み中
      STATE_WORKING,	//	稼動中
    };

    STATE   m_State;
    INPUT   m_Input;
    OUTPUT  m_Output;
  };


}


#endif