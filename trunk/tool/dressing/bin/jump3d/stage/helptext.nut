//  ヘルプオブジェクト

class HelpText extends DrawObject
{
  constructor()
  {
    DrawObject.constructor();
    
    Type = ColorText();
    Type.ColorR = 0.0
    Type.ColorG = 0.0
    Type.ColorB = 0.0
    Type.Alpha  = 1.0
  }
  
  function SetPlayText()
  {
    Type.Text = "←、→で移動。Zでジャンプ。それだけ。"
  }
  
  function SetGameOverText()
  {
    Type.Text = "Enterでタイトルに戻る"
  }
}


