#ifndef maid2_auxiliary_jobcachetemplate_h
#define maid2_auxiliary_jobcachetemplate_h


#include"../config/define.h"
#include"debug/assert.h"
#include"globalpointer.h"
#include"macro.h"
#include"thread.h"

#include"jobpool.h"
#include<map>

namespace Maid
{
  /*!
    @class  JobCacheTemplate jobcachetemplate.h
    @brief  JobPoolで実行したデータを共有するための補助クラス
            INPUT が引数をあらわしていて
            FUNC  が実行する処理
            OUTPUTが実行結果になっていて、キャッシュされます

            このクラスもスレッドセーフになっているので、ガンガン使えます

            使用サンプルは SoundFile がわかりやすいかも
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
      Start( *(rha.m_pInput) );
    }

    virtual ~JobCacheTemplate()
    {
      Reset();
    }
 
    JobCacheTemplate<INPUT,FUNC,OUTPUT>& operator = ( const JobCacheTemplate<INPUT,FUNC,OUTPUT>& rha )
    {
      Reset();
      if( !rha.IsEmpty() )
      {
        Start( *(rha.m_pInput) );
      }
      return *this;
    }

  public:

		//! 処理の開始
		/*!
		 	  @param  input [i ]  実行パラメーター
		 */
    void Start( const INPUT& input )
    {
      Reset();

      m_pInput.reset(new INPUT(input));

      {
        ThreadMutexLocker lock(s_Mutex);
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
      }
      m_State = STATE_LOADING;

      //  とりあえず調べておく
      StateCheck();
    }

		//! 処理の開放
		/*!
        実行中の処理もすてちゃうので注意
		 */
    void Reset()
    {
      if( IsEmpty() ) { return ; }

      {
        ThreadMutexLocker lock(s_Mutex);

        INFOMAP::iterator ite = s_InfoMap.find(GetInput());

        m_State = STATE_EMPTY;
        ite->second.ReferenceCount -= 1;

        if( ite->second.ReferenceCount==0 )
        {
          s_InfoMap.erase( ite );
        }
      }
    }


		//! 処理が実行中か調べる
		/*!
		 	  @return 実行中なら true
    \n          それ以外はfalse
		 */
    bool IsExecuting() const
    {
      if( m_State==STATE_EMPTY ) { return false; }  //  読み込みしてないならfalseでしょう
      const_cast<JobCacheTemplate<INPUT,FUNC,OUTPUT>*>(this)->StateCheck();
      return m_State!=STATE_WORKING;
    }

		//! 初期状態か？
		/*!
		 	  @return なにもしてないなら true
    \n          それ以外はfalse
		 */
    bool IsEmpty() const { return m_State==STATE_EMPTY; }

		//! 実行結果を共有しているオブジェクトが１つしかないか？
		/*!
		 	  @return ひとつなら true
    \n          複数あるならfalse
    \n          ０ならfalse
		 */
    bool IsUnique() const
    {
      if( IsEmpty() ) { false; }

      bool ret = false;
      {
        ThreadMutexLocker lock(s_Mutex);

        INFOMAP::iterator ite = s_InfoMap.find(GetInput());
        ret = ite->second.ReferenceCount==1;
      }

      return ret;
    }

		//! 入力パラメーターの取得
		/*!
		 	  @return 入力パラメーター
		 */
    const INPUT& GetInput() const
    {
      MAID_ASSERT( IsEmpty(), "まだ設定されてません" );
      return static_cast<const INPUT&>(*m_pInput.get()); 
    }

		//! 実行結果の取得
		/*!
		 	  @return 実行結果
		 */
    const OUTPUT& GetOutput() const
    {
      const_cast<JobCacheTemplate<INPUT,FUNC,OUTPUT>*>(this)->StateCheck();
      MAID_ASSERT( IsExecuting(), "まだ処理が終わってません" );

      ThreadMutexLocker lock(s_Mutex);

      INFO& info = s_InfoMap[GetInput()];

      return static_cast<const OUTPUT&>(*info.pStatus->GetOutput().get()); 
    }

  private:

		//! 状態の更新
		/*!
		 */
    void StateCheck()
    {
      if( m_State!=STATE_LOADING ) { return; }

      {
        ThreadMutexLocker lock(s_Mutex);

        INFOMAP::iterator ite = s_InfoMap.find(GetInput());
        INFO& info = ite->second;

        //  実行中ならまだまだ
        if( info.pStatus->IsExecuting() ) { return ; }
      }

      m_State = STATE_WORKING;
    }


    static ThreadMutex s_Mutex;
    static INFOMAP s_InfoMap;

    enum STATE
    {
      STATE_EMPTY,    //	まだ何もしてない
      STATE_LOADING,	//	読み込み中
      STATE_WORKING,	//	稼動中
    };

    boost::shared_ptr<INPUT> m_pInput;
    STATE   m_State;
  };


}


#endif