#include"stdafx.h"
#include"modelfile.h"

#include"../app.h"

using namespace Maid;
using namespace Sqrat;




void ModelFile::Register()
{
  RootTable().Bind(
    _SC("ModelFile"), 
    DerivedClassEx<ModelFile,CppDrawType,ModelFileAllocator>()
	  .SqObject(_SC("Matrix" ), &ModelFile::Matrix)
	  .Var(_SC("Alpha"  ), &ModelFile::Alpha)
    .Prop(_SC("FileName"), &ModelFile::GetFileName, &ModelFile::SetFileName)
  );
}

ModelFile::ModelFile()
  :Alpha(1)
{
  HSQUIRRELVM v = DefaultVM::Get();
  MAID_ASSERT( v==NULL, "VMが設定されていません" );
  sq_resetobject( &Matrix );
}

ModelFile::~ModelFile()
{
  HSQUIRRELVM v = DefaultVM::Get();
  MAID_ASSERT( v==NULL, "VMが設定されていません" );
  const SQBool drawtype = sq_release( v, &Matrix );
}

Sqrat::string ModelFile::GetFileName() const
{
  const String str = Model.GetFileName();

  const Sqrat::string ret = String::ConvertMAIDtoUNICODE(str);

  return ret;
}

void ModelFile::SetFileName(const Sqrat::string& name)
{
  const String str = String::ConvertUNICODEtoMAID(name);
  const String dir = GetApp().GetCurrentDirectry();

  Model.Load( dir + MAIDTEXT("\\") + str );
}

static const MATRIX4DF idx( MATRIX4DF().SetIdentity() );

void ModelFile::Draw( float time, const Maid::POINT3DF& pos )
{
  if( Model.IsEmpty() ) { return ; }
  if( Model.IsLoading() ) { return ; }

  const CppMatrix* p  = (const CppMatrix*)sq_objtoinstance(&Matrix);
  const MATRIX4DF& mat = p==NULL? idx : p->GetMatrix();
  const MATRIX4DF t = mat * MATRIX4DF().SetTranslate( pos.x, pos.y, pos.z );

  GetApp().Get3DRender().Blt( t, Model, Alpha );
}
