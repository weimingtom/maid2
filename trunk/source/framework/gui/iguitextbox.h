#ifndef maid2_framework_gui_iguitextbox_h
#define maid2_framework_gui_iguitextbox_h


#include"../../config/define.h"
#include"../../auxiliary/string.h"
#include"iguiparts.h"

namespace Maid
{
  class IGUITextBox : public IGUIParts
  {
  public:
    IGUITextBox();

    void SetText( const String& text );
    const String& GetText() const;
    void SetTextLengthByte( int Length );

    int GetTextCursor() const;
    const String& GetIMText() const;
    int GetIMCursor() const;

    bool IsIMOpen() const;

  protected:
    virtual MESSAGERETURN MessageExecuting( SPGUIPARAM& pParam );
  private:
    void	ClipText();

  private:
    String  m_InputText;
    int     m_CursorPos;
    int     m_TextLengthByte;

    String  m_IMText;
    int     m_IMCursorPos;
    bool    m_IsIMOpen;
  };
}

#endif