﻿#ifndef maid2_graphics_graphics3drenderlocal_h
#define maid2_graphics_graphics3drenderlocal_h

#include"../config/define.h"
#include"../auxiliary/mathematics.h"
#include"color.h"

struct CONSTANT0100
{
  Maid::MATRIX4DF mWVP;
  Maid::COLOR_R32G32B32A32F Color;
};


struct CONSTANTSPRITE
{
  Maid::MATRIX4DF mWVP;
};

#endif
