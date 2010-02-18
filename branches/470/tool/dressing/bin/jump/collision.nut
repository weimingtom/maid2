
class POINT
{
  x=0
  y=0

  constructor( a, b )
  {
    x = a
    y = b
  }
}

class RECT
{
  x = 0
  y = 0
  w = 0
  h = 0

  constructor( a, b, c, d )
  {
    x = a
    y = b
    w = c
    h = d
  }

  function _get(key)
  {
    switch( key )
    {
    case "left"  : { return x   }break;
    case "top"   : { return y   }break;
    case "right" : { return x+w }break;
    case "bottom": { return y+h }break;
    }
  }
}


class COLLISION
{
  function PointRect( p, r )
  {
    if( p.x < r.x           ) { return false }
    if(       r.x+r.w < p.x ) { return false }
    if( p.y < r.y           ) { return false }
    if(       r.y+r.h < p.y ) { return false }
  }

  function RectRect( r0, r1 )
  {
    if ( r0.x      >= r1.x+r1.w ) { return false; }
    if ( r0.x+r0.w <= r1.x      ) { return false; }
    if ( r0.y      >= r1.y+r1.h ) { return false; }
    if ( r0.y+r0.h <= r1.y      ) { return false; }

    return true
  }


}


return ;


{
  local r0 = RECT(100,100,50,50)
  local r1 = RECT(120,120,40,40)

  assert( COLLISION.RectRect( r0, r1  )==true )
  assert( COLLISION.RectRect( r1, r0  )==true )
}

{
  local r0 = RECT(100,100,50,50)
  local r1 = RECT(120,120,10,10)

  assert( COLLISION.RectRect( r0, r1  )==true )
  assert( COLLISION.RectRect( r1, r0  )==true )
}

{
  local r0 = RECT(100,100,50,50)
  local r1 = RECT(150,150,10,10)

  assert( COLLISION.RectRect( r0, r1  )==false )
  assert( COLLISION.RectRect( r1, r0  )==false )
}

print( "test ok!" )