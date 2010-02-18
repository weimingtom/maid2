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

    void SetCheckedMessage( bool on );

  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );
    virtual void OnCheck();

  private:
    bool  m_IsCheck;
    bool  m_IsCheckedMessage; //  すでにチェックされている状態にもかかわらず、再度チェックされたときにメッセージを送るか？
    GUIRadioGroup* m_pGroup;
  };
}
#endif