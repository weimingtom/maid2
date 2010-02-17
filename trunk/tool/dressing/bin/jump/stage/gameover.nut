//  ゲームオーバーオブジェクト

class GameOver extends DrawObject
{
  constructor()
  {
    DrawObject.constructor();
    
    Type = ColorText();
    Type.Text = "GAME OVER"
    Type.ColorR = 0.0
    Type.ColorG = 0.0
    Type.ColorB = 0.0
    Type.Alpha  = 1.0
    Type.FontW = 64
    Type.FontH = 32
    
    Type.LayoutX = ColorText.LAYOUT_CENTER
    Type.LayoutY = ColorText.LAYOUT_CENTER
    x = 400
    y = 300
    z = -50
  }
}


