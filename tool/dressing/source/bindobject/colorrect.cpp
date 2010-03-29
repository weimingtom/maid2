#include"stdafx.h"
#include"colorrect.h"

#include"../app.h"

using namespace Maid;
using namespace Sqrat;

void ColorRect::Register()
{
  RootTable().Bind(
    _SC("ColorRect"), 
    DerivedClassEx<ColorRect,CppDrawType, ColorRectAllocator>()
	  .SqObject(_SC("Matrix" ), &ColorRect::Matrix)
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
  HSQUIRRELVM v = DefaultVM::Get();
  MAID_ASSERT( v==NULL, "VMが設定されていません" );
  sq_resetobject( &Matrix );
}

ColorRect::~ColorRect()
{
  HSQUIRRELVM v = DefaultVM::Get();
  MAID_ASSERT( v==NULL, "VMが設定されていません" );
  const SQBool drawtype = sq_release( v, &Matrix );
}


static const MATRIX4DF idx( MATRIX4DF().SetIdentity() );


void ColorRect::Draw( float time, const Maid::POINT3DF& pos )
{
  const POINT2DI point( pos.x, pos.y );
  const POINT2DI center(CenterX,CenterY);
  const SIZE2DI size(Width,Height);
  const COLOR_R32G32B32A32F col(ColorR,ColorG,ColorB,Alpha);

  const CppMatrix* p  = (const CppMatrix*)sq_objtoinstance(&Matrix);
  const MATRIX4DF& mat = p==NULL? idx : p->GetMatrix();

  GetApp().Get2DRender().FillMatrix( point, col, size, center, mat );
}
