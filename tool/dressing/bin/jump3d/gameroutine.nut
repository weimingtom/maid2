import( "stage/scene.nut" );
import( "tittle/scene.nut" );

enum SCENEID
{
  TITTLE,
  STAGE,
}

class GameRoutine extends IGameRoutine
{
  static SCENEID_TITTLE = 0
  static SCENEID_STAGE  = 1

  _SceneID = 0

  // 一番最初のシーン作成時に呼ばれるメソッド
  // この中で次に行うシーンを作成する
  function Initialize()
  {
    _SceneID = SCENEID_TITTLE
    SetScene( SceneTittle() );
  }

  // 現在実行中のシーンが終了したあとに呼ばれるメソッド
  // @param exitcode Scene.Finalize() で渡される終了情報
  function CreateNextScene( exitcode )
  {
    switch( _SceneID )
    {
    case SCENEID_TITTLE:
      {
        _SceneID = SCENEID_STAGE
        SetScene( SceneStage() );
      }break;
    
    case SCENEID_STAGE:
      {
        _SceneID = SCENEID_TITTLE
        SetScene( SceneTittle() );
      }break;
    }
  }
}

