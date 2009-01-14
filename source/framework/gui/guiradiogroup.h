#ifndef maid2_framework_gui_guiradiogroup_h
#define maid2_framework_gui_guiradiogroup_h


#include"../../config/define.h"
#include"iguiparts.h"


namespace Maid
{
  class GUIRadioButton;
  class GUIRadioGroup
  {
    friend GUIRadioButton;
  public:
    GUIRadioGroup();

    GUIRadioButton* GetCheckButton();

  private:
    GUIRadioButton* m_pSelectted; //  選択されているボタン
  };


}
#endif