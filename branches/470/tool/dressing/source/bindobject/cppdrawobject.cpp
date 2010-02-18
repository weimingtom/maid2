#include"stdafx.h"

#include"cppdrawobject.h"


using namespace Maid;
using namespace Sqrat;

void CppDrawObject::Register()
{
  RootTable().Bind(
    _SC("CppDrawObject"), 
    ClassEx<CppDrawObject,CppDrawObjectAllocator>()
	  .SqObject(_SC("Type"), &CppDrawObject::Type)
	  .SqObject(_SC("Parent"), &CppDrawObject::Parent)
	  .Var(_SC("x"), &CppDrawObject::x)
	  .Var(_SC("y"), &CppDrawObject::y)
	  .Var(_SC("z"), &CppDrawObject::z)
  );
}



CppDrawObject::CppDrawObject()
  :x(0.0f)
  ,y(0.0f)
  ,z(0.0f)
  ,time(0.0f)
{
  HSQUIRRELVM v = DefaultVM::Get();
  MAID_ASSERT( v==NULL, "VMが設定されていません" );
  sq_resetobject( &Type );
  sq_resetobject( &Parent );
}

CppDrawObject::~CppDrawObject()
{
  HSQUIRRELVM v = DefaultVM::Get();
  MAID_ASSERT( v==NULL, "VMが設定されていません" );
  const SQBool drawtype = sq_release( v, &Type );
  const SQBool parent = sq_release( v, &Parent );
}

const CppDrawObject* CppDrawObject::GetParent() const
{
  HSQOBJECT& obj = const_cast<CppDrawObject*>(this)->Parent;

  const CppDrawObject* p  = (const CppDrawObject*)sq_objtoinstance(&obj);

  return p;
}

CppDrawType*  CppDrawObject::GetType()
{
  HSQOBJECT& obj = const_cast<CppDrawObject*>(this)->Type;

  CppDrawType* p  = (CppDrawType*)sq_objtoinstance(&obj);

  return p;
}

POINT3DF CppDrawObject::CalcGlobalPos() const
{
  const CppDrawObject* p = GetParent();

  if( p==NULL ) { return POINT3DF(x,y,z); }

  POINT3DF tmp = p->CalcGlobalPos();
  tmp.x += x;
  tmp.y += y;
  tmp.z += z;

  return tmp;
}
