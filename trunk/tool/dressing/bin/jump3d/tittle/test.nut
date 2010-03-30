
class Model extends DrawObject
{
  constructor()
  {
    DrawObject.constructor();
    
    Type = ModelFile(); //  色付の四角形を作ります
    Type.FileName = "block.mqo"
    Type.Alpha = 1;     //  不透明度 (0.0で見えなくなる)

    x = 400
    y = 150
  }

  function UpdateFrame()
  {
    Type.Matrix = MATRIX4D().SetRotationY( DEGtoRAD(45.0) )
  }


}


