/*!
  @class  DrawObject drawobject.nut
  @brief  描画オブジェクトクラス
  \n      ゲーム画面に何か表示させたい場合は、このクラスを継承します
  \n      表示タイプは this.Type メンバに入れればOKです
*/
class DrawObject extends CppDrawObject
{
  _Manager = null
  _Chiled = null  //  親子関係にある DrawObject 

  constructor()
  {
    CppDrawObject.constructor();
    _Chiled = []
  }


  // 動作が開始される時に呼ばれる
  function Initialize()
  {
  }

  // 動作が終了される時(Kill()が呼ばれた後)に呼ばれる
  function Finalize()
  {
    // 子に親の情報を加えておく
    foreach( ch in _Chiled )
    {
      local pos = ch.CalcGlobalPos();
      ch.x = pos.x
      ch.y = pos.y
      ch.z = pos.z
      ch.Parent = null
    }
    _Chiled = []
  }

  // フレーム毎に呼ばれる関数
  // 各種処理はここで書くこと
  function UpdateFrame()
  {
  }


  // このオブジェクトが用済みになったら呼ぶ
  // 次のフレームから UpdateFrame() が呼ばれなくなります
  function Kill()
  {
    _Manager.RemoveDrawObject( this )
  }

  // ch と親子関係を結ぶ
  // this が親で ch が子です
  // 親子関係になると座標系が変わります
  function AddChiled( ch )
  {
    ch.Parent = this;
    _Chiled.push( ch );
  }
  
  function DelChiled( ch )
  {
    foreach( i, obj in _Chiled )
    {
      if( obj==ch ) { _Chiled.remove(i); break; }
    }
    
    local pos = ch.CalcGlobalPos();
    ch.x = pos.x
    ch.y = pos.y
    ch.z = pos.z
    ch.Parent = null
  }
  
  function DelParent()
  {
    if( ch.Parent==null ) { return ; }

    ch.Parent.DelChiled(this);
  }
  
  function SetManager( m )
  {
    _Manager = m
  }

  //! 絶対座標を求める
  /*!
      @return 絶対座標が入ったテーブル {x,y,z}
   */
  function CalcGlobalPos()
  {
    if( Parent==null ) { return {x=this.x,y=this.y,z=this.z} }
    
    local tmp = Parent.CalcGlobalPos()
    tmp.x += x
    tmp.y += y
    tmp.z += z
    
    return tmp
  }
}


class DrawObjectManager extends Task
{
  _ObjectList= null;  // CppDrawObject を継承した配列
  _InsertList= null;  // 次フレームで実行されるオブジェクト CppDrawObject を継承した配列
  _RemoveList= null;  // 次フレームで削除されるオブジェクト CppDrawObject を継承した配列

  constructor()
  {
    _ObjectList= [];
    _InsertList= [];
    _RemoveList= [];
  }
  
  function Initialize()
  {
    UpdateInsertObject();
  }
  function Finalize()
  {
    // _RemoveList に先に入っているものを消すべきかも？
    foreach( obj in _ObjectList)
    {
      obj.Finalize();
    }
    _ObjectList.clear();
    _InsertList.clear();
    _RemoveList.clear();
  }

  function UpdateFrame()
  {
    foreach( obj in _ObjectList)
    {
      obj.UpdateFrame();
    }

    // 不要になったオブジェクトの削除
    {
      foreach( obj in _RemoveList )
      {
        obj.Finalize();
        foreach( index, ele in _ObjectList )
        {
          if( ele==obj ) { _ObjectList.remove(index); break; }
        }
      }
      _RemoveList.clear();
    }

    // 新しく作成されたオブジェクトの設定
    UpdateInsertObject();

  }

  function UpdateInsertObject()
  {
    foreach( obj in _InsertList )
    {
      _ObjectList.push( obj );
      obj.SetManager( this );
      obj.Initialize();
    }
    _InsertList.clear();
  }


  function GetAllDrawObject()
  {
    return _ObjectList;
  }

  function InsertDrawObject( obj )
  {
    _InsertList.push( obj );  
  }

  function RemoveDrawObject( obj )
  {
    _RemoveList.push( obj );  
  }

}



