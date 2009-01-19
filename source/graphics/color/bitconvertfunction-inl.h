namespace Maid
{
  //!  int 01bit -> int 08bit
  inline unt08 BITCONVERT01Ito08I( unt08 s ){ return (~s)+1;}

  //!  int 02bit -> int 08bit
  inline unt08 BITCONVERT02Ito08I( unt08 s ){ return (s<<6)|(s<<4)|(s<<2)|s;}

  //!  int 04bit -> int 08bit
  inline unt08 BITCONVERT04Ito08I( unt08 s ){ return (s<<4)|s;	}

  //!  int 05bit -> int 08bit
  inline unt08 BITCONVERT05Ito08I( unt08 s ){ return (s<<3)|(s>>2); }

  //!  int 06bit -> int 08bit
  inline unt08 BITCONVERT06Ito08I( unt08 s ){ return (s<<2)|(s>>4); }



  //!  int 01bit -> int 16bit
  inline unt16 BITCONVERT01Ito16I( unt08 s ) { return BITCONVERT08Ito16I(BITCONVERT01Ito08I(s)); }

  //!  int 02bit -> int 16bit
  inline unt16 BITCONVERT02Ito16I( unt08 s ) { return BITCONVERT08Ito16I(BITCONVERT02Ito08I(s)); }

  //!  int 04bit -> int 16bit
  inline unt16 BITCONVERT04Ito16I( unt08 s ) { return BITCONVERT08Ito16I(BITCONVERT04Ito08I(s)); }

  //!  int 05bit -> int 16bit
  inline unt16 BITCONVERT05Ito16I( unt08 s ) { return BITCONVERT08Ito16I(BITCONVERT05Ito08I(s)); }

  //!  int 06bit -> int 16bit
  inline unt16 BITCONVERT06Ito16I( unt08 s ) { return BITCONVERT08Ito16I(BITCONVERT06Ito08I(s)); }

  //!  int 08bit -> int 16bit
  inline unt16 BITCONVERT08Ito16I( unt08 s ) { return (unt16(s)<<8)|(s); }

  //!  int 10bit -> int 16bit
  inline unt16 BITCONVERT10Ito16I( unt16 s ) { return (s<<6)|(s>>4); }


  // 8bit–O˜a‰‰ŽZ
  inline unt08 _CLIP08I( unt08 s, unt08 a )
  {
    if( s+a>0xFF ) { return 0xFF; }
    return s+a;
  }


  //!  int 08bit -> int 01bit
	inline unt08 BITCONVERT08Ito01I( unt08 s ){ return _CLIP08I(s,0x3F)>>7; }

  //!  int 08bit -> int 02bit
	inline unt08 BITCONVERT08Ito02I( unt08 s ){ return _CLIP08I(s,0x1F)>>6; }

  //!  int 08bit -> int 04bit
	inline unt08 BITCONVERT08Ito04I( unt08 s ){ return _CLIP08I(s,0x07)>>4; }

  //!  int 08bit -> int 05bit
	inline unt08 BITCONVERT08Ito05I( unt08 s ){ return _CLIP08I(s,0x03)>>3; }

  //!  int 08bit -> int 06bit
	inline unt08 BITCONVERT08Ito06I( unt08 s ){ return _CLIP08I(s,0x01)>>2; }



  // 16bit–O˜a‰‰ŽZ
  inline unt16 _CLIP16I( unt16 s, unt16 a )
  {
    if( s+a>0xFFFF ) { return 0xFFFF; }
    return s+a;
  }

  //!  int 16bit -> int 01bit
  inline unt08 BITCONVERT16Ito01I( unt16 s ) { return (unt08)(_CLIP16I(s,0x3FFF)>>15); }

  //!  int 16bit -> int 02bit
  inline unt08 BITCONVERT16Ito02I( unt16 s ) { return (unt08)(_CLIP16I(s,0x1FFF)>>14); }

  //!  int 16bit -> int 04bit
  inline unt08 BITCONVERT16Ito04I( unt16 s ) { return (unt08)(_CLIP16I(s,0x07FF)>>12); }

  //!  int 16bit -> int 05bit
  inline unt08 BITCONVERT16Ito05I( unt16 s ) { return (unt08)(_CLIP16I(s,0x03FF)>>11); }

  //!  int 16bit -> int 06bit
  inline unt08 BITCONVERT16Ito06I( unt16 s ) { return (unt08)(_CLIP16I(s,0x01FF)>>10); }

  //!  int 16bit -> int 08bit
  inline unt08 BITCONVERT16Ito08I( unt16 s ) { return (unt08)(_CLIP16I(s,0x00FF)>> 8); }

  //!  int 16bit -> int 10bit
  inline unt16 BITCONVERT16Ito10I( unt16 s ) { return        (_CLIP16I(s,0x003F)>> 6); }


  //!  float 32bit -> int 08bit
  inline unt08 BITCONVERT32Fto08I( float32 s )
  {
    return BITCONVERT16Ito08I(BITCONVERT32Fto16I(s)); 
  }

  //!  float 32bit -> int 16bit
  inline unt16 BITCONVERT32Fto16I( float32 s )
  {
    if( s>1.0f ) { return 0xFFFF; }
    if( s<0.0f ) { return 0x0000; }

    const int no = int(s*65535.0f+0.5f);
    return (unt16)no;
  }

  //!  int 08bit -> float 32bit
  inline float32 BITCONVERT08Ito32F( unt08 s )
  {
    return BITCONVERT16Ito32F(BITCONVERT08Ito16I(s)); 
  }


  //!  int 16bit -> float 32bit
  inline float32 BITCONVERT16Ito32F( unt16 s )
  {
    return float32(s) / 65535.0f;
  }
}

