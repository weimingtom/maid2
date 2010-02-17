#ifndef tool_dressing_bindobject_colorrect_h
#define tool_dressing_bindobject_colorrect_h


#include"cppdrawtype.h"


class ColorRect 
  : public CppDrawType
{
public:
  static void Register();
  int CenterX;
  int CenterY;
  int Width;
  int Height;

  float ColorR;
  float ColorG;
  float ColorB;
  float Alpha;

  ColorRect();
  virtual void Draw( float time, const Maid::POINT3DF& pos );
};


#endif
