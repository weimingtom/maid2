
class Camera
{
  Eye = null
  Target = null
  Up = null

  Fov = 0
  Aspect = 0
  Near = 0
  Far = 0

  constructor()
  {
    Eye = POINT3D(10,0,0)
    Target = POINT3D(0,0,0)

    Up = VECTOR3D(0,1,0)

    Fov = 60.0
    Aspect = 600.0 / 800.0
    Near = 1
    Far  = 100
  }
}

