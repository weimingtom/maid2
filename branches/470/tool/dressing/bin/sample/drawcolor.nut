// ColorRect の使い方

class DrawObjectColor extends DrawObject
{
  // squirrelの言語仕様により コンストラクタを定義する場合は
  // 一緒に親のコンストラクタを呼ぶこと
  
  
  constructor()
  {
    DrawObject.constructor();
    
    Type = ColorRect(); //  色付の四角形を作ります
    Type.Width = 100;   //  横幅pixel
    Type.Height = 200;  //  縦幅
    Type.ColorR = 0;    //  色 red
    Type.ColorG = 0.5;  //  色 green
    Type.ColorB = 0;    //  色 blue
    Type.Alpha = 1;     //  不透明度 (0.0で見えなくなる)

    Type.CenterX = 0;   // オフセット。回転させる時などに使う基準位置
    Type.CenterY = 0;

  }

  function UpdateFrame()
  {
  }
}


class SceneSample extends IScene
{
  _obj = null

  function Initialize()
  {
    IScene.Initialize();

    _obj = DrawObjectColor();

    GetDrawObjectManager().InsertDrawObject( _obj );
  }

  function Finalize()
  {
    IScene.Finalize();
  }

  function IsExit()
  {
    return false;
  }

  function UpdateFrame()
  {
    IScene.UpdateFrame();
    
    local x = GetInput().GetPos(MOUSE.X);
    local y = GetInput().GetPos(MOUSE.Y);
    
    // マウス座標の位置に表示させる
    _obj.x = x;
    _obj.y = y;
    
    // Lボタンをおしてる間、緑が濃くなる
    if( GetInput().IsDown(MOUSE.L) )
    {
      _obj.Type.ColorG = 1.0;
    }else{
      _obj.Type.ColorG = 0.5;
    }
  }
}


class GameRoutine extends IGameRoutine
{
  // 一番最初のシーン作成時に呼ばれるメソッド
  // この中で次に行うシーンを作成する
  function CreateFirstScene()
  {
    return SceneSample();
  }

  // 現在実行中のシーンが終了したあとに呼ばれるメソッド
  // \param exitcode Scene.GetExitCode() で渡される終了情報
  function CreateNextScene( exitcode )
  {
    SetScene( SceneSample() );
  }
}



//	いろいろ定義したのでゲーム開始！
Run( GameRoutine() );

