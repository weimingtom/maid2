function abs( val )
{
  if( val<0 ) { return -val }
  return val
}

function max( a, b )
{
  if( a<b ) { return b }
  return a
}

function min( a, b )
{
  if( a<b ) { return a }
  return b
}

PI <- 3.141592654

function DEGtoRAD( deg )
{
  return deg * (PI/180.0)
}

function RADtoDEG( rad )
{
  return rad * (180.0/PI)
}
