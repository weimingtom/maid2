#ifndef tool_dressing_bindobject_colortext_h
#define tool_dressing_bindobject_colortext_h


#include"cppdrawtype.h"


class ColorText
  : public CppDrawType
{
public:
  static void Register();
  float ColorR;
  float ColorG;
  float ColorB;
  float Alpha;

  int LayoutX;  //  テキストの配置位置
  int LayoutY;  //  テキストの配置位置

  Maid::String Text;

  Sqrat::string GetText() const;
  void SetText(const Sqrat::string& text);

  Sqrat::string GetFontName() const;
  void SetFontName(const Sqrat::string& name);

  int GetFontW() const;
  void SetFontW(const int& w);

  int GetFontH() const;
  void SetFontH(const int& h);

  Maid::Font  hFont;

  ColorText();
  virtual void Draw( float time, const Maid::POINT3DF& pos );
};


#endif
