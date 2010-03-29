#include"stdafx.h"
#include"colorrect3d.h"

#include"../app.h"

using namespace Maid;
using namespace Sqrat;

void ColorRect3D::Register()
{
  RootTable().Bind(
    _SC("ColorRect3D"), 
    DerivedClassEx<ColorRect3D,CppDrawType, ColorRect3DAllocator>()
	  .SqObject(_SC("Matrix" ), &ColorRect3D::Matrix)
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
  HSQUIRRELVM v = DefaultVM::Get();
  MAID_ASSERT( v==NULL, "VMが設定されていません" );
  sq_resetobject( &Matrix );
}

ColorRect3D::~ColorRect3D()
{
  HSQUIRRELVM v = DefaultVM::Get();
  MAID_ASSERT( v==NULL, "VMが設定されていません" );
  const SQBool drawtype = sq_release( v, &Matrix );
}



static const MATRIX4DF idx( MATRIX4DF().SetIdentity() );

void ColorRect3D::Draw( float time, const Maid::POINT3DF& pos )
{
  const POINT3DF center(CenterX,CenterY,0);
  const SIZE2DI size(Width,Height);
  const COLOR_R32G32B32A32F col(ColorR,ColorG,ColorB,Alpha);

  const CppMatrix* p  = (const CppMatrix*)sq_objtoinstance(&Matrix);
  const MATRIX4DF& mat = p==NULL? idx : p->GetMatrix();
  const MATRIX4DF t = mat * MATRIX4DF().SetTranslate( pos.x, pos.y, pos.z );

  GetApp().Get3DRender().SpriteFill( t, col, size, center );
}
