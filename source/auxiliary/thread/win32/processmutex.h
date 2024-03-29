﻿#ifndef maid2_auxiliary_thread_win32_processmutex_h
#define maid2_auxiliary_thread_win32_processmutex_h


/*!
    @file
    @brief プロセス間の同期オブジェクト
 */

#include"../../../config/define.h"
#include"../../../config/Win32.h"
#include"../../string.h"


namespace Maid
{
	class ProcessMutex
	{
	public:
		ProcessMutex();
		~ProcessMutex();


		void Open( const String& MutexName, bool IsOriginal );

		void Close();

		static bool IsExist(  const String& MutexName );
	private:

		HANDLE	m_hMutex;
	};

}


#endif
