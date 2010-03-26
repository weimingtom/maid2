#include"stdafx.h"
#include"colorrect3d.h"

#include"../app.h"

using namespace Maid;
using namespace Sqrat;

void ColorRect3D::Register()
{
  RootTable().Bind(
    _SC("ColorRect3D"), 
    DerivedClass<ColorRect3D,CppDrawType>()
	  .Var(_SC("CenterX"), &ColorRect3D::CenterX)
	  .Var(_SC("CenterY"), &ColorRect3D::CenterY)
	  .Var(_SC("Width"  ), &ColorRect3D::Width)
	  .Var(_SC("Height" ), &ColorRect3D::Height)
	  .Var(_SC("ColorR" ), &ColorRect3D::ColorR)
	  .Var(_SC("ColorG" ), &ColorRect3D::ColorG)
	  .Var(_SC("ColorB" ), &ColorRect3D::ColorB)
	  .Var(_SC("Alpha" ), &ColorRect3D::Alpha)
  );
}

ColorRect3D::ColorRect3D()
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



void ColorRect3D::Draw( float time, const Maid::POINT3DF& pos )
{
  const POINT2DI center(CenterX,CenterY);
  const SIZE2DI size(Width,Height);
  const COLOR_R32G32B32A32F col(ColorR,ColorG,ColorB,Alpha);

  GetApp().Get3DRender().SpriteFill( pos, col, size, center );
}
