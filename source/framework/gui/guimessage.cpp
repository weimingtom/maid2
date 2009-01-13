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

    case IGUIParam::MESSAGE_MOUSEMOVE:    { ret = MAIDTEXT("MOUSEMOVE"); }break;
    case IGUIParam::MESSAGE_MOUSEIN:      { ret = MAIDTEXT("MOUSEIN"); }break;
    case IGUIParam::MESSAGE_MOUSEOUT:     { ret = MAIDTEXT("MOUSEOUT"); }break;
    case IGUIParam::MESSAGE_MOUSEDOWN:    { ret = MAIDTEXT("MOUSEDOWN"); }break;
    case IGUIParam::MESSAGE_MOUSEUP:      { ret = MAIDTEXT("MOUSEUP"); }break;
    case IGUIParam::MESSAGE_BUTTON_CLICK: { ret = MAIDTEXT("BUTTON_CLICK"); }break;
    case IGUIParam::MESSAGE_CHECKBOX_SETCHECK: { ret = MAIDTEXT("CHECKBOX_SETCHECK"); }break;

    case IGUIParam::MESSAGE_RADIOBUTTON_SETCHECK: { ret = MAIDTEXT("RADIOBUTTON_SETCHECK"); }break;

    case IGUIParam::MESSAGE_RADIDGROUP_MOUSEMOVE: { ret = MAIDTEXT("RADIDGROUP_MOUSEMOVE"); }break;
    case IGUIParam::MESSAGE_RADIDGROUP_MOUSEIN: { ret = MAIDTEXT("RADIDGROUP_MOUSEIN"); }break;
    case IGUIParam::MESSAGE_RADIDGROUP_MOUSEOUT: { ret = MAIDTEXT("RADIDGROUP_MOUSEOUT"); }break;
    case IGUIParam::MESSAGE_RADIDGROUP_MOUSEDOWN: { ret = MAIDTEXT("RADIDGROUP_MOUSEDOWN"); }break;
    case IGUIParam::MESSAGE_RADIDGROUP_MOUSEUP: { ret = MAIDTEXT("RADIDGROUP_MOUSEUP"); }break;

    case IGUIParam::MESSAGE_RADIDGROUP_CHANGESELECT: { ret = MAIDTEXT("RADIDGROUP_CHANGESELECT"); }break;
    }

    return ret;
  }

}

