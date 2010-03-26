// ColorRect の使い方

class BackGround extends DrawObject
{
  // squirrelの言語仕様により コンストラクタを定義する場合は
  // 一緒に親のコンストラクタを呼ぶこと
  
  
  constructor()
  {
    DrawObject.constructor();
    
    Type = ColorRect(); //  色付の四角形を作ります
    Type.Width = 800;   //  横幅pixel
    Type.Height = 600;  //  縦幅
    Type.ColorR = 0;    //  色 red
    Type.ColorG = 1;    //  色 green
    Type.ColorB = 1;    //  色 blue
    Type.Alpha = 1;     //  不透明度 (0.0で見えなくなる)

    Type.CenterX = 0;   // オフセット。回転させる時などに使う基準位置
    Type.CenterY = 0;

  }

}


