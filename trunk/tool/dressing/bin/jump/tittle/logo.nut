class Logo extends DrawObject
{
  constructor()
  {
    DrawObject.constructor();
    
    Type = ColorText();
    Type.Text = "Ç‚ÇŒÇ©Ç¡ÇΩÇÁîÚÇ◊ÅI"
    Type.ColorR = 1.0
    Type.ColorG = 1.0
    Type.ColorB = 1.0
    Type.Alpha  = 1.0
    Type.FontW = 32
    Type.FontH = 64

    Type.LayoutX = ColorText.LAYOUT_CENTER
    Type.LayoutY = ColorText.LAYOUT_CENTER
    x = 400
    y = 300
    z = -50

  }

}


