#ifndef sample_0050gameframework_startupdialog_h
#define sample_0050gameframework_startupdialog_h

#include"../../source/framework/igamethread.h"

enum RETURNCODE
{
	GAMESTART,
	APPEXIT,
};

RETURNCODE	BeginStartupInfo( const Maid::IGameThread::DEVICELIST& DeviceList, Maid::IGameThread::DEFAULTCONFIG& conf );



#endif
