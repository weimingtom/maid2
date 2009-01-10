#ifndef maid2_framework_gui_guibutton_h
#define maid2_framework_gui_guibutton_h


#include"iguiparts.h"
#include<boost/smart_ptr.hpp>

namespace Maid
{
  class GUIButton : public IGUIParts
  {
  public:
    class Listner : public IGUIListner
    {
      friend GUIButton;
    public:
      Listner():m_ButtonDown(false){}
      virtual void ButtonClick( const POINT2DI& pos ) {}

    protected:
      bool IsButtonDown() const { return m_ButtonDown; }

    private:
      bool  m_ButtonDown;
    };

    GUIButton();

    void SetListner( const boost::shared_ptr<Listner>& pListner );

  protected:
    virtual void LocalSendMessage( const IGUIParam& Param );

  private:
    boost::shared_ptr<Listner>  m_pListner;
  };

  typedef boost::shared_ptr<GUIButton::Listner> SPGUIBUTTONLISTNER;

}
#endif