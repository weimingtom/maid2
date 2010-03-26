/*
    ３Ｄ表示に使うカメラクラス
    １シーン単位で持っています
*/
class Camera
{
  Eye = null
  Target = null
  Up = null

  Fov = 0
  Aspect = 0
  Near = 0
  Far = 0
  
  ParallaxEye    = 0.0  // 視差の長さ　立体視用
  ParallaxTarget = 0.0  // 視差の長さ　立体視用

  constructor()
  {
    // デフォルトは２Ｄっぽいモード
    Set2DPosition( 800, 600, 1.0, 1000.0 )
    
    ParallaxEye = 5.0
    ParallaxTarget = ParallaxEye // とりあえず平行法で
  }
  
  //! ３Ｄなんだけど、２Ｄっぽい表示をするための設定にする
  /*!
      @param  width  [i ]  スクリーンの横幅(int)
      @param  height [i ]  スクリーンの縦幅(int)
      @param  near   [i ]  表示する画面手前の距離(float)
      @param  far    [i ]  表示する画面奥の距離(float)
  */  
  function Set2DPosition( width, height, near, far )
  {
    local fov= DEGtoRAD(60.0)
    local hw = width  / 2
    local hh = height / 2
    local dep= hh.tofloat() / tan(fov/2.0)

    Eye = POINT3D(hw.tofloat(), hh.tofloat(), -dep )
    Target = POINT3D(hw.tofloat(), hh.tofloat(), 0 )
    Up = VECTOR3D(0,1,0)

    Fov = fov
    Aspect = width.tofloat() / height.tofloat()
    Near = near
    Far  = far    
  }
}

