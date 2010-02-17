/*!
  @class  Task task.nut
  @brief  タスククラス
  \n      各種処理はこのクラスを継承して実装する
  \n      タスクとは、なにかしらの１処理と考えるといい。
  \n      このクラスは描画機能をもたせてはいけない。DrawObjectを使うこと
*/
class Task
{
  _Group   = null;

  // タスクの動作が開始される時に呼ばれる
  function Initialize()
  {
  }
  
  // タスクの動作が終了される時(Kill()が呼ばれた後)に呼ばれる
  function Finalize()
  {
  
  }
  
  // フレーム毎に呼ばれる関数
  // 各種処理はここで書くこと
  function UpdateFrame()
  {
  }

  // このオブジェクトが用済みになった合図
  function Kill()
  {
    _Group.GetManager().RemoveTask( GetPriority(), this );
  }
 
  function SetGroup( g )
  {
    _Group = g;
  }
  
  function GetPriority()
  {
    return _Group.GetPriority();
  }
}

/*!
  @class  TaskGroup dressinglib.nut
  @brief  複数のTaskを一括して操作するクラス
*/
class TaskGroup
{
  _Task = null;
  _Manager = null;
  _Priority = 0;

  function Initialize( manager, priority )
  {
    _Task = [];
    _Manager = manager;
    _Priority= priority;
  }

  function Finalize()
  {
    _Task = null;
    _Manager = null;
    _Priority= 0;
  }


  function UpdateFrame()
  {
    foreach( obj in _Task )
    {
      obj.UpdateFrame();
    }
  }

  function InsertTask( task )
  {
    task.SetGroup(this);
    _Task.push( task );
  }

  function RemoveTask( task )
  {
    foreach( index, ele in _Task )
    {
      if( ele==task ) { _Task.remove(index); break; }
    }
  }
  
  function GetManager()
  {
    return _Manager;
  }
  function GetPriority()
  {
    return _Priority;
  }
  
  function GetTask()
  {
    return _Task;
  }
}

/*!
  @class  TaskManager dressinglib.nut
  @brief  Taskを優先度に沿って管理するクラス
*/
class TaskManager
{
  _GroupList = null; // タスクリスト _GroupList[xx].UpdateFrame()とかやる

  _InsertList= null; // _TaskListに追加されるリスト。中身は[priority,task]のテーブル
  _RemoveList= null; // _TaskListから排除されるリスト。中身は[priority,task]のテーブル

  // 初期化
  // 戻り値なし
  function Initialize()
  {
    _GroupList = [];
    _InsertList= [];
    _RemoveList= [];
    for( local i=0; i<10; ++i )
    {
      local group = TaskGroup();
      group.Initialize(this, i);
      _GroupList.push( group );
    }
  }

  // 終了
  function Finalize()
  {
    foreach( group in _GroupList )
    {
      local tasklist = group.GetTask();
      foreach( task in tasklist )
      {
        task.Finalize();
      }
      group.Finalize();
    }

    _GroupList = null;
    _InsertList= null;
    _RemoveList= null;
  }

  // フレーム毎の更新
  // 戻り値無し
  function UpdateFrame()
  {
   // オブジェクトの更新
    {
      foreach( group in _GroupList )
      {
        group.UpdateFrame();
      }
    }

    // 不要になったオブジェクトの削除
    {
      foreach( dat in _RemoveList )
      {
        dat.task.Finalize();
        _GroupList[dat.priority].RemoveTask( dat.task );
      }
      _RemoveList.clear();
    }

    UpdateInsertTask()
  }

  // 新しく作成されたオブジェクトの設定
  function UpdateInsertTask()
  {
    foreach( dat in _InsertList )
    {
      _GroupList[dat.priority].InsertTask( dat.task );
      dat.task.Initialize();
    }
    _InsertList.clear();
  }

  function InsertTask( p, t )
  {
   local dat =
    {
      priority=p
      task    =t
    }

    _InsertList.push( dat );  
  }

  function RemoveTask( p, t )
  {
    local dat =
    {
      priority=p
      task    =t
    }

    _RemoveList.push( dat );  
  }
}


