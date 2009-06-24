#include"guimessageque.h"

#include"../../auxiliary/macro.h"

namespace Maid
{

	/*!
	 	  @class	GUIMessageQue guimessageque.h
	 	  @brief	ＧＵＩメッセージキュー
	 */ 

GUIMessageQue::~GUIMessageQue()
{
  //  残ってるメッセージを処理したほうがいい予感なのでする
  Execute();
}


//! メッセージを貯める
/*!
    @param  pTarget [i ]  送信先
    @param  Param   [i ]  送るメッセージ

    @return 作成した文字列
 */
void GUIMessageQue::PostMessage( IGUIParts* pTarget, const IGUIParam& Param )
{
  //  メッセージの大きさを固定にすれば new なんてしなくてもいいんだけど
  //  いろいろめんどいのでこうしてある。改良の余地アリ
  MESSAGE m;
  m.pTarget = pTarget;
  m.pParam.reset(Param.Clone());

  m_Que.push_back( m );
}

//! 溜め込んだメッセージを実行する
/*!
 */
void GUIMessageQue::Execute()
{
  while( true )
  {
    //  この関数の中でメッセージが増えることがあるので
    //  なくなるまでひたすらに処理する
    if( m_Que.empty() ) { break; }

    MESSAGEQUE dat;
    dat.swap(m_Que);

    for( size_t i=0; i<dat.size(); ++i )
    {
      MESSAGE& m = dat[i];

      m.pTarget->SendMessage( *(m.pParam) );
    }
  }

}

}