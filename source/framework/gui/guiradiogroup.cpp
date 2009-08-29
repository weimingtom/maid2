#include"guiradiogroup.h"
#include"iguiradiobutton.h"


namespace Maid
{
	/*!
	 	  @class	GUIRadioGroup guiradiogroup.h
	 	  @brief	ラジオボタンのグループを監視してるクラス
	 */ 
GUIRadioGroup::GUIRadioGroup()
:m_pSelectted(NULL)
{
}

//! 現在チェックされているラジオボタンを取得する
/*!
    @return チェックされているラジオボタン
  \n        NULLなら選択されていない。
 */
IGUIRadioButton* GUIRadioGroup::GetCheckButton()
{
  //  GetがあってSetがないのは IGUIRadioButton 内で強引に書き換えているから

  return m_pSelectted;
}

const IGUIRadioButton* GUIRadioGroup::GetCheckButton()const
{
  //  GetがあってSetがないのは IGUIRadioButton 内で強引に書き換えているから

  return m_pSelectted;
}


}
