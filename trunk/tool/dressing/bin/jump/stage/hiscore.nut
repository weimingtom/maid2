//  ハイスコア表示

class HiScore extends DrawObject
{
  _Score = 0

  constructor( score )
  {
    DrawObject.constructor();
    
    _Score = score
    
    Type = ColorText();
    Type.ColorR = 0.0
    Type.ColorG = 0.0
    Type.ColorB = 0.0
    Type.Alpha  = 1.0
    Type.Text = "HiScore:" + _Score
    x = 400
    y = 0
  }

  function UpdateFrame()
  {
  }

}


