#include"stdafx.h"
#include"colorrect.h"

#include"../app.h"

using namespace Maid;
using namespace Sqrat;

void ColorRect::Register()
{
  RootTable().Bind(
    _SC("ColorRect"), 
    DerivedClass<ColorRect,CppDrawType>()
	  .Var(_SC("CenterX"), &ColorRect::CenterX)
	  .Var(_SC("CenterY"), &ColorRect::CenterY)
	  .Var(_SC("Width"  ), &ColorRect::Width)
	  .Var(_SC("Height" ), &ColorRect::Height)
	  .Var(_SC("ColorR" ), &ColorRect::ColorR)
	  .Var(_SC("ColorG" ), &ColorRect::ColorG)
	  .Var(_SC("ColorB" ), &ColorRect::ColorB)
	  .Var(_SC("Alpha" ), &ColorRect::Alpha)
  );
}

ColorRect::ColorRect()
  :CenterX(0)
  ,CenterY(0)
  ,Width(0)
  ,Height(0)
  ,ColorR(0)
  ,ColorG(0)
  ,ColorB(0)
  ,Alpha(1)
{
}



void ColorRect::Draw( float time, const Maid::POINT3DF& pos )
{
  const POINT2DI point( pos.x, pos.y );
  const POINT2DI center(CenterX,CenterY);
  const SIZE2DI size(Width,Height);
  const COLOR_R32G32B32A32F col(ColorR,ColorG,ColorB,Alpha);

  GetApp().Get2DRender().Fill( point, col, size, center );
}
