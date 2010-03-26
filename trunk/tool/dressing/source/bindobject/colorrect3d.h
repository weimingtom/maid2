#ifndef tool_dressing_bindobject_colorrect3d_h
#define tool_dressing_bindobject_colorrect3d_h


#include"cppdrawtype.h"


class ColorRect3D 
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

  ColorRect3D();
  virtual void Draw( float time, const Maid::POINT3DF& pos );
};


#endif
