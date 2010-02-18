#include"fpstimer.h"


namespace Maid
{
	/*!
	 	@class	FPSTimer fpstimer.h
	 	@brief	FPS を管理するタイマー
	 */ 


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
FPSTimer::FPSTimer()
{
  SetFPS( 60 );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
FPSTimer::~FPSTimer()
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 初期化
/*!
 */
void FPSTimer::Initialize()
{
	Reset();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! タイマーをリセットする
/*!
 */
void FPSTimer::Reset()
{
	m_BeginTime = m_Timer.Get();
	m_TimeOverCount= 0;
	m_NowFrame  = 0;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 時間までスレッドを止める
/*!
 	この関数は IsTimerOver() を更新するので毎フレーム呼び出すこと
 */
void FPSTimer::Sleep()
{
	const unt NOW =  m_Timer.Get();
	m_NowFrame++;

	const unt TotalTime = NOW - m_BeginTime;
	const unt TotalFrame= TotalTime*m_FPS/1000;

	if( TotalFrame<m_NowFrame )
	{	//	予定フレームよりも早くSleep() がきたらスレッドを止める
		const unt SleepTime = m_NowFrame*1000/m_FPS-TotalTime;

		m_Timer.Sleep(SleepTime);
		
		m_TimeOverCount = 0;
	}else
	{	//	予定時間をオーバーしたらアウト
		m_TimeOverCount++;

		//	余りにもオーバーしてたらリセットしておく
		if( m_TimeOverCount>120 )
		{
			Reset();
		}
	}
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 時間が過ぎているか？
/*!
 	戻り値が true の場合これ以降の処理を飛ばして Sleep() にいくこと
 */
bool FPSTimer::IsTimerOver() const 
{
	return m_TimeOverCount!=0;	
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 維持するＦＰＳを変更する
/*!
 	@param	fps	[i ]	新しいＦＰＳ
 */
void FPSTimer::SetFPS( unt fps )
{
	m_FPS = fps;
	Reset();
}


}
