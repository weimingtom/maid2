#include"iguicheckbox.h"
#include"../../input/keybord.h"

namespace Maid
{


/*!
 	  @class	IGUICheckBox iguicheckbox.h
 	  @brief	チェックボタンをあらわすクラス
            押すと、ＯＮ，ＯＦＦが切り替わります
 */ 

//! チェック状態が変わったときに呼ばれる
/*!
    @param  NewState [i ]  新しい状態
 */
void IGUICheckBox::OnStateChange( bool NewState ) {}



IGUICheckBox::IGUICheckBox()
:m_IsCheck(false)
{

}



//! 外部からチェック状態を変更する
/*!
    @param  IsCheck [i ]  true  でチェック
 \n                       false で未チェック
 */
void IGUICheckBox::SetCheck( bool IsCheck )
{
  if( m_IsCheck == IsCheck ) { return ; }

  {
    GUIMESSAGE_CHECKBOX_SETCHECK mess;
    mess.NewState = IsCheck;
    SendMessage( mess );
  }
}

//! チェック状態を調べる
/*!
    @return  true  でチェック
 \n          false で未チェック
 */
bool IGUICheckBox::IsCheck()const
{
  return m_IsCheck;
}

IGUICheckBox::MESSAGERETURN IGUICheckBox::MessageExecuting( SPGUIPARAM& Param )
{
  switch( Param->Message )
  {
  case IGUIParam::MESSAGE_BUTTON_CLICK:
    {
      const GUIMESSAGE_BUTTON_CLICK& m = static_cast<const GUIMESSAGE_BUTTON_CLICK&>(*Param);
      {
        GUIMESSAGE_CHECKBOX_SETCHECK mess;
        mess.NewState = !m_IsCheck;
        PostMessage( mess );
      }

    }break;

  case IGUIParam::MESSAGE_CHECKBOX_SETCHECK:
    {
      const GUIMESSAGE_CHECKBOX_SETCHECK& m = static_cast<const GUIMESSAGE_CHECKBOX_SETCHECK&>(*Param);
      m_IsCheck = m.NewState;
      OnStateChange(m.NewState);
    }break;
/*
  case IGUIParam::MESSAGE_KEYBORDDOWN:
    { //  継承もとの IGUIButton が処理してくれてるので不要
      const GUIMESSAGE_KEYBORDDOWN& m = static_cast<const GUIMESSAGE_KEYBORDDOWN&>(*Param);

      if( m.Key==Keybord::BUTTON_ENTER )
      {
        GUIMESSAGE_CHECKBOX_SETCHECK mess;
        mess.NewState = !m_IsCheck;
        PostMessage( mess );
      }

    }break;
*/
  }

  return IGUIButton::MessageExecuting( Param );
}

}
