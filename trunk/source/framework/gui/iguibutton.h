#ifndef maid2_framework_gui_iguibutton_h
#define maid2_framework_gui_iguibutton_h


#include"../../config/define.h"
#include"iguiparts.h"

namespace Maid
{
  class IGUIButton : public IGUIParts
  {
  public:
    IGUIButton();

    bool IsButtonDown() const;

  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );

    virtual void OnMouseClick( const POINT2DI& pos );

  private:
    bool  m_IsButtonDown;
  };

}
#endif