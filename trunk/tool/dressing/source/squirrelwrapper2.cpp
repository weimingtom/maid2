#include"stdafx.h"
#include"squirrelwrapper.h"
#include"squirrelwrapperlocal.h"


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

FUNCTIONRESULT SquirrelWrapper::GetStorageData( XMLWriter& xml )
{
  HSQUIRRELVM v = m_SquirrelVM;

  sq_pushroottable(v);
  sq_pushstring( v, L"storage", -1 );
  SQRESULT ret_storage = sq_get(v, -2);
  if( SQ_FAILED(ret_storage) ) { sq_pop( v, 1 ); return FUNCTIONRESULT_ERROR; }
  sq_pushstring( v, L"Root", -1 );
  SQRESULT ret_Root = sq_get(v, -2);
  if( SQ_FAILED(ret_Root) ) { sq_pop( v, 2 ); return FUNCTIONRESULT_ERROR; }
  const SQObjectType resulttype = sq_gettype(v, -1);


  xml.AscendNode( MAIDTEXT("Root") );
  ReadStorageTable( v, xml );
  xml.DescendNode();

  sq_pop( v, 3 ); // &Root, &storage, v

  return FUNCTIONRESULT_OK;
}

FUNCTIONRESULT  SquirrelWrapper::ReadStorageArray( HSQUIRRELVM v, Maid::XMLWriter& xml )
{
  const String ARRAYKEY = MAIDTEXT("d");

  sq_pushnull(v);
  while( true )
  {
    const SQRESULT next_ret = sq_next(v,-2);
    if( SQ_FAILED(next_ret) ) { break; }

    const SQObjectType keytype = sq_gettype(v, -2);
    const SQObjectType valuetype = sq_gettype(v, -1);

    switch( valuetype )
    {
    case OT_INTEGER:
      {
        SQInteger i;
        sq_getinteger( v, -1, &i );

        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_INTEGER );
        xml.AddTag( ARRAYKEY, (int)i );
      }break;
    case OT_FLOAT:
      {
        SQFloat f;
        sq_getfloat( v, -1, &f );

        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_FLOAT );
        xml.AddTag( ARRAYKEY, (float)f );
      }break;

    case OT_STRING:
      {
        const SQChar* str;
        sq_getstring( v, -1, &str );

        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_STRING );
        xml.AddTag( ARRAYKEY, String::ConvertUNICODEtoMAID(str) );
      }break;

    case OT_BOOL:
      {
        SQBool b;
        sq_getbool( v, -1, &b );

        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_BOOL );

        if( b ) { xml.AddTag( ARRAYKEY, STORAGEVALUE_TRUE  ); }
        else    { xml.AddTag( ARRAYKEY, STORAGEVALUE_FALSE ); }
      }break;

    case OT_NULL:
      {
        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_NULL );
        xml.AddTag( ARRAYKEY, STORAGEVALUE_NULL  );
      }break;

    case OT_ARRAY:
      {
        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_ARRAY );
        xml.AscendNode( ARRAYKEY );
        ReadStorageArray(v, xml );
        xml.DescendNode();
      }break;

    case OT_TABLE:
      {
        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_TABLE );
        xml.AscendNode( ARRAYKEY );
        ReadStorageTable(v, xml );
        xml.DescendNode();
      }break;

    }
    sq_pop( v, 2 ); //  sq_next
  }
  sq_pop( v, 1 ); //  null
  return FUNCTIONRESULT_OK;
}

FUNCTIONRESULT  SquirrelWrapper::ReadStorageTable( HSQUIRRELVM v, Maid::XMLWriter& xml )
{
  sq_pushnull(v);
  while( true )
  {
    const SQRESULT next_ret = sq_next(v,-2);
    if( SQ_FAILED(next_ret) ) { break; }

    const SQObjectType keytype = sq_gettype(v, -2);
    const SQObjectType valuetype = sq_gettype(v, -1);

    String KeyName;
    {
      const SQChar* n;
      sq_getstring( v, -2, &n );
      KeyName = String::ConvertUNICODEtoMAID(n);
    }

    switch( valuetype )
    {
    case OT_INTEGER:
      {
        SQInteger i;
        sq_getinteger( v, -1, &i );

        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_INTEGER );
        xml.AddTag( KeyName, (int)i );
      }break;
    case OT_FLOAT:
      {
        SQFloat f;
        sq_getfloat( v, -1, &f );

        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_FLOAT );
        xml.AddTag( KeyName, (float)f );
      }break;

    case OT_STRING:
      {
        const SQChar* str;
        sq_getstring( v, -1, &str );

        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_STRING );
        xml.AddTag( KeyName, String::ConvertUNICODEtoMAID(str) );
      }break;

    case OT_BOOL:
      {
        SQBool b;
        sq_getbool( v, -1, &b );

        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_BOOL );

        if( b ) { xml.AddTag( KeyName, STORAGEVALUE_TRUE  ); }
        else    { xml.AddTag( KeyName, STORAGEVALUE_FALSE ); }
      }break;

    case OT_NULL:
      {
        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_NULL );
        xml.AddTag( KeyName, STORAGEVALUE_NULL  );
      }break;

    case OT_ARRAY:
      {
        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_ARRAY );
        xml.AscendNode( KeyName );
        ReadStorageArray(v, xml );
        xml.DescendNode();
      }break;

    case OT_TABLE:
      {
        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_TABLE );
        xml.AscendNode( KeyName );
        ReadStorageTable(v, xml );
        xml.DescendNode();
      }break;

    }
    sq_pop( v, 2 ); //  sq_next
  }
  sq_pop( v, 1 ); //  null

  return FUNCTIONRESULT_OK;
}
