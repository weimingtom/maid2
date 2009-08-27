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
:m_IsCheck(false),m_pGroup(NULL)
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
      if( m_IsCheck ) { return IGUIRadioButton::MESSAGERETURN_NONE; }

      SetCheck();
      OnCheck();
    }break;
  }

  return IGUIButton::MessageExecuting( Param );
}

}
