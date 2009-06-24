#ifndef maid2_input_textinput_h
#define maid2_input_textinput_h

#include"../config/define.h"
#include"../auxiliary/string.h"

#include"core/icharcodedevice.h"
#include"core/iinputmethoddevice.h"


namespace Maid
{
  class TextInput
  {
  public:
    TextInput();
    virtual ~TextInput();

    void Initialize( const Input::SPCHARCODEDEVICE& pChara, const Input::SPINPUTMETHODDEVICE& pInput );
    void Finalize();

		void Update();

    bool IsEvent() const;

    const String& GetCharaCode() const;
    const String& GetIMCompString() const;
    int           GetIMCompPosition() const;
    bool IsIMOpen() const;

    const Input::SPCHARCODEDEVICE& GetCharaCodeDevice() const;
    const Input::SPINPUTMETHODDEVICE& GetInputMethodDevice() const;

  private:
    String  m_CharaCode;
    String  m_IMComp;
    int     m_IMCompPosition;
    bool    m_IsIMOpen;
    bool    m_IsEvent;

    Input::SPCHARCODEDEVICE     m_pChara;
    Input::SPINPUTMETHODDEVICE  m_pInput;
  };

}

#endif