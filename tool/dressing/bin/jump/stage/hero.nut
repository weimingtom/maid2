class Hero extends DrawObject
{
  // squirrelの言語仕様により コンストラクタを定義する場合は
  // 一緒に親のコンストラクタを呼ぶこと

  static STATE_STAND = 0
  static STATE_JUMP = 1
  static WALK_BRAKE = 0.5 // 横加速度のブレーキ
  static WALK_POW   = 1.0 // 横加速度
  static WALK_POWMAX= 2.0 // 横加速度最大値
  static JUMP_POW   = 1.0 // 縦加速度
  static JUMP_POWMAX= 20.0 // 縦加速度最大値
  

  _State = 0  //  現在の状態
  _JumpPow = 0.0    //  ジャンプ力
  _Land = null      //  現在着地してる地面(nullなら浮いてる)

  _WalkPow = 0.0    // 横加速度
  
  constructor()
  {
    DrawObject.constructor();
    
    Type = ColorRect();//  色付の四角形を作ります
    Type.Width  = 64;  //  横幅pixel
    Type.Height = 64;  //  縦幅
    Type.ColorR = 0;   //  色 red
    Type.ColorG = 0;   //  色 green
    Type.ColorB = 0;   //  色 blue
    Type.Alpha  = 1;   //  不透明度 (0.0で見えなくなる)

    Type.CenterX = Type.Width/2;   // オフセット。回転させる時などに使う基準位置
    Type.CenterY = Type.Height;

     x = 64
    _JumpPow = 0
    _WalkPow = 0
    _State = STATE_STAND
  }

  function UpdateFrame()
  {
    x += _WalkPow;

    switch( _State )
    {
    case STATE_STAND:
      {
        x -= _Land._Speed
      }break;

    case STATE_JUMP:
      {
        y += -_JumpPow  //  y座標系が↓に正なので反転する
        _JumpPow -= JUMP_POW
      }
    }
    
    { // 画面外にでないようにクリップ
      if( x< 50.0 ) { x = 50.0 }
      if( 750.0 < x ) { x = 750.0 }
    }
    { // 横加速度調節
           if( _WalkPow < -WALK_BRAKE ) { _WalkPow += WALK_BRAKE; }
      else if( WALK_BRAKE < _WalkPow  ) { _WalkPow -= WALK_BRAKE; }
      else { _WalkPow = 0.0 }
    }
    { //  縦加速度調節
      if( _JumpPow < -JUMP_POWMAX ) { _JumpPow = -JUMP_POWMAX }
    }
  }

  function Jump()
  {
    _JumpPow = JUMP_POWMAX
    _State = STATE_JUMP;
    _Land = null;
  }
  
  function Fall()
  {
    _State = STATE_JUMP;
    _JumpPow = 0
    _Land = null;
  }
  
  function Land( block )
  {
    _State = STATE_STAND;
    _JumpPow = 0
    _Land = block;
    
    local rc = block.GetCollision()
    y = rc.top
  }
  
  function HitTop( block )
  {
    _JumpPow = 0
    y = block.GetCollision().bottom + Type.CenterY
  }

  function HitRight( block )
  {
    _WalkPow = 0
    x = block.GetCollision().left - Type.CenterX
  }

  function HitLeft( block )
  {
    _WalkPow = 0
    x = block.GetCollision().right + Type.CenterX
  }

  function WalkR()
  {
    _WalkPow += WALK_POW
    if( _WalkPow<WALK_POWMAX ) { _WalkPow = WALK_POWMAX }
  }
  
  function WalkL()
  {
    _WalkPow -= WALK_POW
    if( -WALK_POWMAX<_WalkPow ) { _WalkPow = -WALK_POWMAX }
  }

  function GetState()
  {
    return _State
  }
  
  function GetTopCollision()
  {
    local tmp = GetCollision()
    return RECT2D(tmp.left+10, tmp.top, tmp.w-10-10, 10 );
  }

  function GetBottomCollision()
  {
    local tmp = GetCollision()
    return RECT2D(tmp.left+10, tmp.bottom-5, tmp.w-10-10, 10 );
  }

  function GetRightCollision()
  {
    local tmp = GetCollision()
    return RECT2D( tmp.right-5, tmp.top+5, 5, tmp.h-5-5 );
  }

  function GetLeftCollision()
  {
    local tmp = GetCollision()
    return RECT2D( tmp.left, tmp.top+5, 5, tmp.h-5-5 );
  }

  function GetCollision()
  {
    return RECT2D( x-Type.CenterX, y-Type.CenterY, Type.Width, Type.Height )
  
  }
}


