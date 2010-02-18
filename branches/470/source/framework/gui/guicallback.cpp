#include"guicallback.h"



namespace Maid
{

	/*!
	 	  @class	GUICallback guicallback.h
	 	  @brief	子から飛んできたメッセージをそのままＧＵＩの外に流すクラス
	 */ 


//! コールバックする関数の設定
/*!
    @param  Func  [i ] 設定する関数。
 */
void GUICallback::SetReviceFunction( const RECIVEFUNCTION& Func )
{
  m_Func = Func;
}

void GUICallback::ReciveMessage( ID src, const IGUIParam& Param )
{
  //  そのまま流すだけ
  if( !m_Func ) { return ; }

  m_Func( src, Param );
}


}
