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

static float mysqrt( float val )
{
  return Math<float>::sqrt( val );
}



static float mysin( float val )
{
  return Math<float>::sin( val );
}

static float mycos( float val )
{
  return Math<float>::cos( val );
}

static float mytan( float val )
{
  return Math<float>::tan( val );
}

void SquirrelWrapper::SetupBindObject()
{
  {
    RootTable()
      .Func( _SC("RandI") , &RandI )
      .Func( _SC("sqrt") , &mysqrt )
      .Func( _SC("sin") , &mysin )
      .Func( _SC("cos") , &mycos )
      .Func( _SC("tan") , &mytan )
      ;

  }
  {
    CppMatrix::Register();

  }
  {
    CppDrawType::Register();
    ColorRect::Register();
    ImageRect::Register();
    ColorText::Register();

    ColorRect3D::Register();
    ImageRect3D::Register();
    ModelFile::Register();
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


FUNCTIONRESULT SquirrelWrapper::GetSceneInfo( SCENEINFOLIST& list )
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

    SCENEINFO sceneinfo;
    sq_pushnull(v);
    while( true )
    {
      const SQRESULT next_ret = sq_next(v,-2);
      if( SQ_FAILED(next_ret) ) { break; }

      const SQObjectType keytype = sq_gettype(v, -2);
      const SQObjectType valuetype = sq_gettype(v, -1);

      String KeyName;
      {
        const SQChar* p;
        sq_getstring(v,-2, &p );
        KeyName = String::ConvertUNICODEtoMAID(p);
      }

      if( KeyName==MAIDTEXT("camera") )
      {
        ReadCameraData( v, sceneinfo );
      }
      else if( KeyName==MAIDTEXT("object") )
      {
        ReadDrawObject( v, sceneinfo );
      }
      else if( KeyName==MAIDTEXT("baselight") )
      {
        ReadBaseLight( v, sceneinfo );
      }

      sq_pop( v, 2 );
    }
    sq_pop( v, 1 );
    list.push_back(sceneinfo);

    sq_pop( v, 2 );
  }
  sq_pop( v, 6 ); // nullとcall の戻り値と &GameObject, &admin, v

  return FUNCTIONRESULT_OK;
}


FUNCTIONRESULT  SquirrelWrapper::ReadCameraData( HSQUIRRELVM v, SCENEINFO& info )
{
  {
    sq_pushstring( v, L"Eye", -1 );
    SQRESULT ret = sq_get(v, -2);

    info.CameraEye = POINT3DF( GetFloat(v,L"x"), GetFloat(v,L"y"), GetFloat(v,L"z") );
    sq_pop( v, 1 );
  }
  {
    sq_pushstring( v, L"Target", -1 );
    SQRESULT ret = sq_get(v, -2);

    info.CameraTarget = POINT3DF( GetFloat(v,L"x"), GetFloat(v,L"y"), GetFloat(v,L"z") );
    sq_pop( v, 1 );
  }

  {
    sq_pushstring( v, L"Up", -1 );
    SQRESULT ret = sq_get(v, -2);

    info.CameraUp = VECTOR3DF( GetFloat(v,L"x"), GetFloat(v,L"y"), GetFloat(v,L"z") );
    sq_pop( v, 1 );
  }

  info.CameraFov    = GetFloat( v, L"Fov") ;
  info.CameraAspect = GetFloat( v, L"Aspect") ;
  info.CameraNear   = GetFloat( v, L"Near") ;
  info.CameraFar    = GetFloat( v, L"Far") ;
  info.CameraParallaxEye = GetFloat( v, L"ParallaxEye") ;
  info.CameraParallaxTarget = GetFloat( v, L"ParallaxTarget") ;

  return FUNCTIONRESULT_OK;
}

FUNCTIONRESULT  SquirrelWrapper::ReadBaseLight( HSQUIRRELVM v, SCENEINFO& info )
{
  {
    sq_pushstring( v, L"Ambient", -1 );
    SQRESULT ret = sq_get(v, -2);

    info.AmbientLight = COLOR_R32G32B32A32F( GetFloat(v,L"r"), GetFloat(v,L"g"), GetFloat(v,L"b"), GetFloat(v,L"a") );
    sq_pop( v, 1 );

  }

  {
    sq_pushstring( v, L"Direction", -1 );
    SQRESULT ret = sq_get(v, -2);
    {
      sq_pushstring( v, L"Color", -1 );
      SQRESULT ret = sq_get(v, -2);

      info.DirectionalLightColor = COLOR_R32G32B32A32F( GetFloat(v,L"r"), GetFloat(v,L"g"), GetFloat(v,L"b"), GetFloat(v,L"a") );
      sq_pop( v, 1 );
    }

    {
      sq_pushstring( v, L"Direction", -1 );
      SQRESULT ret = sq_get(v, -2);

      info.DirectionalLightVec = VECTOR3DF( GetFloat(v,L"x"), GetFloat(v,L"y"), GetFloat(v,L"z") );
      sq_pop( v, 1 );
    }
    sq_pop( v, 1 );
  }
  return FUNCTIONRESULT_OK;
}


float SquirrelWrapper::GetFloat( HSQUIRRELVM v, const SQChar* p )
{
  sq_pushstring( v, p, -1 );
  SQRESULT ret__GetGameObject = sq_get(v, -2);

  float val;
  sq_getfloat( v, -1, &val );

  sq_pop( v, 1 );
  return val;
}

int   SquirrelWrapper::GetInteger( HSQUIRRELVM v, const SQChar* p )
{
  sq_pushstring( v, p, -1 );
  SQRESULT ret__GetGameObject = sq_get(v, -2);

  int val;
  sq_getinteger( v, -1, &val );

  sq_pop( v, 1 );
  return val;
}


FUNCTIONRESULT  SquirrelWrapper::ReadDrawObject( HSQUIRRELVM v, SCENEINFO& info )
{
  sq_pushnull(v);
  while( true )
  {
    const SQRESULT next_ret = sq_next(v,-2);
    if( SQ_FAILED(next_ret) ) { break; }

    const SQObjectType keytype = sq_gettype(v, -2);
    const SQObjectType valuetype = sq_gettype(v, -1);
    {
      CppDrawObject* pObj = NULL;
      const SQRESULT ret_get = sq_getinstanceup( v, -1, (SQUserPointer*)&pObj, NULL );
      if( SQ_FAILED(ret_get) ) { continue; }

      info.ObjectList.push_back( pObj );
    }
    sq_pop( v, 2 );
  }
  sq_pop( v, 1 );

  return FUNCTIONRESULT_OK;
}


FUNCTIONRESULT SquirrelWrapper::GetStorageData( XMLWriter& xml )
{
  HSQUIRRELVM v = m_SquirrelVM;

  sq_pushroottable(v);
  sq_pushstring( v, L"g_Storage", -1 );
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
        xml.AddTag( ARRAYKEY, STORAGEVALUE_EMPTY  );
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

    xml.AddAttribute( STORAGEKEYNAME, KeyName );
    switch( valuetype )
    {
    case OT_INTEGER:
      {
        SQInteger i;
        sq_getinteger( v, -1, &i );

        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_INTEGER );
        xml.AddTag( STORAGETAG, (int)i );
      }break;
    case OT_FLOAT:
      {
        SQFloat f;
        sq_getfloat( v, -1, &f );

        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_FLOAT );
        xml.AddTag( STORAGETAG, (float)f );
      }break;

    case OT_STRING:
      {
        const SQChar* str;
        sq_getstring( v, -1, &str );

        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_STRING );
        xml.AddTag( STORAGETAG, String::ConvertUNICODEtoMAID(str) );
      }break;

    case OT_BOOL:
      {
        SQBool b;
        sq_getbool( v, -1, &b );

        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_BOOL );

        if( b ) { xml.AddTag( STORAGETAG, STORAGEVALUE_TRUE  ); }
        else    { xml.AddTag( STORAGETAG, STORAGEVALUE_FALSE ); }
      }break;

    case OT_NULL:
      {
        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_NULL );
        xml.AddTag( STORAGETAG, STORAGEVALUE_EMPTY  );
      }break;

    case OT_ARRAY:
      {
        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_ARRAY );
        xml.AscendNode( STORAGETAG );
        ReadStorageArray(v, xml );
        xml.DescendNode();
      }break;

    case OT_TABLE:
      {
        xml.AddAttribute( STORAGEKEYTYPE, STORAGEKEY_TABLE );
        xml.AscendNode( STORAGETAG );
        ReadStorageTable(v, xml );
        xml.DescendNode();
      }break;

    }
    sq_pop( v, 2 ); //  sq_next
  }
  sq_pop( v, 1 ); //  null

  return FUNCTIONRESULT_OK;
}
