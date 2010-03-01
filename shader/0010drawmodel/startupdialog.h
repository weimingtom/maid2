#ifndef maid2_shader_0010drawmodel_startupdialog_h
#define maid2_shader_0010drawmodel_startupdialog_h

#include"../../source/framework/igamethread.h"

enum RETURNCODE
{
	GAMESTART,
	APPEXIT,
};

RETURNCODE	BeginStartupInfo( const Maid::IGameThread::DEVICELIST& DeviceList, Maid::IGameThread::DEFAULTCONFIG& conf );



#endif
