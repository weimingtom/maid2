
class VECTOR3D
{
  x = 0
  y = 0
  z = 0

  constructor( a, b, c )
  {
    x = a
    y = b
    z = c
  }

  function Normalize()
  {
    local f = sqrt(x*x + y*y + z*z)
    x /= f
    y /= f
    z /= f
  }

}

