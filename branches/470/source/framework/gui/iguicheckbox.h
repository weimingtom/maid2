#ifndef maid2_framework_gui_iguicheckbox_h
#define maid2_framework_gui_iguicheckbox_h


#include"../../config/define.h"
#include"iguibutton.h"

namespace Maid
{
  class IGUICheckBox : public IGUIButton
  {
  public:
    IGUICheckBox();

    void SetCheck( bool IsCheck );
    bool IsCheck()const;

  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );

    virtual void OnStateChange( bool NewState );

  private:
    bool  m_IsCheck;
  };


}
#endif