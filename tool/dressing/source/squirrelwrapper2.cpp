#include"stdafx.h"
#include"squirrelwrapper.h"


#include"squirrelex.h"



using namespace Maid;
using namespace Sqrat;



static int RandI( int min, int max )
{
  return GlobalRand::GetI( min, max );
}

void SquirrelWrapper::SetupBindObject()
{
  {
    RootTable()
      .Func( _SC("RandI") , &RandI )
      ;

  }
  {
    CppDrawType::Register();
    ColorRect::Register();
    ImageRect::Register();
    ColorText::Register();
  }
  {
    CppDrawObject::Register();
  }

  {
    CppSoundType::Register();
    PcmFile::Register();
  }
  {
    CppSoundObject::Register();
  }
}

void SquirrelWrapper::ReleaseBindObject()
{

}


FUNCTIONRESULT SquirrelWrapper::GetDrawObjectList( SCENEINFOLIST& list )
{
  HSQUIRRELVM v = m_SquirrelVM;

  sq_pushroottable(v);
  sq_pushstring( v, L"admin", -1 );
  SQRESULT ret_admin = sq_get(v, -2);
  if( SQ_FAILED(ret_admin) ) { sq_pop( v, 1 ); return FUNCTIONRESULT_ERROR; }
  sq_pushstring( v, L"GetDrawObject", -1 );
  SQRESULT ret_GetGameObject = sq_get(v, -2);
  if( SQ_FAILED(ret_GetGameObject) ) { sq_pop( v, 2 ); return FUNCTIONRESULT_ERROR; }
  sq_pushstring( v, L"admin", -1 );
  SQRESULT ret_admin_ = sq_get(v, -4);
  if( SQ_FAILED(ret_admin_) ) { sq_pop( v, 3 ); return FUNCTIONRESULT_ERROR; }

  const SQRESULT ret_call = sq_call(v, 1,  SQTrue, IsRaiseError() );
  const SQObjectType resulttype = sq_gettype(v, -1);

  sq_pushnull(v);
  while( true )
  {
    const SQRESULT next_ret = sq_next(v,-2);
    if( SQ_FAILED(next_ret) ) { break; }

    const SQObjectType keytype = sq_gettype(v, -2);
    const SQObjectType valuetype = sq_gettype(v, -1);

    list.push_back(SCENEINFO());
    sq_pushnull(v);
    while( true )
    {
      const SQRESULT next_ret = sq_next(v,-2);
      if( SQ_FAILED(next_ret) ) { break; }

      const SQObjectType keytype = sq_gettype(v, -2);
      const SQObjectType valuetype = sq_gettype(v, -1);

      CppDrawObject* pObj = NULL;
      const SQRESULT ret_get = sq_getinstanceup( v, -1, (SQUserPointer*)&pObj, NULL );
      if( SQ_FAILED(ret_get) ) { continue; }

      list.back().ObjectList.push_back( pObj );

      sq_pop( v, 2 );
    }

    sq_pop( v, 2 );
  }
  sq_pop( v, 6 ); // nullとcall の戻り値と &GameObject, &admin, v

  return FUNCTIONRESULT_OK;
}
