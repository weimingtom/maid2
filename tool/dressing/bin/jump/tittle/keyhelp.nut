class KeyHelp extends DrawObject
{
  constructor()
  {
    DrawObject.constructor();
    
    Type = ColorText();
    Type.Text = "EnterÇ≈êiÇﬁ"
    Type.ColorR = 1.0
    Type.ColorG = 1.0
    Type.ColorB = 1.0
    Type.Alpha  = 1.0
    Type.FontW = 8
    Type.FontH = 16

    Type.LayoutX = ColorText.LAYOUT_FIRST
    Type.LayoutY = ColorText.LAYOUT_FIRST
    x = 0
    y = 0
    z = -50

  }

}


