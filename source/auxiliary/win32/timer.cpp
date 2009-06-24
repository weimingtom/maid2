/*!
	@file
	@brief	タイマクラス timeGetTime() Ver.
*/

#include"timer.h"

//WIN32_LEAN_AND_MEAN を指定してるため ヘッダがincludeされない
#include<mmsystem.h>

#pragma comment (lib,"winmm.lib" )


namespace Maid
{

int Timer::s_RefarenceCount;

Timer::Timer()
{
	if( s_RefarenceCount==0 )
	{
		::TIMECAPS TimeCaps;
		::timeGetDevCaps(&TimeCaps,sizeof(::TIMECAPS));       
		::timeBeginPeriod(TimeCaps.wPeriodMin);
	}

	++s_RefarenceCount;
}

Timer::Timer( const Timer& rha )
{
	++s_RefarenceCount;
}

Timer::~Timer()
{
	--s_RefarenceCount;

	if( s_RefarenceCount==0 )
	{
		::TIMECAPS TimeCaps;
		::timeGetDevCaps(&TimeCaps,sizeof(::TIMECAPS));       
		::timeEndPeriod(TimeCaps.wPeriodMin);
	}
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 時間の取得
/*!
    @return	ＯＳが起動してからの時間(ミリ秒)
 */
unt Timer::Get() const
{
	return ::timeGetTime();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! スレッドを寝かせる
/*!
    あんまり正確ではないらしいけど、きにならないかな。

    @param	time [i ] 寝かせる時間(ミリ秒)
 */
void Timer::Sleep( unt time )
{
	::Sleep( time );
}


Timer& Timer::operator = ( const Timer& rha )
{
	++s_RefarenceCount;

	return *this;
}

}


