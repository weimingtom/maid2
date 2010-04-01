//  地面オブジェクト。自機が乗れる

class Block extends DrawObject
{
  _Speed = 0  //  スクロールスピード
  
  _Width = 0
  _Height = 0
  
  constructor( w, h, speed )
  {
    DrawObject.constructor();
    
    Type = ModelFile(); //  色付の四角形を作ります
    Type.FileName = "block.mqo"
    Type.Alpha = 1;     //  不透明度 (0.0で見えなくなる)
    
    _Speed = speed
    
    _Width  = w
    _Height = h
    
    Type.Matrix = MATRIX4D().SetScale( w.tofloat()/200.0, h.tofloat()/200.0, 1.0 )
  }

  function UpdateFrame()
  {
    x -= _Speed
  }
  
  function GetCollision()
  {
    return RECT2D(x, y-_Height, _Width, _Height )
  }
}


