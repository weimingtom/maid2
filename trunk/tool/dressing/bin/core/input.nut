class KeyInput
{
  _Keybord = null
  _MouseButton = null;
  _MousePos = null;
  _MouseDelta = null;

  function Update()
  {
     local ret = suspend(["inputstate"]);

    _Keybord     = ret[1];
    _MouseButton = ret[2];
    _MousePos    = ret[3];
    _MouseDelta  = ret[4];
  }

  function GetButton( pos )
  {
    if( pos <= 0xFF ) { return _Keybord[pos]; }
    return _MouseButton[pos-0x0100];
  }

  function IsDown( pos )
  {
    return GetButton(pos) & (1<<0)
  }

  function IsIn( pos )
  {
    return GetButton(pos) & (1<<1)
  }

  function IsOut( pos )
  {
    return GetButton(pos) & (1<<2)
  }

  function GetPos(pos)
  {
    return _MousePos[pos];
  }
  function GetDelta(pos)
  {
    return _MouseDelta[pos];
  }

}


