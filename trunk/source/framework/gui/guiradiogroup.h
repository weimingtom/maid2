#ifndef maid2_framework_gui_guiradiogroup_h
#define maid2_framework_gui_guiradiogroup_h


#include"../../config/define.h"
#include"iguiparts.h"


namespace Maid
{
  class IGUIRadioButton;
  class GUIRadioGroup
  {
    friend IGUIRadioButton;
  public:
    GUIRadioGroup();

    IGUIRadioButton* GetCheckButton();

  private:
    IGUIRadioButton* m_pSelectted; //  選択されているボタン
  };


}
#endif