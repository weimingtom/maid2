#include"iguiwindow.h"

namespace Maid
{

  static const IGUIParts::ID ID_HIDEBUTTON    = IGUIParts::PARTSID_LOCAL + 1;
  static const IGUIParts::ID ID_MINIMUMBUTTON = IGUIParts::PARTSID_LOCAL + 2;


IGUIWindow::IGUIWindow()
  :m_IsMinimum(false)
  ,m_State(STATE_NORMAL)
  ,m_PrevMovePosition(0,0)
{

}


void IGUIWindow::SetMinimum( bool IsMin )
{
  GUIMESSAGE_WINDOW_MINIMUM mess;
  mess.IsMinimum = IsMin;
  SendMessage( mess );
}


bool IGUIWindow::IsMinimum() const
{
  return m_IsMinimum;
}


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
      ef( IsHideButtonCollision(pos) )
      {
        ChangeState( pos, STATE_HIDEBUTTONDOWN );
      }
      ef( IsBarCollision(pos) )
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


}

