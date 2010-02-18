#include"iguibutton.h"

#include"../../input/keybord.h"


namespace Maid
{

	/*!
	 	  @class	IGUIButton iguibutton.h
	 	  @brief	ボタンをあらわすクラス
  \n          押せるだけってことですね。
	 */ 

//! ボタンをクリックした時に呼ばれる
/*!
    @param  pos [i ]  クリックした座標
 */
void IGUIButton::OnMouseClick( const POINT2DI& pos ){}



IGUIButton::IGUIButton()
  :m_IsButtonDown(false)
{
}




//! ボタンが押されている最中か？
/*!
    @return  押されているなら true
 */
bool IGUIButton::IsButtonDown() const
{
  return m_IsButtonDown; 
}



IGUIButton::MESSAGERETURN IGUIButton::MessageExecuting( SPGUIPARAM& Param )
{
  switch( Param->Message )
  {
  case IGUIParam::MESSAGE_MOUSEDOWN:
    {
      const GUIMESSAGE_MOUSEDOWN& m = static_cast<const GUIMESSAGE_MOUSEDOWN&>(*Param);
      m_IsButtonDown = true;
    }break;

  case IGUIParam::MESSAGE_MOUSEUP:
    {
      const GUIMESSAGE_MOUSEUP& m = static_cast<const GUIMESSAGE_MOUSEUP&>(*Param);

      if( m_IsButtonDown )
      {
        m_IsButtonDown = false;

        if( IsCollision(m.Position) )
        {
          OnMouseClick( CalcLocalPosition(m.Position) );

          GUIMESSAGE_BUTTON_CLICK mess;

          PostMessage( mess );
        }
      }
    }break;

  case IGUIParam::MESSAGE_KEYBORDDOWN:
    {
      if( !IsEnable() ) { break; }
      const GUIMESSAGE_KEYBORDDOWN& m = static_cast<const GUIMESSAGE_KEYBORDDOWN&>(*Param);

      if( m.Key==Keybord::BUTTON_ENTER )
      {
        m_IsButtonDown = false;
        GUIMESSAGE_BUTTON_CLICK mess;

        PostMessage( mess );
      }

    }break;
  }

  return IGUIParts::MessageExecuting( Param );
}

}
