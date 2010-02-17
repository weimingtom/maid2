//  地面オブジェクト。自機が乗れる

class Block extends DrawObject
{
  _Speed = 0  //  スクロールスピード
  
  constructor( w, h, speed )
  {
    DrawObject.constructor();
    
    Type = ColorRect(); //  色付の四角形を作ります
    Type.Width = w;   //  横幅pixel
    Type.Height = h;  //  縦幅
    Type.ColorR = 1.0;    //  色 red
    Type.ColorG = 0.5;  //  色 green
    Type.ColorB = 0;    //  色 blue
    Type.Alpha = 1;     //  不透明度 (0.0で見えなくなる)

    Type.CenterX = 0;   // オフセット。回転させる時などに使う基準位置
    Type.CenterY = 0;
    
    _Speed = speed
  }

  function UpdateFrame()
  {
    x -= _Speed
  }
  
  function GetCollision()
  {
    return RECT(x-Type.CenterX, y-Type.CenterY, Type.Width, Type.Height )
  }
}


