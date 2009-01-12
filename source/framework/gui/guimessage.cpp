#include"guimessage.h"

namespace Maid
{

  String CreateMessageLog( const IGUIParam& param )
  {
    String ret;


    switch( param.Message )
    {
    case IGUIParam::MESSAGE_INITIALIZE:   { ret = MAIDTEXT("INITIALIZE"); }break;
    case IGUIParam::MESSAGE_FINALIZE:     { ret = MAIDTEXT("FINALIZE"); }break;
    case IGUIParam::MESSAGE_UPDATEFRAME:  { ret = MAIDTEXT("UPDATEFRAME"); }break;
    case IGUIParam::MESSAGE_UPDATEDRAW:   { ret = MAIDTEXT("UPDATEDRAW"); }break;
    case IGUIParam::MESSAGE_SETPOSITION:  { ret = MAIDTEXT("SETPOSITION"); }break;
    case IGUIParam::MESSAGE_ENABLE:       { ret = MAIDTEXT("ENABLE"); }break;
    case IGUIParam::MESSAGE_VISIBLE:      { ret = MAIDTEXT("VISIBLE"); }break;
    case IGUIParam::MESSAGE_MOUSEMOVE:    { ret = MAIDTEXT("MOUSEMOVE"); }break;
    case IGUIParam::MESSAGE_MOUSEIN:      { ret = MAIDTEXT("MOUSEIN"); }break;
    case IGUIParam::MESSAGE_MOUSEOUT:     { ret = MAIDTEXT("MOUSEOUT"); }break;
    case IGUIParam::MESSAGE_MOUSEDOWN:    { ret = MAIDTEXT("MOUSEDOWN"); }break;
    case IGUIParam::MESSAGE_MOUSEUP:      { ret = MAIDTEXT("MOUSEUP"); }break;
    case IGUIParam::MESSAGE_BUTTON_CLICK: { ret = MAIDTEXT("BUTTON_CLICK"); }break;
    case IGUIParam::MESSAGE_CHECKBOX_SETCHECK: { ret = MAIDTEXT("CHECKBOX_SETCHECK"); }break;
    }

    return ret;
  }

}

