#include"stdafx.h"
#include"imagerect.h"

#include"../app.h"

using namespace Maid;
using namespace Sqrat;


ImageRect::ImageRect()
  :CenterX(0)
  ,CenterY(0)
  ,RectX(0)
  ,RectY(0)
  ,RectW(0)
  ,RectH(0)
  ,Alpha(1)
{
}

void ImageRect::Register()
{
  RootTable().Bind(
    _SC("ImageRect"), 
    DerivedClass<ImageRect,CppDrawType>()
	  .Var(_SC("CenterX"), &ImageRect::CenterX)
	  .Var(_SC("CenterY"), &ImageRect::CenterY)
	  .Var(_SC("RectX"  ), &ImageRect::RectX)
	  .Var(_SC("RectY"  ), &ImageRect::RectY)
	  .Var(_SC("RectW"  ), &ImageRect::RectW)
	  .Var(_SC("RectH"  ), &ImageRect::RectH)
	  .Var(_SC("Alpha"  ), &ImageRect::Alpha)
    .Prop(_SC("FileName"), &ImageRect::GetFileName, &ImageRect::SetFileName)
  );
}


Sqrat::string ImageRect::GetFileName() const
{
  const String str = Texture.GetLoadText();

  const Sqrat::string ret = String::ConvertMAIDtoUNICODE(str);

  return ret;
}

void ImageRect::SetFileName(const Sqrat::string& name)
{
  const String str = String::ConvertUNICODEtoMAID(name);
  const String dir = GetApp().GetCurrentDirectry();

  Texture.LoadFile( dir + MAIDTEXT("\\") + str );
}


void ImageRect::Draw( float time, const Maid::POINT3DF& pos )
{
  if( Texture.IsEmpty() ) { return ; }
  if( Texture.IsLoading() ) { return ; }

  const POINT2DI point( pos.x, pos.y );
  const POINT2DI center(CenterX,CenterY);
  const RECT2DI rc(RectX,RectY,RectW,RectH);

  GetApp().Get2DRender().Blt( point, Texture, rc, center, Alpha );

}
