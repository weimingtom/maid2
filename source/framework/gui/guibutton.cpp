#include"guibutton.h"



namespace Maid
{

GUIButton::GUIButton()
  :m_IsButtonDown(false)
{
}


IGUIParts::MESSAGERETURN GUIButton::OnSendMessage( const IGUIParam& Param )
{
  switch( Param.Message )
  {
  case IGUIParam::MESSAGE_MOUSEDOWN:
    {
      const GUIMESSAGE_MOUSEDOWN& p = static_cast<const GUIMESSAGE_MOUSEDOWN&>(Param);
      m_IsButtonDown = true;
    }break;

  case IGUIParam::MESSAGE_MOUSEUP:
    {
      const GUIMESSAGE_MOUSEUP& p = static_cast<const GUIMESSAGE_MOUSEUP&>(Param);

      if( m_IsButtonDown )
      {
        m_IsButtonDown = false;

        const POINT2DI pos = p.Position-CalcTotalOffset();
        if( LocalIsCollision(pos) )
        {
          if( OnMouseClick( pos )==MESSAGERETURN_SENDPARENT )
          {
            GUIMESSAGE_BUTTON_CLICK m;
            m.Position = p.Position;
            m.Button   = p.Button;
            SendMessageParent( m );
          }
        }
      }
    }break;
  }

  return MESSAGERETURN_SENDPARENT;
}

}
