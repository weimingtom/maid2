// 画像表示

class DrawObjectImage extends DrawObject
{
  constructor()
  {
    DrawObject.constructor();

    Type = ImageRect3D();   //  画像ファイルを表示するクラス
    Type.FileName = "nc1673.bmp"; //  表示するファイル名。対応ファイルは bmp,png,jpeg
    Type.RectX = 10;  //  表示範囲Ｘ
    Type.RectY = 10;  //  表示範囲Ｙ
    Type.RectW = 100; //  表示範囲width
    Type.RectH = 200; //  height
    Type.Alpha = 1.0  //  透明度
  
    //  画像ファイルは読み込み処理がある都合上、即座には表示されません
    //  この問題はそのうち解決する予定(パラメータを追加する等)
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

    _obj = DrawObjectImage();

    GetDrawObjectManager().InsertDrawObject( _obj );
  }

  function IsExit()
  {
    return false;
  }

  function UpdateFrame()
  {
    IScene.UpdateFrame();
    
    _obj.x = 400;
    _obj.y = 300;

    // Lボタンをおしてる間、不透明になる。
    if( GetInput().IsDown(MOUSE.L) )
    {
      _obj.Type.Alpha = 1.0;
    }else{
      _obj.Type.Alpha = 0.5;
    }
  }
}


class GameRoutine extends IGameRoutine
{
  // 一番最初のシーン作成時に呼ばれるメソッド
  // この中で次に行うシーンを作成する
  function Initialize()
  {
    SetScene( SceneSample() );
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


