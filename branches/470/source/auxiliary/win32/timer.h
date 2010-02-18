/*!
	@file
	@brief	タイマクラス timeGetTime() Ver.
*/

#ifndef maid2_auxiliary_win32_timer_h
#define maid2_auxiliary_win32_timer_h

#include"../../config/define.h"
#include"../../config/typedef.h"
#include"../../config/win32.h"

namespace Maid
{

	class Timer
	{
	public:
		Timer();
		Timer( const Timer& rha );
		~Timer();

		unt Get() const;
		void Sleep( unt time );


		Timer& operator = ( const Timer& rha );
	private:
		static int	s_RefarenceCount;
	};


}


#endif
