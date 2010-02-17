#ifndef tool_dressing_bindobject_imagerect_h
#define tool_dressing_bindobject_imagerect_h


#include"cppdrawtype.h"


class ImageRect 
  : public CppDrawType
{
public:
  static void Register();
  int CenterX;
  int CenterY;
  int RectX;
  int RectY;
  int RectW;
  int RectH;
  float Alpha;

  Maid::Texture2D Texture;

  Sqrat::string GetFileName() const;
  void SetFileName(const Sqrat::string& name);

  ImageRect();
  virtual void Draw( float time, const Maid::POINT3DF& pos );
};


#endif
