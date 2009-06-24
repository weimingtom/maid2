/*!
 	@file
 	@brief	FPS を管理するタイマー
*/

#ifndef maid2_auxiliary_fpstimer_h
#define maid2_auxiliary_fpstimer_h


#include"../config/define.h"
#include"../config/typedef.h"
#include"timer.h"

namespace Maid
{
  class FPSTimer
  {
  public:
    FPSTimer();
    ~FPSTimer();

    void Initialize();

    void Reset();
    bool IsTimerOver() const;

    void Sleep();

    void SetFPS( unt fps );

  private:
    Timer	m_Timer;

    unt	m_BeginTime;    //!<	１フレーム目の開始時間
    unt	m_TimeOverCount;//!<	予定時刻を連続で過ぎている回数
    unt	m_NowFrame;     //!<	現在のフレーム数

    unt	m_FPS;          //!<	維持するＦＰＳ
  };


}


#endif
