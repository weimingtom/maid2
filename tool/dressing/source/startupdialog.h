#ifndef tool_dressing_startupdialog_h
#define tool_dressing_startupdialog_h

#include"../../source/framework/igamethread.h"

enum RETURNCODE
{
	GAMESTART,
	APPEXIT,
};

RETURNCODE	BeginStartupInfo( const Maid::IGameThread::DEVICELIST& DeviceList, Maid::IGameThread::DEFAULTCONFIG& conf );



#endif
