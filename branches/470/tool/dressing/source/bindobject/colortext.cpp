#include"stdafx.h"
#include"colortext.h"

#include"../app.h"

using namespace Maid;
using namespace Sqrat;

static const int LAYOUT_FIRST   = 0;
static const int LAYOUT_CENTER = 1;
static const int LAYOUT_LAST  = 2;

void ColorText::Register()
{
  RootTable().Bind(
    _SC("ColorText"), 
    DerivedClass<ColorText,CppDrawType>()
	  .Var(_SC("ColorR" ), &ColorText::ColorR)
	  .Var(_SC("ColorG" ), &ColorText::ColorG)
	  .Var(_SC("ColorB" ), &ColorText::ColorB)
	  .Var(_SC("Alpha" ), &ColorText::Alpha)
	  .Var(_SC("LayoutX" ), &ColorText::LayoutX)
	  .Var(_SC("LayoutY" ), &ColorText::LayoutY)
    .Prop(_SC("Text"), &ColorText::GetText, &ColorText::SetText)
    .Prop(_SC("FontName"), &ColorText::GetFontName, &ColorText::SetFontName)
    .Prop(_SC("FontW"), &ColorText::GetFontW, &ColorText::SetFontW)
    .Prop(_SC("FontH"), &ColorText::GetFontH, &ColorText::SetFontH)

    .SetStaticValue( _SC("LAYOUT_FIRST"), LAYOUT_FIRST )
    .SetStaticValue( _SC("LAYOUT_CENTER"), LAYOUT_CENTER )
    .SetStaticValue( _SC("LAYOUT_LAST"), LAYOUT_LAST )
  );
}

ColorText::ColorText()
  :ColorR(1)
  ,ColorG(1)
  ,ColorB(1)
  ,Alpha(1)
  ,LayoutX(0)
  ,LayoutY(0)
{
  hFont.Create( SIZE2DI(8,16), true );
}

Sqrat::string ColorText::GetText() const
{
  const Sqrat::string ret = String::ConvertMAIDtoUNICODE(Text);

  return ret;
}

void ColorText::SetText(const Sqrat::string& text)
{
  const String str = String::ConvertUNICODEtoMAID(text);
  Text = str;
}

Sqrat::string ColorText::GetFontName() const
{
  const String str = hFont.GetName();
  const Sqrat::string ret = String::ConvertMAIDtoUNICODE(str);

  return ret;

}
void ColorText::SetFontName(const Sqrat::string& name)
{
  const int w = GetFontW();
  const int h = GetFontH();
  const String str = String::ConvertUNICODEtoMAID(name);

  hFont.Create( str, SIZE2DI(w,h), true, COLOR_R32G32B32A32F(1,1,1,1), Font::FILTER_NONE );
}

int ColorText::GetFontW() const
{
  return hFont.GetSize().w;
}

void ColorText::SetFontW(const int& w)
{
  const int h = GetFontH();
  const String str = hFont.GetName();

  hFont.Create( str, SIZE2DI(w,h), true, COLOR_R32G32B32A32F(1,1,1,1), Font::FILTER_NONE );
}

int ColorText::GetFontH() const
{
  return hFont.GetSize().h;
}

void ColorText::SetFontH(const int& h)
{
  const int w = GetFontW();
  const String str = hFont.GetName();

  hFont.Create( str, SIZE2DI(w,h), true, COLOR_R32G32B32A32F(1,1,1,1), Font::FILTER_NONE );
}

void ColorText::Draw( float time, const Maid::POINT3DF& pos )
{
  const POINT2DI point( (int)pos.x, (int)pos.y );
  const COLOR_R32G32B32A32F pow( ColorR, ColorG, ColorB, Alpha );

  const SIZE2DI size = hFont.CalcTextSize( Text );

  int offsetx = 0;
  {
    switch( LayoutX )
    {
    case LAYOUT_FIRST:   { offsetx = 0;}break;
    case LAYOUT_CENTER: { offsetx = size.w / 2;}break;
    case LAYOUT_LAST:  { offsetx = size.w;    }break;
    default: { MAID_ASSERT( true, "レイアウトの値が適切ではありません " << LayoutX ); }break;
    }
  }

  int offsety = 0;
  {
    switch( LayoutY )
    {
    case LAYOUT_FIRST:   { offsety = 0;}break;
    case LAYOUT_CENTER: { offsety = size.h / 2;}break;
    case LAYOUT_LAST:  { offsety = size.h;    }break;
    default: { MAID_ASSERT( true, "レイアウトの値が適切ではありません " << LayoutY ); }break;
    }
  }

  GetApp().Get2DRender().BltText( point-VECTOR2DI(offsetx,offsety), hFont, Text, pow );
}


