#ifndef maid2_framework_gui_guimessage_h
#define maid2_framework_gui_guimessage_h

#include"../../config/define.h"
#include"../../auxiliary/mathematics.h"
#include"../../auxiliary/string.h"


#include<boost/smart_ptr.hpp>

namespace Maid
{
  class IGUIParam
  {
  public:
    enum MESSAGE
    {
      MESSAGE_INITIALIZE, //  初期化される
      MESSAGE_FINALIZE, //  開放される

      MESSAGE_SETPOSITION,  //  座標の変更（新規設定）
      MESSAGE_MOVEPOSITION, //  座標の変更（加算設定）
      MESSAGE_ENABLE,       //  稼動状態が変更
      MESSAGE_VISIBLE,      //  表示状態が変更
      MESSAGE_SETKEYBORDFOCUS,     //  フォーカスの設定


      MESSAGE_MOUSEMOVE,//  マウスが移動した
      MESSAGE_MOUSEIN,  //  マウスがパーツの範囲内に入った
      MESSAGE_MOUSEOUT, //  マウスがパーツの範囲内から出た
      MESSAGE_MOUSEDOWN,//  マウスボタンが押された
      MESSAGE_MOUSEUP,  //  マウスボタンが離された

      MESSAGE_KEYBORDDOWN,  //  キーボードキーが押された
      MESSAGE_KEYBORDUP,    //  キーボードキーが離された

      MESSAGE_BUTTON_CLICK,  //  ボタンがクリックされた

      MESSAGE_CHECKBOX_SETCHECK,  //  チェックボックスの状態が変更された

      MESSAGE_RADIOBUTTON_SETCHECK,  //  ラジオボタンがチェックされた

      MESSAGE_SLIDER_CHANGEVALUE,  //  スライダーの値が変わった

      MESSAGE_WINDOW_MINIMUM,  //  最小ボタンが押された
      MESSAGE_TEXTBOX_UPDATESTATE,  //  入力中のテキストが変更された

      MESSAGE_CONTEXTMENU_CLICK,  //  コンテキストメニューが選択された

      MESSAGE_COMBOBOX_CHANGEELEMENT,  //  コンボボックスで要素が変更された
    };

    IGUIParam( MESSAGE m ):Message(m){}
    virtual ~IGUIParam(){}
    const MESSAGE Message;

    enum BUTTONTYPE
    {
      BUTTON_LEFT=0,  //  マウス左ボタン
      BUTTON_RIGHT,   //  マウス右ボタン
      BUTTON_MIDDLE,  //  マウス中央ボタン
    };

    virtual IGUIParam* Clone() const = 0;
  };

  String CreateMessageLog( const IGUIParam& param );

  typedef boost::shared_ptr<IGUIParam> SPGUIPARAM;


  class IGUIParts;
  class GUIMESSAGE_INITIALIZE : public IGUIParam
  {
  public:
    GUIMESSAGE_INITIALIZE():IGUIParam(MESSAGE_INITIALIZE){}
    IGUIParts*  pParent;
    unt         ID;

    IGUIParam* Clone() const { return new GUIMESSAGE_INITIALIZE(*this); }
  };

  class GUIMESSAGE_FINALIZE : public IGUIParam
  {
  public:
    GUIMESSAGE_FINALIZE():IGUIParam(MESSAGE_FINALIZE){}
    IGUIParam* Clone() const { return new GUIMESSAGE_FINALIZE(*this); }
  };

  class GUIMESSAGE_SETPOSITION : public IGUIParam
  {
  public:
    GUIMESSAGE_SETPOSITION():IGUIParam(MESSAGE_SETPOSITION){}
    POINT2DI Position;
    IGUIParam* Clone() const { return new GUIMESSAGE_SETPOSITION(*this); }
  };

  class GUIMESSAGE_MOVEPOSITION : public IGUIParam
  {
  public:
    GUIMESSAGE_MOVEPOSITION():IGUIParam(MESSAGE_MOVEPOSITION){}
    VECTOR2DI Offset;
    IGUIParam* Clone() const { return new GUIMESSAGE_MOVEPOSITION(*this); }
  };

  class GUIMESSAGE_ENABLE : public IGUIParam
  {
  public:
    GUIMESSAGE_ENABLE():IGUIParam(MESSAGE_ENABLE){}
    bool IsEnable;
    IGUIParam* Clone() const { return new GUIMESSAGE_ENABLE(*this); }
  };

  class GUIMESSAGE_VISIBLE : public IGUIParam
  {
  public:
    GUIMESSAGE_VISIBLE():IGUIParam(MESSAGE_VISIBLE){}
    bool IsVisible;
    IGUIParam* Clone() const { return new GUIMESSAGE_VISIBLE(*this); }
  };


  class GUIMESSAGE_SETKEYBORDFOCUS : public IGUIParam
  {
  public:
    GUIMESSAGE_SETKEYBORDFOCUS():IGUIParam(MESSAGE_SETKEYBORDFOCUS){}
    bool IsFocus;
    IGUIParam* Clone() const { return new GUIMESSAGE_SETKEYBORDFOCUS(*this); }
  };

  class GUIMESSAGE_MOUSEMOVE : public IGUIParam
  {
  public:
    GUIMESSAGE_MOUSEMOVE():IGUIParam(MESSAGE_MOUSEMOVE){}
    POINT2DI Position;
    IGUIParam* Clone() const { return new GUIMESSAGE_MOUSEMOVE(*this); }
  };

  class GUIMESSAGE_MOUSEIN : public IGUIParam
  {
  public:
    GUIMESSAGE_MOUSEIN():IGUIParam(MESSAGE_MOUSEIN){}
    POINT2DI Position;
    IGUIParam* Clone() const { return new GUIMESSAGE_MOUSEIN(*this); }
  };

  class GUIMESSAGE_MOUSEOUT : public IGUIParam
  {
  public:
    GUIMESSAGE_MOUSEOUT():IGUIParam(MESSAGE_MOUSEOUT){}
    POINT2DI Position;
    IGUIParam* Clone() const { return new GUIMESSAGE_MOUSEOUT(*this); }
  };

  class GUIMESSAGE_MOUSEDOWN : public IGUIParam
  {
  public:
    GUIMESSAGE_MOUSEDOWN():IGUIParam(MESSAGE_MOUSEDOWN){}
    POINT2DI Position;
    BUTTONTYPE Button;
    IGUIParam* Clone() const { return new GUIMESSAGE_MOUSEDOWN(*this); }
  };

  class GUIMESSAGE_MOUSEUP : public IGUIParam
  {
  public:
    GUIMESSAGE_MOUSEUP():IGUIParam(MESSAGE_MOUSEUP){}
    POINT2DI Position;
    BUTTONTYPE Button;
    IGUIParam* Clone() const { return new GUIMESSAGE_MOUSEUP(*this); }
  };

  class GUIMESSAGE_KEYBORDDOWN : public IGUIParam
  {
  public:
    GUIMESSAGE_KEYBORDDOWN():IGUIParam(MESSAGE_KEYBORDDOWN){}
    unt08 Key;
    IGUIParam* Clone() const { return new GUIMESSAGE_KEYBORDDOWN(*this); }
  };

  class GUIMESSAGE_KEYBORDUP : public IGUIParam
  {
  public:
    GUIMESSAGE_KEYBORDUP():IGUIParam(MESSAGE_KEYBORDUP){}
    unt08 Key;
    IGUIParam* Clone() const { return new GUIMESSAGE_KEYBORDUP(*this); }
  };



  class GUIMESSAGE_BUTTON_CLICK : public IGUIParam
  {
  public:
    GUIMESSAGE_BUTTON_CLICK():IGUIParam(MESSAGE_BUTTON_CLICK){}
    IGUIParam* Clone() const { return new GUIMESSAGE_BUTTON_CLICK(*this); }
  };

  class GUIMESSAGE_CHECKBOX_SETCHECK : public IGUIParam
  {
  public:
    GUIMESSAGE_CHECKBOX_SETCHECK():IGUIParam(MESSAGE_CHECKBOX_SETCHECK){}
    bool NewState;
    IGUIParam* Clone() const { return new GUIMESSAGE_CHECKBOX_SETCHECK(*this); }
  };

  class GUIMESSAGE_RADIOBUTTON_SETCHECK : public IGUIParam
  {
  public:
    GUIMESSAGE_RADIOBUTTON_SETCHECK():IGUIParam(MESSAGE_RADIOBUTTON_SETCHECK){}
    IGUIParam* Clone() const { return new GUIMESSAGE_RADIOBUTTON_SETCHECK(*this); }
  };

  class GUIMESSAGE_SLIDER_CHANGEVALUE : public IGUIParam
  {
  public:
    GUIMESSAGE_SLIDER_CHANGEVALUE():IGUIParam(MESSAGE_SLIDER_CHANGEVALUE){}
    IGUIParam* Clone() const { return new GUIMESSAGE_SLIDER_CHANGEVALUE(*this); }
    int Value;
  };

  class GUIMESSAGE_WINDOW_MINIMUM : public IGUIParam
  {
  public:
    GUIMESSAGE_WINDOW_MINIMUM():IGUIParam(MESSAGE_WINDOW_MINIMUM){}
    IGUIParam* Clone() const { return new GUIMESSAGE_WINDOW_MINIMUM(*this); }
    bool IsMinimum; //  ( true...最小化しようとしてる false...元に戻ろうとしてる
  };

  class GUIMESSAGE_TEXTBOX_UPDATESTATE : public IGUIParam
  {
  public:
    GUIMESSAGE_TEXTBOX_UPDATESTATE():IGUIParam(MESSAGE_TEXTBOX_UPDATESTATE){}
    IGUIParam* Clone() const { return new GUIMESSAGE_TEXTBOX_UPDATESTATE(*this); }

    String  CharaCode;      //  入力された文字
    String  IMCompString;   //  変換中の文字列
    int     IMCompPosition; //  変換中のカーソルがある位置
    bool    IsIMOpen;       //  IM中か？
  };

  class GUIMESSAGE_CONTEXTMENU_CLICK : public IGUIParam
  {
  public:
    GUIMESSAGE_CONTEXTMENU_CLICK():IGUIParam(MESSAGE_CONTEXTMENU_CLICK){}
    IGUIParam* Clone() const { return new GUIMESSAGE_CONTEXTMENU_CLICK(*this); }

    int     SelectID; //  選択されたID IGUIContextMenu::Insert or IGUIContextMenu::IElement::Insert の引数
  };

  class GUIMESSAGE_COMBOBOX_CHANGEELEMENT : public IGUIParam
  {
  public:
    GUIMESSAGE_COMBOBOX_CHANGEELEMENT():IGUIParam(MESSAGE_COMBOBOX_CHANGEELEMENT){}
    IGUIParam* Clone() const { return new GUIMESSAGE_COMBOBOX_CHANGEELEMENT(*this); }

    int     SelectID; //  選択されたID IGUIConboBox::Insert の引数
  };


}

#endif