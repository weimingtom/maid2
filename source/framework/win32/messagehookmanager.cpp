#include"messagehookmanager.h"
#include"../../auxiliary/debug/assert.h"


namespace Maid
{

MessageHookManager* GlobalPointer<MessageHookManager>::s_pPointer;


MessageHookManager::MessageHookManager()
{
	m_pInterruptHook = NULL;
}

MessageHookManager::~MessageHookManager()
{
	Finalize();
}

void MessageHookManager::Initialize()
{
  Set();
}

void MessageHookManager::Finalize()
{
  Clear();
}

void MessageHookManager::AddHook( MessageHook* pHook, HWND hWnd )
{
  m_ProcMap[hWnd].push_front( pHook );
}

void MessageHookManager::DelHook( MessageHook* pHook, HWND hWnd )
{
  const HWNDMAP::iterator ite = m_ProcMap.find(hWnd);

  MAID_ASSERT(ite==m_ProcMap.end(), "登録されていないクラスが削除されようとしています" );

  HOOKLIST& List = ite->second;

  List.remove( pHook );
}

  //!	WM_CREATE等を拾うとき用の割り込みフック
  /*!	
	    @param pHook  [i ]  WM_CREATEが飛んでくると確定しているポインタ
  */
void MessageHookManager::SetInterruptHook( MessageHook* pHook )
{
  m_pInterruptHook = pHook;
}


LRESULT CALLBACK MessageHookManager::OnMessage( WindowsMessage& mess  )
{
  if( m_pInterruptHook!=NULL )
  {
    if( m_pInterruptHook->OnMessage( mess )==MessageHook::RETURNCODE_RETURN ) { return 0; }
  }else
  {
    HWNDMAP::iterator ProcList = m_ProcMap.find( mess.GetHWND() );
    if( ProcList!=m_ProcMap.end() )
    {
      HOOKLIST& List = ProcList->second;

      for( HOOKLIST::iterator ite=List.begin(); ite!=List.end(); ++ite )
      {	//	投げられたメッセージを処理したら DefWindowProc は呼ばない
        if( (*ite)->OnMessage( mess )==MessageHook::RETURNCODE_RETURN ) { return 0; }
      }
    }  
  }

	return ::DefWindowProc(mess.GetHWND(),mess.GetMSG(),mess.GetWPARAM(),mess.GetLPARAM());
}

}