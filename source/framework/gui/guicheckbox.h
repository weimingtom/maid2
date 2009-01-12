#ifndef maid2_framework_gui_guicheckbox_h
#define maid2_framework_gui_guicheckbox_h


#include"../../config/define.h"
#include"iguiparts.h"
#include"guibutton.h"

namespace Maid
{
  class GUICheckBox : public IGUIParts
  {
  public:
    GUICheckBox();

    void SetCheck( bool IsCheck );
    bool IsCheck()const;

  protected:
    virtual void OnSendMessage( const IGUIParam& Param );
    virtual void ReciveMessage( ID src, const IGUIParam& Param );

    virtual void OnMouseMove( const POINT2DI& pos ){}
    virtual void OnMouseIn( const POINT2DI& pos ){}
    virtual void OnMouseOut( const POINT2DI& pos ){}

    virtual void OnMouseDown( const POINT2DI& pos, int Button ){}
    virtual void OnMouseUp( const POINT2DI& pos, int Button ){}
    virtual void OnStateChange( bool IsCheck ){}

    bool IsButtonDown() const { return m_Button.IsButtonDown(); }
    bool IsMouseIn() const { return m_Button.IsMouseIn(); }

  private:

    class GUIButtonEx : public GUIButton
    {
    protected:
      virtual bool LocalIsMouseIn( const POINT2DI& pos ) const;
    };

    GUIButtonEx m_Button;

    bool  m_IsCheck;
  };


}
#endif