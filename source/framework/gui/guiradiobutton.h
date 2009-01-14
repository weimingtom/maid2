#ifndef maid2_framework_gui_guiradiobutton_h
#define maid2_framework_gui_guiradiobutton_h


#include"../../config/define.h"
#include"guibutton.h"

namespace Maid
{
  class GUIRadioGroup;
  class GUIRadioButton : public GUIButton
  {
  public:
    GUIRadioButton();

    void SetCheck();
    bool IsCheck()const;

    void SetGroup( GUIRadioGroup& Group );

  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );
    virtual void OnCheck() {}

  private:
    bool  m_IsCheck;
    GUIRadioGroup* m_pGroup;
  };
}
#endif