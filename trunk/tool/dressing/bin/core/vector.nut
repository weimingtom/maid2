function VECTOR4DxMATRIX4D( a, b )
{
  local x = a.x*b.m11 + a.y*b.m21 + a.z*b.m31 + a.w*b.m41;
  local y = a.x*b.m12 + a.y*b.m22 + a.z*b.m32 + a.w*b.m42;
  local z = a.x*b.m13 + a.y*b.m23 + a.z*b.m33 + a.w*b.m43;
  local w = a.x*b.m14 + a.y*b.m24 + a.z*b.m34 + a.w*b.m44;

  return VECTOR4D( x, y, z, w )
}

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

  function _typeof() { return "VECTOR3D"; }

  function _mul(o)
  {
    local ret = VECTOR3D(
     x * o,
     y * o,
     z * o
    )

    return ret
  }

  function Normalize()
  {
    local f = sqrt(x*x + y*y + z*z)
    x /= f
    y /= f
    z /= f
    return this
  }
}




class VECTOR4D
{
  x = 0
  y = 0
  z = 0
  w = 1

  constructor( a, b, c, d )
  {
    x = a
    y = b
    z = c
    w = d
  }

  function _typeof() { return "VECTOR4D"; }

  function _mul(o)
  {
    if( typeof o == "MATRIX4D" ) { return VECTOR3DxMATRIX4D( this, o ); }
    
    local ret = VECTOR4D(
      x * o,
      y * o,
      z * o,
      w * o
      )
    return ret
  }

  function Normalize()
  {
    local f = sqrt(x*x + y*y + z*z + w*w)
    x /= f
    y /= f
    z /= f
    w /= f
    return this
  }
  
  function ConvertVECTOR3D()
  {
    return VECTOR3D( x/w, x/y, z/w )
  }
}

