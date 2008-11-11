#include"gameframework.h"

#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/trace.h"
#include"../../auxiliary/exception.h"


namespace Maid
{
/*
void CSingleWindowAppWindows::OnNextFrame( RETURNMESSAGEQUE& ret )
{
	while( true )
	{
		CThreadMutexLocker Lock(m_MessageMutex);

		CGraphic::FRAMEINFO* pInfo = &m_FrameInfo;

		if( !m_IsFrameUpdating )
		{
			//	描画が終わっていたら、キューを交換する
			m_GraphicCore.SwapRenderMessage( pInfo->que );
		}else
		{
			//	終わっていなくても、強制実行命令が出ている場合は待つ
			if( pInfo->IsForce ) { ::Sleep(0); continue; }
		}

		ret.swap(m_ReturnMessageQue);
		pInfo->que.resize(0);
		pInfo->IsForce = false;

		m_IsFrameUpdating = true;
		break;
	}
}
*/

void GameFrameWork::ExecuteWindowMessage()
{
	if( m_pGame.get()==NULL ) { return ; }

	OSMESSAGEMESSAGEQUE que;

	{
		ThreadMutexLocker Lock(m_MessageMutex);
		m_OSMessageQue.swap( que );
	}


	for( int i=0; i<(int)que.size(); ++i )
	{
		m_pGame->OnOSMessage( que[i] ); 
	}
}

unt GameFrameWork::ThreadFunction( volatile ThreadController::BRIGEDATA& brige )
{
	::ResetEvent(m_hGameEvent);

	{
		m_pGame->OnSetup();

		if( m_pGame->IsExitGame() )
		{
			::SetEvent(m_hAppEvent);
			return 0; 
		}
	}

	::SetEvent(m_hAppEvent);
	::WaitForSingleObject(m_hGameEvent, INFINITE);

	{

		GameThread::BEGINPARAM	param;
/*		param.pInfo = &m_FrameInfo;
		param.pFontDriver = SPFONTDRIVER( new CFontDriverWindows() );
		param.pKeybordDriver  = m_pKeybordDriver;
		param.pMouseDriver    = m_pMouseDriver;
		param.pCharCodeDriver = m_pCharCodeDriver;
		param.pIMEDriver = m_pIMEDriver;
*/
		m_pGame->OnBegin( param );
	}

	while( true )
	{
		if( brige.IsExit ) { break; }
		if( m_pGame->IsExitGame() ) { break; }

		ExecuteWindowMessage();
		m_pGame->OnLoop();
	}

	m_pGame->OnEnd();

	m_pGame.reset();

//	RETURNMESSAGEQUE ret;

//	OnNextFrame(ret);
	return 0;
}



}
