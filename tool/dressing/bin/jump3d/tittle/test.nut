// ColorRect の使い方

class Test3D extends DrawObject
{
  // squirrelの言語仕様により コンストラクタを定義する場合は
  // 一緒に親のコンストラクタを呼ぶこと
  
  _Scale = 0
  
  constructor()
  {
    DrawObject.constructor();
    
    Type = ColorRect3D(); //  色付の四角形を作ります
    Type.Width = 100;   //  横幅pixel
    Type.Height = 100;  //  縦幅
    Type.ColorR = 0;    //  色 red
    Type.ColorG = 1;    //  色 green
    Type.ColorB = 1;    //  色 blue
    Type.Alpha = 1;     //  不透明度 (0.0で見えなくなる)

    Type.CenterX = 0;   // オフセット。回転させる時などに使う基準位置
    Type.CenterY = 0;

    x = 400
    y = 150
  }

  function UpdateFrame()
  {
    _Scale += 1
    _Scale %= 5
    
    if( _Scale==0 ) { _Scale = 1 }

    Type.Matrix = MATRIX4D().SetScale( 1, _Scale, 1 )
//                * MATRIX4D().SetScale( _Scale, 1, 1 )
                  *MATRIX4D().SetRotationY( DEGtoRAD(45.0) )

//                * MATRIX4D().SetRotationY( DEGtoRAD(45.0) )
                
  
  }


}


