
class Model extends DrawObject
{
  _Count = 0
  
  constructor()
  {
    DrawObject.constructor();
    
    Type = ModelFile(); //  色付の四角形を作ります
    Type.FileName = "block.mqo"
    Type.Alpha = 1;     //  不透明度 (0.0で見えなくなる)

    x = 300
    y = 400
  }

  function UpdateFrame()
  {
    _Count += 1;
    _Count %= 360
    
    Type.Matrix = MATRIX4D().SetRotationY( DEGtoRAD(_Count.tofloat()) )
  }


}


