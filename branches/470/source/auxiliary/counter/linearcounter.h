/*!
 	@file
 	@brief	普通のカウンタテンプレート
 		インライン定義してあるので、普通の値と変わらないはず
 
 		Stepを超えた ++,-- は同じ回数反対にしないと戻ってこない
 */

#ifndef maid2_auxiliary_counter_linearcounter_h
#define maid2_auxiliary_counter_linearcounter_h

#include"../../config/define.h"

#include<algorithm>

namespace Maid
{
  /*!
    @class CCounter Counter.h
    @brief 普通のカウンタテンプレート
   */
  template<class TYPE, class CAST=TYPE>
  class LinearCounter
  {
  public:
    LinearCounter();
    LinearCounter( TYPE Value );

    LinearCounter<TYPE,CAST>& operator++()		{ Inc(); return (*this); }
    LinearCounter<TYPE,CAST>  operator++(int)	{ LinearCounter<TYPE,CAST> _Tmp = *this; Inc(); return (_Tmp); }
    LinearCounter<TYPE,CAST>& operator--()		{ Dec(); return (*this); }
    LinearCounter<TYPE,CAST>  operator--(int)	{ LinearCounter<TYPE,CAST> _Tmp = *this; Dec(); return (_Tmp); }

    operator TYPE() const { return Get(); }

    void  Set( const TYPE& Start, const TYPE& End, int Step );

    void  Inc();
    void  Dec();
    void  ResetStep() { ResetStep(0); }
    void  ResetStep( int NewStep ) { m_NowStep = std::min(m_Step,NewStep); }

    TYPE  Get()	  const;
    const TYPE& GetStart()const	{ return m_Start;	}
    const TYPE& GetEnd()  const	{ return m_End;		}
    int         GetStep() const	{ return m_Step;	}
    int         GetNowStep()const{ return m_NowStep;}

    bool  IsEnd()   const{ return m_Step==0 || m_Step<=m_NowStep; }
    bool  IsBegin() const{ return m_Step==0 || m_NowStep<=0; }

  private:
    TYPE  m_Start;
    TYPE  m_End;
    int   m_Step;
    int   m_NowStep;
  };


  /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
  //! コンストラクタ
  /*!
   */
  template<class TYPE, class CAST>
  LinearCounter<TYPE,CAST>::LinearCounter()
    :m_Step(0),m_NowStep(0)
  {
  };

  /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
  //! コンストラクタ
  /*!
      @param  Value		[i ]	初期値
   */
  template<class TYPE, class CAST>
  LinearCounter<TYPE,CAST>::LinearCounter( TYPE Value )
    :m_Start(Value),m_End(Value),m_Step(0),m_NowStep(0)
  {
  };

  /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
  //! カウンタの初期化
  /*!
      @param  Start		[i ]	初期値
      @param  End			[i ]	終了値
      @param  Step		[i ]	終了値になるまでに何回インクリメントするか
  \n                        Step==0 の時は常に End が返る
   */
  template<class TYPE, class CAST>
  void LinearCounter<TYPE,CAST>::Set( const TYPE& Start, const TYPE& End, int Step )
  {
    m_Start	= Start;
    m_End	  = End;
    m_Step		= Step;
    m_NowStep	= 0;
  };

  /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
  //! カウンタのインクリメント
  /*!
   */
  template<class TYPE, class CAST>
  void LinearCounter<TYPE,CAST>::Inc()
  {
    //	クリップはしない（実際に取得するときにクリップする）
    ++m_NowStep;
  }


  /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
  //! カウンタのデクリメント
  /*!
   */
  template<class TYPE, class CAST>
  void LinearCounter<TYPE,CAST>::Dec()
  {
    //	クリップはしない（実際に取得するときにクリップする）
    --m_NowStep;
  }


  /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
  //! 現在の値を取得する
  /*!
    @return 現在の値
   */
  template<class TYPE, class CAST>
  TYPE LinearCounter<TYPE,CAST>::Get()const
  {
    if( IsEnd()		) { return m_End;	}
    if( IsBegin()	) { return m_Start; }

    if( m_Start>m_End ) { return CAST(m_End) + CAST(m_Start-m_End) * CAST(m_Step-m_NowStep) / CAST(m_Step) ; }
    return CAST(m_Start) + CAST(m_End-m_Start) * CAST(m_NowStep) / CAST(m_Step) ;
  }
}

#endif
