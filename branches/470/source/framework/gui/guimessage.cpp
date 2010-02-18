#include"guimessage.h"

namespace Maid
{
  //! デバッグ用にメッセージの内容を文字列に変換する
  /*!
      @param  param [i ]  変換したいメッセージ

      @return 作成した文字列
   */
  String CreateMessageLog( const IGUIParam& param )
  {
    String ret;

    switch( param.Message )
    {
    case IGUIParam::MESSAGE_INITIALIZE:   { ret = MAIDTEXT("INITIALIZE"); }break;
    case IGUIParam::MESSAGE_FINALIZE:     { ret = MAIDTEXT("FINALIZE"); }break;

    case IGUIParam::MESSAGE_SETPOSITION:   { ret = MAIDTEXT("SETPOSITION"); }break;
    case IGUIParam::MESSAGE_MOVEPOSITION:     { ret = MAIDTEXT("MOVEPOSITION"); }break;
    case IGUIParam::MESSAGE_ENABLE:   { ret = MAIDTEXT("ENABLE"); }break;
    case IGUIParam::MESSAGE_VISIBLE:     { ret = MAIDTEXT("VISIBLE"); }break;
    case IGUIParam::MESSAGE_SETKEYBORDFOCUS:     { ret = MAIDTEXT("SETKEYBORDFOCUS"); }break;


    case IGUIParam::MESSAGE_MOUSEMOVE:    { ret = MAIDTEXT("MOUSEMOVE"); }break;
    case IGUIParam::MESSAGE_MOUSEIN:      { ret = MAIDTEXT("MOUSEIN"); }break;
    case IGUIParam::MESSAGE_MOUSEOUT:     { ret = MAIDTEXT("MOUSEOUT"); }break;
    case IGUIParam::MESSAGE_MOUSEDOWN:    { ret = MAIDTEXT("MOUSEDOWN"); }break;
    case IGUIParam::MESSAGE_MOUSEUP:      { ret = MAIDTEXT("MOUSEUP"); }break;
    case IGUIParam::MESSAGE_BUTTON_CLICK: { ret = MAIDTEXT("BUTTON_CLICK"); }break;
    case IGUIParam::MESSAGE_CHECKBOX_SETCHECK: { ret = MAIDTEXT("CHECKBOX_SETCHECK"); }break;

    case IGUIParam::MESSAGE_RADIOBUTTON_SETCHECK: { ret = MAIDTEXT("RADIOBUTTON_SETCHECK"); }break;
    case IGUIParam::MESSAGE_SLIDER_CHANGEVALUE: { ret = MAIDTEXT("SLIDER_CHANGEVALUE"); }break;

    case IGUIParam::MESSAGE_WINDOW_MINIMUM: { ret = MAIDTEXT("WINDOW_MINIMUM"); }break;
    case IGUIParam::MESSAGE_TEXTBOX_UPDATESTATE: { ret = MAIDTEXT("TEXTBOX_UPDATESTATE"); }break;
    case IGUIParam::MESSAGE_CONTEXTMENU_CLICK:
      {
        const GUIMESSAGE_CONTEXTMENU_CLICK& p = static_cast<const GUIMESSAGE_CONTEXTMENU_CLICK&>(param);

        ret = MAIDTEXT("CONTEXTMENU_CLICK") + String::PrintFormat(" id:%0d",p.SelectID ); 
      }break;

    case IGUIParam::MESSAGE_KEYBORDDOWN:
      {
        const GUIMESSAGE_KEYBORDDOWN& p = static_cast<const GUIMESSAGE_KEYBORDDOWN&>(param);

        ret = MAIDTEXT("KEYBORDDOWN") + String::PrintFormat(" id:%0d",p.Key ); 
      }break;

    case IGUIParam::MESSAGE_KEYBORDUP:
      {
        const GUIMESSAGE_KEYBORDUP& p = static_cast<const GUIMESSAGE_KEYBORDUP&>(param);

        ret = MAIDTEXT("KEYBORDUP") + String::PrintFormat(" id:%0d",p.Key ); 
      }break;

    case IGUIParam::MESSAGE_COMBOBOX_CHANGEELEMENT:
      {
        const GUIMESSAGE_COMBOBOX_CHANGEELEMENT& p = static_cast<const GUIMESSAGE_COMBOBOX_CHANGEELEMENT&>(param);

        ret = MAIDTEXT("COMBOBOX_CHANGEELEMENT") + String::PrintFormat(" id:%0d", p.SelectID ); 

      }break;

    default:
      {
        char buf[256];
        sprintf( buf, "unkwon %03d", param.Message );
        ret = String::ConvertSJIStoMAID(buf);
      }break;
    }

    return ret;
  }

}

