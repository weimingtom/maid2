/*
    ３Ｄ表示に使うライトクラス
    １シーン単位で持っています
*/
class DirectionalLight
{
  Color = null
  Direction = null
  
  constructor()
  {
    Color = COLOR(1,1,1,1)
    Direction = VECTOR3D(1,0,0)
  }
}

class BaseLight
{
  Direction = null
  Ambient   = null

  constructor()
  {
    Direction = DirectionalLight()
    Ambient   = COLOR(0,0,0,0)
  
  }

}