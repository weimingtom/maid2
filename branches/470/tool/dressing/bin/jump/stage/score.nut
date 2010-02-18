//  スコア表示

class Score extends DrawObject
{
  _Score = 0
  _Hero = null  //  自機の参照。スコア計算に使う
  
  _IsPlay = false //  カウントを開始してるか？

  constructor( h )
  {
    DrawObject.constructor();
    
    _Score = 0
    _Hero  = h
    
    Type = ColorText();
    Type.ColorR = 0.0
    Type.ColorG = 0.0
    Type.ColorB = 0.0
    Type.Alpha  = 1.0
    x = 600
    y = 0
  }

  function Initialize()
  {
    UpdateScore();
  }

  function UpdateFrame()
  {
  if( !_IsPlay ) { return }

    _Score += _Hero.x.tointeger()
    UpdateScore();
  }
  
  function UpdateScore()
  {
    Type.Text = "Score:" + _Score
  }

  function Start()
  {
    _IsPlay = true
  }

  function Stop()
  {
    _IsPlay = false
  }

  function GetScore()
  {
    return _Score
  }
}


