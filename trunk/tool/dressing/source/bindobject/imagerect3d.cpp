#include"stdafx.h"
#include"imagerect3d.h"

#include"../app.h"

using namespace Maid;
using namespace Sqrat;


ImageRect3D::ImageRect3D()
  :CenterX(0)
  ,CenterY(0)
  ,RectX(0)
  ,RectY(0)
  ,RectW(0)
  ,RectH(0)
  ,Alpha(1)
{
}

void ImageRect3D::Register()
{
  RootTable().Bind(
    _SC("ImageRect3D"), 
    DerivedClass<ImageRect3D,CppDrawType>()
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


void ImageRect3D::Draw( float time, const Maid::POINT3DF& pos )
{
  if( Texture.IsEmpty() ) { return ; }
  if( Texture.IsLoading() ) { return ; }

  const POINT2DI center(CenterX,CenterY);
  const RECT2DI  rc(RectX,RectY,RectW,RectH);

  GetApp().Get3DRender().SpriteBlt( pos, Texture, rc, center, Alpha );

//    void SpriteBlt  ( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha );

}
