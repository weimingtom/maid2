#ifndef maid2_framework_gui_guiradiobutton_h
#define maid2_framework_gui_guiradiobutton_h


#include"../../config/define.h"
#include"iguiparts.h"
#include"guibutton.h"

namespace Maid
{
  class GUIRadioButton : public GUIButton
  {
  public:
    GUIRadioButton();

    void SetCheck( bool IsCheck );
    bool IsCheck()const;

  protected:
    virtual MESSAGERETURN OnMouseClick( const POINT2DI& pos );
    virtual MESSAGERETURN OnCheck() { return MESSAGERETURN_SENDPARENT; }

    bool  m_IsCheck;
  };
}
#endif