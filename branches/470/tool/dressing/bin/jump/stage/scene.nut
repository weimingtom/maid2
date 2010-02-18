import( "hero.nut" );
import( "helptext.nut" );
import( "background.nut" );
import( "block.nut" );
import( "score.nut" );
import( "gameover.nut" );
import( "redygo.nut" );

class SceneStage extends IScene
{
  _bg = null
  _makecount = 0  //  地面が作られるタイミングを管理するカウンタ。０になると作成する
  _Block = null   //  地面データが入った配列
  _Hero = null  //  自機
  _HelpText = null  //  キーヘルプ

  _Score = null //  スコア表示

  static STATE_LEDY = 0  //  ゲーム開始前
  static STATE_GAME = 1  //  ゲームプレイ中
  static STATE_GAMEOVER = 2 // ゲームオーバー
  static STATE_END = 3 // 終了待ち

  _State = 0  //  現在のゲーム状態（STATE_LEDY,STATE_GAME,STATE_GAMEOVERのどれか）

  _RedyGo = null

  function SetState( s )
  {
    _State = s
  }

  function Initialize()
  {
    IScene.Initialize();

    _makecount = 100;
    _Block = []
    _State = STATE_LEDY

    {
      _bg = BackGround();
      _bg.z = 100.0

      GetDrawObjectManager().InsertDrawObject( _bg );
    }

    {
      // 最初の一個はゲームが始まるまでの一個
      local tmp = Block( 1000, 40, 1 )
      tmp.x = 0;
      tmp.y = 600-40
      GetDrawObjectManager().InsertDrawObject( tmp );
      _Block.push(tmp);
    }
    
    {
      _Hero = Hero();
      GetDrawObjectManager().InsertDrawObject( _Hero );
    }
    {
      _HelpText = HelpText()
      _HelpText.z = -100
      _HelpText.SetPlayText()
      GetDrawObjectManager().InsertDrawObject( _HelpText );
    }
    {
      _Score = Score( _Hero )
      GetDrawObjectManager().InsertDrawObject( _Score );
    }
    {
      _RedyGo = RedyGo()
      GetDrawObjectManager().InsertDrawObject( _RedyGo );
    }
  }

  function Finalize()
  {
    IScene.Finalize();
  }

  function IsExit()
  {
    return _State==STATE_END;
  }
  
  //  rect とぶつかっているブロックを探す
  //  @return ぶつかっている Block  インスタンス
  //          ぶつかってなければ null
  function FindHitBlock( rect )
  {
    foreach( block in _Block )
    {
      local b = block.GetCollision()
      if( COLLISION.RectRect( rect, b ) )
      {
        return block
      }
    }  
    return null
  }

  function UpdateFrame()
  {
    IScene.UpdateFrame();

    UpdateGameRule()
    switch( _State )
    {
    case STATE_LEDY:    { UpdateFrameLedy()     }break;
    case STATE_GAME:    { UpdateFrameGame()     }break;
    case STATE_GAMEOVER:{ UpdateFrameGameOver() }break;
    case STATE_END:{ }break;
    }
  }
  

  function UpdateFrameLedy()
  {
    if( _RedyGo.IsEnd() )
    {
      _RedyGo.Kill();
      _Score.Start()
      SetState(STATE_GAME)
    }
  }
  
  function UpdateFrameGame()
  {
    if( GetInput().IsDown(KEYBOARD.LEFT ) ) { _Hero.WalkL() }
    if( GetInput().IsDown(KEYBOARD.RIGHT) ) { _Hero.WalkR() }
    if( GetInput().IsIn('Z')&& _Hero.GetState()==Hero.STATE_STAND) { _Hero.Jump() }
    
    
    //  自機が画面下に消えたらゲームオーバー
    if( 700 < _Hero.y )
    {
      local tmp = GameOver()
      GetDrawObjectManager().InsertDrawObject( tmp )
      
      _Score.Stop()
      _HelpText.SetGameOverText()
      SetState(STATE_GAMEOVER)

    }
  }
  
  function UpdateFrameGameOver()
  {
    if( GetInput().IsIn(KEYBOARD.ENTER) ) { SetState(STATE_END) }
  
  }
  
  function UpdateGameRule()
  {
    {
      // 床との当たり判定。
      local block = FindHitBlock( _Hero.GetBottomCollision() );
      
      // ジャンプ中と着地中で挙動が違う確認
      if( _Hero.GetState()==Hero.STATE_STAND )
      {
        // 床との当たり判定。床にぶつかってないなら落下する
        // ぶつかっているならブロックのスピード分だけ左へ移動する（乗ってる雰囲気）
        if( block==null )
        {
          _Hero.Fall()
        }
      }
      else if( _Hero.GetState()==Hero.STATE_JUMP )
      {
        if( block != null )
        {
          _Hero.Land( block )
        }
      }
    }
    
    {
      // 天井との当たり判定
      local block = FindHitBlock( _Hero.GetTopCollision() );
      if( block!=null )
      {
        _Hero.HitTop( block )
      }    
    }

    {
      // 右壁との当たり判定
      local block = FindHitBlock( _Hero.GetRightCollision() );
      if( block!=null )
      {
        _Hero.HitRight( block )
      }
    }
    {
      // 左壁との当たり判定
      local block = FindHitBlock( _Hero.GetLeftCollision() );
      if( block!=null )
      {
        _Hero.HitLeft( block )
      }
    }

    //  いらなくなった壁の削除
    //  左に近い順にソートされてるはずなので、(0,y)を過ぎた物を削除する
    while( true )
    {
      if( _Block.len()==0 ) { break; }

      local w = _Block[0].x + _Block[0].Type.Width
      if( w < 0 ) { _Block.remove(0); }
      else { break; }
    }

    GameLevelCheck();  
  }

  function GameLevelCheck()
  {
    // 新しく壁を作る
    if( _makecount==0 )
    {
      local w = RandI(100,200)
      local tmp = Block( w, 40, 1 )
      tmp.x = 800;
      tmp.y = 600-100 - RandI(0,100)

      GetDrawObjectManager().InsertDrawObject( tmp );
      _Block.push(tmp);

      _makecount = RandI(200,500)
   }else
   {
      _makecount--
   }
  }

}


