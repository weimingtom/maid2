#include"iguiradiobutton.h"
#include"guiradiogroup.h"

namespace Maid
{
/*!
 	  @class	IGUIRadioButton iguiradiobutton.h
 	  @brief	ラジオボタン
  \n        チェックボタンとの違いはチェックした後（外部から）チェックをはずすことができません
 */ 
IGUIRadioButton::IGUIRadioButton()
  :m_IsCheck(false)
  ,m_IsCheckedMessage(false)
  ,m_pGroup(NULL)
{

}

//! チェックする
/*!
 */
void IGUIRadioButton::SetCheck()
{
  if( m_pGroup!=NULL )
  {
    if( m_pGroup->m_pSelectted!=NULL )
    {
      m_pGroup->m_pSelectted->m_IsCheck = false;
    }
    m_pGroup->m_pSelectted = this;
  }

  m_IsCheck = true;
}

//! 現在チェックされているか調べる
/*!
    @return されているなら true
            されていないなら false
 */
bool IGUIRadioButton::IsCheck()const
{
  return m_IsCheck;
}


//! ラジオボタンのどれと連携させるかを決める
/*!
    @param  Group [i ] これに関連させたのが連携対象となる
 */
void IGUIRadioButton::SetGroup( GUIRadioGroup& Group )
{
  m_pGroup = &Group;

  if( m_pGroup->m_pSelectted==NULL )
  { //  まだどこもチェックされていないなら自分をチェックしておく
    SetCheck();
  }
}

//!  すでにチェックされている状態にもかかわらず、再度チェックされたときにメッセージを送るか？
/*!
    @param  on [i ] 送るなら true
 */
void IGUIRadioButton::SetCheckedMessage( bool on )
{
  m_IsCheckedMessage = on;
}

//! チェックされた時に呼ばれる
/*!
 */
void IGUIRadioButton::OnCheck()
{

}


IGUIRadioButton::MESSAGERETURN IGUIRadioButton::MessageExecuting( SPGUIPARAM& Param )
{
  switch( Param->Message )
  {
  case IGUIParam::MESSAGE_BUTTON_CLICK:
    {
      const GUIMESSAGE_BUTTON_CLICK& m = static_cast<const GUIMESSAGE_BUTTON_CLICK&>(*Param);

      {
        GUIMESSAGE_RADIOBUTTON_SETCHECK post;
        PostMessage( post );
      }

    }break;

  case IGUIParam::MESSAGE_RADIOBUTTON_SETCHECK:
    {
      const GUIMESSAGE_RADIOBUTTON_SETCHECK& m = static_cast<const GUIMESSAGE_RADIOBUTTON_SETCHECK&>(*Param);

      //  すでにチェックされていたら届けない
      if( !m_IsCheckedMessage && m_IsCheck ) { return IGUIRadioButton::MESSAGERETURN_NONE; }

      SetCheck();
      OnCheck();
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
