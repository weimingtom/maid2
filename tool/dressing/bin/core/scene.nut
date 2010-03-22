class IScene
{
  _TaskManager       = null;
  _DrawObjectManager = null;
  _Admin = null;  //  このシーンを管理してる SceneAdmin

  _Camera = null

  constructor()
  {
  }


  // このシーンの初期化
  // 戻り値なし
  function Initialize()
  {
    _Camera = Camera()
    _TaskManager = TaskManager();
    _DrawObjectManager = DrawObjectManager();
    _TaskManager.Initialize();
    _TaskManager.InsertTask( 5, _DrawObjectManager );
  }

  // このシーンの開放
  // この関数を呼び出したら、もう実行できません
  // このシーンの終了状態（各自決めること）
  function Finalize()
  {
    _TaskManager.Finalize();
    _TaskManager = null;
    _DrawObjectManager = null;
  }

  // このシーンを終了してもいいか？
  // 終了OKなら true , NGならfalse
  function IsExit()
  {
    return false;
  }

  // フレーム毎の更新
  // 戻り値無し
  function UpdateFrame()
  {
    _TaskManager.UpdateFrame();
  }

  //	描画に使うゲームオブジェクトを取得する
  function GetAllDrawObject()
  {
    return _DrawObjectManager.GetAllDrawObject();
  }
  
  function GetDrawObjectManager()
  {
    return _DrawObjectManager;  
  }
  
  function GetInput()
  {
    return _Admin._KeyInput;
  }
}

class SceneAdmin
{
  _GameRoutine = null
  _SceneStack  = null // シーン配列。処理は[0]が最初。描画は[0]が一番奥
  _KeyInput    = null // キーボード・マウスの状態

  function Initialize( routine, input )
  {
    _GameRoutine = routine
    _GameRoutine._SceneAdmin = this;
    _SceneStack= [null]
    _KeyInput = input;
    
    _GameRoutine.Initialize();
  }

  function Finalize()
  { // CppDrawObject が持っている hDrawType をcpp側から sq_release() するために
    // ファイナライザの呼び出しが必須になる。
    {
      // 作成が遅い順に終わっていく
      while( true )
      {
        if( _SceneStack.len()==0 ) { break; }
        _SceneStack.top().Finalize();
        _SceneStack.pop();
      }
    }

    _GameRoutine = null;
    _SceneStack= null;
    _KeyInput = null;
  }

  function UpdateFrame()
  {
    if( _SceneStack.top().IsExit() )
    {
      suspend(["scenefade",100]);
      local retcode = _SceneStack.top().Finalize();
      _GameRoutine.CreateNextScene( retcode );
    }else
    {
      _SceneStack.top().UpdateFrame();
    }
  }

  function GetDrawObject()
  {
    local ret = [];
    foreach( scene in _SceneStack )
    {
      local tmp = 
      {
        camera = scene._Camera,
        object = scene.GetAllDrawObject()
      }
      ret.push( tmp );
    }
    return ret;
  }

  function SetScene( scene )
  {
    scene._Admin = this;
    scene.Initialize()
    _SceneStack[_SceneStack.len()-1] = scene;
  }

  function PushScene( scene )
  {
    scene._Admin = this;
    scene.Initialize()
    _SceneStack.push( scene );
  }

  function PopScene()
  {
    _SceneStack.pop()
  }
}

