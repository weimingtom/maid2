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
      Start( *(rha.m_pInput) );
    }

  public:
    void Start( const INPUT& input )
    {
      Reset();

      m_pInput.reset(new INPUT(input));
      INFOMAP::iterator ite = s_InfoMap.find(input);

      if( ite==s_InfoMap.end() )
      { //  もし初めての読み込みだったら、ファイトです！
        boost::shared_ptr<FUNC  > pFunc  ( new FUNC   );
        boost::shared_ptr<OUTPUT> pOutput( new OUTPUT );

        SPJOBSTATUS pObj = GlobalPointer<JobPool>::Get()->AddJob( m_pInput, pFunc, pOutput );
        const INPUT& in = static_cast<const INPUT&>(*m_pInput.get());

        INFO& info = s_InfoMap[in];

        info.ReferenceCount = 1;
        info.pStatus   = pObj;
      }
      else
      {
        //  ２回目以降はキャッシュのみ
        ite->second.ReferenceCount += 1;
      }
      m_State = STATE_LOADING;

      //  とりあえず調べておく
      StateCheck();
    }

    void Reset()
    {
      if( IsEmpty() ) { return ; }

      INFOMAP::iterator ite = s_InfoMap.find(GetInput());

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
    bool IsUnique() const
    {
      if( IsEmpty() ) { false; }
      INFOMAP::iterator ite = s_InfoMap.find(GetInput());

      return ite->second.ReferenceCount==1;
    }

    const INPUT& GetInput() const
    {
      MAID_ASSERT( IsEmpty(), "まだ設定されてません" );
      return static_cast<const INPUT&>(*m_pInput.get()); 
    }

    const OUTPUT& GetOutput() const
    {
      const_cast<JobCacheTemplate<INPUT,FUNC,OUTPUT>*>(this)->StateCheck();
      MAID_ASSERT( IsExecuting(), "まだ処理が終わってません" );
      INFO& info = s_InfoMap[GetInput()];

      return static_cast<const OUTPUT&>(*info.pStatus->GetOutput().get()); 
    }

  private:
    void StateCheck()
    {
      if( m_State!=STATE_LOADING ) { return; }

      INFOMAP::iterator ite = s_InfoMap.find(GetInput());
      INFO& info = ite->second;

      //  info.pObject が存在している == まだ稼動準備中ということになる
      if( info.pStatus.get()!=NULL )
      {
        //  実行中ならまだまだ
        if( info.pStatus->IsExecuting() ) { return ; }
      }

      m_State = STATE_WORKING;
    }


    static INFOMAP s_InfoMap;

    enum STATE
    {
      STATE_EMPTY,	//	まだ何もしてない
      STATE_LOADING,	//	読み込み中
      STATE_WORKING,	//	稼動中
    };

    boost::shared_ptr<INPUT > m_pInput;
    STATE   m_State;
  };


}


#endif