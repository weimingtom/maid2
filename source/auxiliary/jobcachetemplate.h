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
    JobCacheTemplate<共有アルゴリズム,共有データ>
  */

  template<class ID, class FUNC>
  class JobCacheTemplate : public GlobalPointer<JobPool>
  {
  public:
    JobCacheTemplate():m_State(STATE_EMPTY){}
    JobCacheTemplate( const JobCacheTemplate<ID,FUNC>& rha ):m_State(STATE_EMPTY){}
    virtual ~JobCacheTemplate(){ }

  protected:
    //  以下の関数は継承先で実装しておいてね！
    virtual void JobCopy ( const FUNC& f )=0;
    virtual void JobClear( const FUNC& f )=0;
    virtual void JobSetup( FUNC& )=0;

  protected:
    void Start( const ID& id )
    {
      Reset();
      INFOMAP::iterator ite = s_InfoMap.find(id);

      if( ite==s_InfoMap.end() )
      { //  もし初めての読み込みだったら、ファイトです！
        boost::shared_ptr<FUNC> pFunc( new FUNC );

        JobSetup( *(pFunc) );

        SPJOBSTATUS pObj = GlobalPointer<JobPool>::Get()->AddJob( pFunc );

        INFO& info = s_InfoMap[id];

        info.ReferenceCount = 1;
        info.pStatus   = pObj;
        info.pFunction = pFunc;
      }
      else
      {
        //  ２回目以降はキャッシュのみ
        ite->second.ReferenceCount += 1;
      }

      m_ID = id;
      m_State = STATE_LOADING;

      //  とりあえず調べておく
      StateCheck();
    }

    void Reset()
    {
      if( m_State==STATE_EMPTY ) { return ; }

      INFO& info = GetInfo();
      info.ReferenceCount -= 1;

      JobClear( *(info.pFunction) );

      if( info.ReferenceCount==0 )
      {
        s_InfoMap.erase( s_InfoMap.find(m_ID) );
      }

      m_ID = ID();
      m_State = STATE_EMPTY;
    }

    bool IsExecuting() const
    {
      if( m_State==STATE_EMPTY ) { return false; }  //  読み込みしてないならfalseでしょう
      const_cast<JobCacheTemplate<ID,FUNC>*>(this)->StateCheck();
      return m_State!=STATE_WORKING;
    }

    bool IsEmpty() const { return m_State==STATE_EMPTY; }

  private:
    void StateCheck()
    {
      if( m_State!=STATE_LOADING ) { return; }

      INFO& info = GetInfo();

      //  info.pObject が存在している == まだ稼動準備中ということになる
      if( info.pStatus.get()!=NULL )
      {
        //  実行中ならまだまだ
        if( info.pStatus->IsExecuting() ) { return ; }

        //  ここにきたら読み込みが完了している
        info.pStatus.reset();
      }

      JobCopy( *(info.pFunction) );
      m_State = STATE_WORKING;
    }

    struct INFO
    {
      int         ReferenceCount;
      boost::shared_ptr<FUNC>        pFunction;
      SPJOBSTATUS pStatus;
    };

    typedef std::map<ID,INFO> INFOMAP;
    static INFOMAP s_InfoMap;

    enum STATE
    {
      STATE_EMPTY,	//	まだ何もしてない
      STATE_LOADING,	//	読み込み中
      STATE_WORKING,	//	稼動中
    };

    STATE   m_State;
    ID      m_ID;

    INFO& GetInfo()
    {
      MAID_ASSERT( m_State==STATE_EMPTY, "まだ作成していない" );

      INFOMAP::iterator ite = s_InfoMap.find(m_ID);
      MAID_ASSERT( ite==s_InfoMap.end(), "データが見つかりません" );
      MAID_ASSERT( ite->second.ReferenceCount==0, "作成と開放の回数が一致しません" );

      return ite->second;
    }
  };


}


#endif