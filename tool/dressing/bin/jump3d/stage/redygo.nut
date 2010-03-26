//  redygoオブジェクト

class RedyGo extends DrawObject
{
  _Gen = null

  constructor()
  {
    DrawObject.constructor();
    
    Type = ColorText();
    Type.ColorR = 0.0
    Type.ColorG = 0.0
    Type.ColorB = 0.0
    Type.Alpha  = 1.0
    Type.FontW = 64
    Type.FontH = 32
    
    Type.LayoutX = ColorText.LAYOUT_CENTER
    Type.LayoutY = ColorText.LAYOUT_CENTER
    x = 400
    y = 300
    z = -50
    
    _Gen = Loop()
  }
  
  function UpdateFrame()
  {
    if( _Gen.getstatus()=="suspended" )
    {
      resume _Gen
    }
  }
  
  function Loop()
  {
    Type.Text = "REDY"; yield
    for( local i=0; i<60; ++i) { yield }

    Type.Text = "GO!";  Type.ColorR=1.0;  Type.ColorG=1.0; Type.ColorB=1.0; yield
    for( local i=0; i<60; ++i) { yield }
  }
  
  function IsEnd()
  {
    return _Gen.getstatus()=="dead"
  }
}


