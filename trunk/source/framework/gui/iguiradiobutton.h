#ifndef maid2_framework_gui_iguiradiobutton_h
#define maid2_framework_gui_iguiradiobutton_h


#include"../../config/define.h"
#include"iguibutton.h"

namespace Maid
{
  class GUIRadioGroup;
  class IGUIRadioButton : public IGUIButton
  {
  public:
    IGUIRadioButton();

    void SetCheck();
    bool IsCheck()const;

    void SetGroup( GUIRadioGroup& Group );

  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );
    virtual void OnCheck();

  private:
    bool  m_IsCheck;
    GUIRadioGroup* m_pGroup;
  };
}
#endif