#include"stdafx.h"
#include"imagerect3d.h"

#include"../app.h"

using namespace Maid;
using namespace Sqrat;




void ImageRect3D::Register()
{
  RootTable().Bind(
    _SC("ImageRect3D"), 
    DerivedClassEx<ImageRect3D,CppDrawType,ImageRect3DAllocator>()
	  .SqObject(_SC("Matrix" ), &ImageRect3D::Matrix)
	  .Var(_SC("CenterX"), &ImageRect3D::CenterX)
	  .Var(_SC("CenterY"), &ImageRect3D::CenterY)
	  .Var(_SC("RectX"  ), &ImageRect3D::RectX)
	  .Var(_SC("RectY"  ), &ImageRect3D::RectY)
	  .Var(_SC("RectW"  ), &ImageRect3D::RectW)
	  .Var(_SC("RectH"  ), &ImageRect3D::RectH)
	  .Var(_SC("Alpha"  ), &ImageRect3D::Alpha)
    .Prop(_SC("FileName"), &ImageRect3D::GetFileName, &ImageRect3D::SetFileName)
  );
}

ImageRect3D::ImageRect3D()
  :CenterX(0)
  ,CenterY(0)
  ,RectX(0)
  ,RectY(0)
  ,RectW(0)
  ,RectH(0)
  ,Alpha(1)
{
  HSQUIRRELVM v = DefaultVM::Get();
  MAID_ASSERT( v==NULL, "VMが設定されていません" );
  sq_resetobject( &Matrix );
}

ImageRect3D::~ImageRect3D()
{
  HSQUIRRELVM v = DefaultVM::Get();
  MAID_ASSERT( v==NULL, "VMが設定されていません" );
  const SQBool drawtype = sq_release( v, &Matrix );
}

Sqrat::string ImageRect3D::GetFileName() const
{
  const String str = Texture.GetLoadText();

  const Sqrat::string ret = String::ConvertMAIDtoUNICODE(str);

  return ret;
}

void ImageRect3D::SetFileName(const Sqrat::string& name)
{
  const String str = String::ConvertUNICODEtoMAID(name);
  const String dir = GetApp().GetCurrentDirectry();

  Texture.LoadFile( dir + MAIDTEXT("\\") + str );
}

static const MATRIX4DF idx( MATRIX4DF().SetIdentity() );

void ImageRect3D::Draw( float time, const Maid::POINT3DF& pos )
{
  if( Texture.IsEmpty() ) { return ; }
  if( Texture.IsLoading() ) { return ; }

  const POINT3DF center(CenterX,CenterY,0);
  const RECT2DI  rc(RectX,RectY,RectW,RectH);
  const COLOR_R32G32B32A32F col(1,1,1,Alpha);

  const CppMatrix* p  = (const CppMatrix*)sq_objtoinstance(&Matrix);
  const MATRIX4DF& mat = p==NULL? idx : p->GetMatrix();
  const MATRIX4DF t = mat * MATRIX4DF().SetTranslate( pos.x, pos.y, pos.z );

  GetApp().Get3DRender().SpriteBlt( t, Texture, col, rc, center );

}
