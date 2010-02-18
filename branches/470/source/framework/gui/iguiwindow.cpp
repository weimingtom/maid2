#include"iguiwindow.h"

namespace Maid
{

/*!
 	  @class	IGUIWindow iguiwindow.h
 	  @brief	ウィンドウね。
            Windowsとの違いは表示を消すことはできても
            ウィンドウそのものを破棄することはできません。
            ゲーム的にはそっちのほうがやさしいはず
 */ 


IGUIWindow::IGUIWindow()
  :m_IsMinimum(false)
  ,m_State(STATE_NORMAL)
  ,m_PrevMovePosition(0,0)
{

}

//! 最小化の状態にするか否か？
/*!
    最小化とはウィンドウバーだけの状態を指します。
    macライク

    @param  IsMin [i ]  最小化するなら true
\n                      元の大きさにするなら false
 */ 
void IGUIWindow::SetMinimum( bool IsMin )
{
  GUIMESSAGE_WINDOW_MINIMUM mess;
  mess.IsMinimum = IsMin;
  SendMessage( mess );
}

//! 現在最小化の状態か？
/*!
    @return  最小化してるなら true
\n           元の大きさなら false
 */
bool IGUIWindow::IsMinimum() const
{
  return m_IsMinimum;
}

//! 現在の状態
/*!
    @return  最小化してるなら true
\n           元の大きさなら false
 */
IGUIWindow::STATE IGUIWindow::GetState() const
{
  return m_State;
}


bool IGUIWindow::LocalIsCollision( const POINT2DI& pos ) const
{
  if( IsBarCollision(pos) ) { return true; }
  if( IsMinimumButtonCollision(pos) ) { return true; }
  if( IsHideButtonCollision(pos) ) { return true; }

  if( m_State==STATE_NORMAL )
  {
    if( IsClientCollision(pos) ) { return true; }
  }

  return false;
}

void IGUIWindow::ChangeState( const POINT2DI& pos, STATE state )
{
  m_State = state;
  OnChangeState( pos, m_State );
}


IGUIWindow::MESSAGERETURN IGUIWindow::MessageExecuting( SPGUIPARAM& pParam )
{
  switch( pParam->Message )
  {
  case IGUIParam::MESSAGE_MOUSEDOWN:
    {     
      const GUIMESSAGE_MOUSEDOWN& m = static_cast<const GUIMESSAGE_MOUSEDOWN&>(*pParam);
      const POINT2DI pos  = CalcLocalPosition(m.Position);

      if( IsMinimumButtonCollision(pos) )
      {
        ChangeState( pos, STATE_MINIMUMBUTTONDOWN );
      }
      else if( IsHideButtonCollision(pos) )
      {
        ChangeState( pos, STATE_HIDEBUTTONDOWN );
      }
      else if( IsBarCollision(pos) )
      {
        m_PrevMovePosition = m.Position;
        ChangeState( pos, STATE_MOVING );
      }
    }break;

  case IGUIParam::MESSAGE_MOUSEUP:
    {
      const GUIMESSAGE_MOUSEUP& m = static_cast<const GUIMESSAGE_MOUSEUP&>(*pParam);
      const POINT2DI pos  = CalcLocalPosition(m.Position);

      switch( m_State )
      {
      case STATE_MOVING:
        {
          m_PrevMovePosition = POINT2DI(0,0);
        }break;

      case STATE_MINIMUMBUTTONDOWN:
        {
          if( IsMinimumButtonCollision(pos) )
          {
            SetMinimum( !m_IsMinimum );
          }

        }break;
      case STATE_HIDEBUTTONDOWN:
        {
          if( IsHideButtonCollision(pos) )
          {
            SetEnable(false);
            SetVisible(false);
          }

        }break;
      }

      ChangeState(pos,STATE_NORMAL);
    }break;

  case IGUIParam::MESSAGE_MOUSEMOVE:
    {
      const GUIMESSAGE_MOUSEMOVE& p = static_cast<const GUIMESSAGE_MOUSEMOVE&>(*pParam);
      const VECTOR2DI vec = VECTOR2DI(m_PrevMovePosition,p.Position);

      if( m_State==STATE_MOVING )
      {
        m_PrevMovePosition = p.Position;
        MovePosition( vec );
      }

    }break;

  case IGUIParam::MESSAGE_WINDOW_MINIMUM:
    {
      const GUIMESSAGE_WINDOW_MINIMUM& m = static_cast<const GUIMESSAGE_WINDOW_MINIMUM&>(*pParam);

      if( m_IsMinimum==m.IsMinimum ) { return MESSAGERETURN_NONE; }

      const bool state = !m.IsMinimum;  //  小さくするなら状態はfalse

      for( CHILDLIST::const_iterator ite=GetChildList().begin(); ite!=GetChildList().end(); ++ite )
      {
        IGUIParts* p = (*ite);

        p->SetEnable( state );
        p->SetVisible( state );
      }

      m_IsMinimum = m.IsMinimum;
      OnMinimum( m.IsMinimum );
    }break;
  }

  return IGUIParts::MessageExecuting( pParam );
}


//! バーの判定
/*!
    @param  pos [i ]  ＧＵＩの位置を(0,0)とした座標

    @return  入っているなら true
\n           そうでないなら false
 */
bool IGUIWindow::IsBarCollision( const POINT2DI& pos ) const{ return false; }

//! クライアント領域の判定
/*!
    @param  pos [i ]  ＧＵＩの位置を(0,0)とした座標

    @return  入っているなら true
\n           そうでないなら false
 */
bool IGUIWindow::IsClientCollision( const POINT2DI& pos ) const{ return false; }

//! 最小化ボタンの判定
/*!
    @param  pos [i ]  ＧＵＩの位置を(0,0)とした座標

    @return  入っているなら true
\n           そうでないなら false
 */
bool IGUIWindow::IsMinimumButtonCollision( const POINT2DI& pos ) const{ return false; }


//! 消えるボタンの判定
/*!
    @param  pos [i ]  ＧＵＩの位置を(0,0)とした座標

    @return  入っているなら true
\n           そうでないなら false
 */
bool IGUIWindow::IsHideButtonCollision( const POINT2DI& pos ) const{ return false; }


//! ドラッグしたり最小化ボタンを押したときに呼ばれる
/*!
    @param  pos   [i ]  ＧＵＩの位置を(0,0)とした座標
    @param  state [i ]  新しい状態
 */
void IGUIWindow::OnChangeState( const POINT2DI& pos, STATE state ){}


//! 最小化or復帰されつつあるときに呼ばれる
/*!
    @param  IsMin   [i ]  trueなら最小化しつつある。falseならもとにもどりつつある
 */
void IGUIWindow::OnMinimum( bool IsMin ){}

}

