/*!
 	@file
 	@brief カウンタテンプレート（ループ機能付）
 */

#ifndef maid2_auxiliary_counter_loopcounter_h
#define maid2_auxiliary_counter_loopcounter_h


#include"../../config/define.h"

#include<algorithm>

namespace Maid
{
	/*!
	 	@class CLoopCounter LoopCounter.h
	 	@brief カウンタテンプレート（ループ機能付）
	 
	 	Start ≦ Value ＜ End の範囲を Step で回る
	 
	 	終端まですすんだら先頭に戻ってくる
	 
	 	@par 使用例
	 	@code 
	 
	 	CLoopCounter<int> count(0);	//	0で初期化
	 
	 	count.Set( 0, 360, 7 );
	 
	 	for( int i=0; i<7; ++i )
	 	{
	 		int no = count;
	 		printf( "%d,", no );	//	0,51,102,154, 205,257,308, 0 と表示される
	 		++count;
	 	}
	 
	 	@endcode
	 
	 	@par 似たようなクラス
	 		 LinearCounter
	 	*/
  template<class TYPE, class CAST=TYPE>
	class LoopCounter
	{
	public:
		LoopCounter();
		LoopCounter( TYPE Value );

		LoopCounter<TYPE,CAST>& operator++()     { Inc(); return (*this); }
		LoopCounter<TYPE,CAST>  operator++(int)	{ LoopCounter<TYPE,CAST> _Tmp = *this; Inc(); return (_Tmp); }
		LoopCounter<TYPE,CAST>& operator--()     { Dec(); return (*this); }
		LoopCounter<TYPE,CAST>  operator--(int)	{ LoopCounter<TYPE,CAST> _Tmp = *this; Dec(); return (_Tmp); }

		operator TYPE() const{ return Get(); }


		void Set( const TYPE& Start, const TYPE& End, int Step );
		void SetStep( int Step );

		void Inc();
		void Dec();
    void ResetStep() { ResetStep(0); }
    void ResetStep( int NewStep ) { m_NowStep= NewStep % m_MaxStep; m_LoopNum=NewStep / m_MaxStep; }

		TYPE	 Get()const;

		int GetLoopNum() const{ return m_LoopNum; }
		int GetStep()    const{ return m_NowStep; }

	private:
		TYPE	m_Start;
		TYPE	m_End;
		int		m_MaxStep;
		int		m_NowStep;
		int		m_LoopNum;
	};



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
template<class TYPE, class CAST>
LoopCounter<TYPE,CAST>::LoopCounter()
  :m_MaxStep(0),m_NowStep(0),m_LoopNum(0)
{
};


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 	@param  Value	[i ]	初期値（デフォルトで０）
 */
template<class TYPE, class CAST>
LoopCounter<TYPE,CAST>::LoopCounter( TYPE Value )
  :m_Start(Value),m_End(Value),m_MaxStep(0),m_NowStep(0),m_LoopNum(0)
{
};


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! カウンタの設定
/*!	これ以降 inc() を呼ぶたびにカウントが進む
 
 	@param  Start	[i ]	初期値
 	@param  End		[i ]	終了値
 	@param  Step	[i ]	End になるまでに何回インクリメントするか
 */
template<class TYPE, class CAST>
void LoopCounter<TYPE,CAST>::Set( const TYPE& Start, const TYPE& End, int Step )
{
	m_Start	= Start;
	m_End	= End;
	m_MaxStep	= Step;
	m_NowStep	= 0;
	m_LoopNum  = 0;
};


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在のステップ数を変更する
/*!	
 	@param  Step	[i ]	設定ステップ数
 */
template<class TYPE, class CAST>
void LoopCounter<TYPE,CAST>::SetStep( int Step )
{
	m_NowStep = Step;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! カウンタのインクリメント
/*!	カウンタが終端になったら初期値に戻る
 */
template<class TYPE, class CAST>
void LoopCounter<TYPE,CAST>::Inc()
{
	++m_NowStep;
	if( m_MaxStep<=m_NowStep )
	{
		m_NowStep = 0;
		++m_LoopNum;
	}
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! カウンタのデクリメント
/*!	カウンタが初期値になったら終端値に戻る
 */
template<class TYPE, class CAST>
void LoopCounter<TYPE,CAST>::Dec()
{
	--m_NowStep;
	if( m_NowStep<0 )
	{
		m_NowStep = m_MaxStep-1;
		--m_LoopNum;
	}
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在の値を取得する
/*!	何回も値を参照するときはいったん\n
 	テンポラリにコピーしたほうが効率がいい
 
 	@return 現在のあたい
 */
template<class TYPE, class CAST>
TYPE LoopCounter<TYPE,CAST>::Get()const
{
	if( m_MaxStep==0 ) { return m_End; }
	if( m_Start>m_End ) { return CAST(m_End) + CAST(m_Start-m_End) * CAST(m_MaxStep-m_NowStep) / CAST(m_MaxStep) ; }
	return CAST(m_Start) + CAST(m_End-m_Start) * CAST(m_NowStep) / CAST(m_MaxStep);
}
}

#endif
