
class SceneSample extends IScene
{
  _Sound = null;

  function Initialize()
  {
    IScene.Initialize();

    _Sound = CppSoundObject();
    _Sound.Type = PcmFile();
    _Sound.Type.FileName = "nc7802.ogg";
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

    //  クリックする度に再生・停止の繰り返し
    if( GetInput().IsIn(MOUSE.L) )
    {
      _Sound.Play = !_Sound.Play;
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


/*
非同期読み込み
import


*/