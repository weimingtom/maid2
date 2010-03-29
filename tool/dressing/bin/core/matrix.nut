function MATRIX4DxMATRIX4D( a, b )
{
  local r = MATRIX4D()

  r.m11 = a.m11*b.m11 + a.m12*b.m21 + a.m13*b.m31 + a.m14*b.m41;
  r.m21 = a.m21*b.m11 + a.m22*b.m21 + a.m23*b.m31 + a.m24*b.m41;
  r.m31 = a.m31*b.m11 + a.m32*b.m21 + a.m33*b.m31 + a.m34*b.m41;
  r.m41 = a.m41*b.m11 + a.m42*b.m21 + a.m43*b.m31 + a.m44*b.m41;

  r.m12 = a.m11*b.m12 + a.m12*b.m22 + a.m13*b.m32 + a.m14*b.m42;
  r.m22 = a.m21*b.m12 + a.m22*b.m22 + a.m23*b.m32 + a.m24*b.m42;
  r.m32 = a.m31*b.m12 + a.m32*b.m22 + a.m33*b.m32 + a.m34*b.m42;
  r.m42 = a.m41*b.m12 + a.m42*b.m22 + a.m43*b.m32 + a.m44*b.m42;

  r.m13 = a.m11*b.m13 + a.m12*b.m23 + a.m13*b.m33 + a.m14*b.m43;
  r.m23 = a.m21*b.m13 + a.m22*b.m23 + a.m23*b.m33 + a.m24*b.m43;
  r.m33 = a.m31*b.m13 + a.m32*b.m23 + a.m33*b.m33 + a.m34*b.m43;
  r.m43 = a.m41*b.m13 + a.m42*b.m23 + a.m43*b.m33 + a.m44*b.m43;

  r.m14 = a.m11*b.m14 + a.m12*b.m24 + a.m13*b.m34 + a.m14*b.m44;
  r.m24 = a.m21*b.m14 + a.m22*b.m24 + a.m23*b.m34 + a.m24*b.m44;
  r.m34 = a.m31*b.m14 + a.m32*b.m24 + a.m33*b.m34 + a.m34*b.m44;
  r.m44 = a.m41*b.m14 + a.m42*b.m24 + a.m43*b.m34 + a.m44*b.m44;

  return r;
}





class MATRIX4D extends CppMatrix
{
  function _typeof() { return "MATRIX4D"; }

  function _mul(o)
  {
    return MATRIX4DxMATRIX4D( this, o );
  }
  
  function Set(
    s11, s12, s13, s14,
    s21, s22, s23, s24,
    s31, s32, s33, s34,
    s41, s42, s43, s44
    )
  {
    m11 = s11;  m12 = s12;  m13 = s13;  m14 = s14;
    m21 = s21;  m22 = s22;  m23 = s23;  m24 = s24;
    m31 = s31;  m32 = s32;  m33 = s33;  m34 = s34;
    m41 = s41;  m42 = s42;  m43 = s43;  m44 = s44;
  }

  function SetIdentity()
  {
    Set( 
      1,0,0,0,
      0,1,0,0,
      0,0,1,0,
      0,0,0,1
    )
    return this
  }

  function SetScale( x, y, z )
  {
    Set( 
      x,0,0,0,
      0,y,0,0,
      0,0,z,0,
      0,0,0,1
    )
    return this
  }

  function SetTranslate( x, y, z )
  {
    Set( 
      1,0,0,0,
      0,1,0,0,
      0,0,1,0,
      x,y,z,1
    )
    return this
  }

  function SetRotationX( rad )
  {
    local s = sin(rad)
    local c = cos(rad)
    Set( 
      1, 0, 0, 0,
      0, c, s, 0,
      0,-s, c, 0,
      0, 0, 0, 1
    )
    return this
  }

  function SetRotationY( rad )
  {
    local s = sin(rad)
    local c = cos(rad)
    Set( 
      c, 0, -s, 0,
      0, 1, 0, 0,
      s, 0, c, 0,
      0, 0, 0, 1
    )
    return this
  }

  function SetRotationZ( rad )
  {
    local s = sin(rad)
    local c = cos(rad)
    Set( 
      c, s, 0, 0,
     -s, c, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
    )
    return this
  }

  //! 任意軸回転行列をセットする
  /*!
      @param	rad	[i ]	回転量（ラジアン）
      @param	vec	[i ]	基準にするベクトル（正規化しておくこと）
   */
  function SetRotationXYZ( rad, vec )
  {
    local s = sin(rad)
    local c = cos(rad)
    local a = 1.0-c
    local x = vec.x
    local y = vec.y
    local z = vec.z
    
    Set( 
      a*x*x+  c, a*x*y-z*s, a*x*z+y*s, 0,
      a*x*y+z*s, a*y*y+  c, a*y*z-x*s, 0,
      a*x*z-y*s, a*y*z+x*s, a*z*z+  c, 0,
              0,         0,         0, 1
    )
    return this
  }
}


return

local a=MATRIX4D()
local b=MATRIX4D()

a.SetIdentity()
a.m12 = 100
b.m21 = 100
b.m22 = 100

local c = a*b;

print( c.m12 ) // 10000

