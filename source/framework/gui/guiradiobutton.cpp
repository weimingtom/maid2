#include"guiradiobutton.h"

namespace Maid
{

GUIRadioButton::GUIRadioButton()
:m_IsCheck(false)
{

}

void GUIRadioButton::SetCheck( bool IsCheck )
{
  if( m_IsCheck == IsCheck ) { return ; }

  m_IsCheck = IsCheck;

  //  未チェック->チェックのときのみメッセージを送る
  if( !m_IsCheck && IsCheck )
  {
    if( OnCheck()==MESSAGERETURN_SENDPARENT )
    {
      GUIMESSAGE_RADIOBUTTON_SETCHECK m;
      SendMessageParent( m );
    }
  }

}

bool GUIRadioButton::IsCheck()const
{
  return m_IsCheck;
}

GUIRadioButton::MESSAGERETURN GUIRadioButton::OnMouseClick( const POINT2DI& pos )
{
  if( !m_IsCheck )
  {
    m_IsCheck = true;
    if( OnCheck()==MESSAGERETURN_SENDPARENT )
    {
      GUIMESSAGE_RADIOBUTTON_SETCHECK m;
      SendMessageParent( m );
    }
  }

  return MESSAGERETURN_SENDPARENT;
}

}
