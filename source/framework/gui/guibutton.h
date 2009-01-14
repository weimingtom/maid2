#ifndef maid2_framework_gui_guibutton_h
#define maid2_framework_gui_guibutton_h


#include"../../config/define.h"
#include"iguiparts.h"

namespace Maid
{
  class GUIButton : public IGUIParts
  {
  public:
    GUIButton();

    bool IsButtonDown() const { return m_IsButtonDown; }

  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );

    virtual void OnMouseClick( const POINT2DI& pos ){}

  private:
    bool  m_IsButtonDown;
  };

}
#endif