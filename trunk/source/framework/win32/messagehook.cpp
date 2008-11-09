#include"messagehook.h"
#include"messagehookmanager.h"


namespace Maid
{

/*!
  @class	MessageHook messagehook.h
  @brief	Win32 用メッセージプロシージャ
          メッセージ処理はすべてこのクラスを継承して実装します
 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
MessageHook::MessageHook()
{
	m_IsHook = false;
	m_hWnd   = NULL;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
MessageHook::~MessageHook()
{
	DelProc();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! プロシージャにコールバックしてもらうようにする
/*!
    @param	hWnd	[i ]	登録する場所
 */
void MessageHook::AddProc( HWND hWnd )
{
  GlobalPointer<MessageHookManager>::Get()->AddHook( this, hWnd );
  m_hWnd   = hWnd;
  m_IsHook = true;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! プロシージャにコールバックをやめてもらう
/*!
 */
void MessageHook::DelProc()
{
  if( m_IsHook )
  {
    GlobalPointer<MessageHookManager>::Get()->DelHook( this, m_hWnd );
    m_hWnd   = NULL;
    m_IsHook = false;
  }
}

void MessageHook::EnterInterruptMode()
{
  GlobalPointer<MessageHookManager>::Get()->SetInterruptHook( this );
}

void MessageHook::LeaveInterruptMode()
{
  GlobalPointer<MessageHookManager>::Get()->SetInterruptHook( NULL );
}


}