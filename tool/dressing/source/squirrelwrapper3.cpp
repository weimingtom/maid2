#include"stdafx.h"
#include"squirrelwrapper.h"
#include"squirrelwrapperlocal.h"

#include <stdarg.h>


using namespace Maid;
using namespace Sqrat;



FUNCTIONRESULT SquirrelWrapper::WakeupGameEnd( RETURNCODE& ret )
{
  HSQUIRRELVM v = m_SquirrelVM;

  sq_newarray(v,0);
  sq_pushstring(v,_SC("gameend"), -1);
  sq_arrayappend(v, -2);

  return Wakeup( ret );
}


FUNCTIONRESULT SquirrelWrapper::WakeupNextFrame( RETURNCODE& ret )
{
  //  フレーム更新のデータフォーマットは
  // ["nextframe", メッセージ(なくても可) ]

  HSQUIRRELVM v = m_SquirrelVM;

  sq_newarray(v,0);
  sq_pushstring(v,_SC("nextframe"), -1);
  sq_arrayappend(v, -2);

  return Wakeup( ret );
}

FUNCTIONRESULT SquirrelWrapper::WakeupSceneFade( RETURNCODE& ret )
{
  HSQUIRRELVM v = m_SquirrelVM;

  sq_newarray(v,0);
  sq_pushstring(v,_SC("scenefade"), -1);
  sq_arrayappend(v, -2);

  return Wakeup( ret );
}


FUNCTIONRESULT SquirrelWrapper::WakeupInputState( const Maid::Keybord& key, const Maid::Mouse& mou, RETURNCODE& ret )
{
  //  key = down<<0 | in<<1 | out<<2  (true なら 1, そうでないなら0)
  //  mou = down<<0 | in<<1 | out<<2
  //  pos = xxx
  //  delta = xxx
  //  [ "inputstate", [key,key,key...] , [mou,mou,mou...], [posx,posy,wheel...], [deltax,deltay,deltawheel...] ] <- これを送る

  HSQUIRRELVM v = m_SquirrelVM;

  sq_newarray(v,0);
  sq_pushstring(v,_SC("inputstate"), -1);
  sq_arrayappend(v, -2);

  {
    //  キーボード情報
    sq_newarray(v,0);
    for( int no=0; no<256; ++no )
    {
      int state = 0;
      if( key.IsDown(no)) { BitOn(state,0); }
      if( key.IsIn(no)  ) { BitOn(state,1); }
      if( key.IsOut(no) ) { BitOn(state,2); }
      sq_pushinteger(v,state);
      sq_arrayappend(v, -2);
    }
    sq_arrayappend(v, -2);
  }

  { //  マウスボタン
    sq_newarray(v,0);
    for( unt08 no=0; no<3; ++no )
    {
      int state = 0;
      if( mou.IsDown(no)) { BitOn(state,0); }
      if( mou.IsIn(no)  ) { BitOn(state,1); }
      if( mou.IsOut(no) ) { BitOn(state,2); }
      sq_pushinteger(v,state);
      sq_arrayappend(v, -2);
    }
    sq_arrayappend(v, -2);
  }
  { //  マウス座標
    sq_newarray(v,0);
    for( unt08 no=0; no<3; ++no )
    {
      int val = mou.GetPos(no);
      sq_pushinteger(v,val);
      sq_arrayappend(v, -2);
    }
    sq_arrayappend(v, -2);
  }
  { //  マウスデルタ
    sq_newarray(v,0);
    for( unt08 no=0; no<3; ++no )
    {
      int val = mou.GetDelta(no);
      sq_pushinteger(v,val);
      sq_arrayappend(v, -2);
    }
    sq_arrayappend(v, -2);
  }

  return Wakeup( ret );
}

FUNCTIONRESULT SquirrelWrapper::WakeupStorageLoad( XMLReader& reader, RETURNCODE& ret )
{
/*
  <a>100</a>
  <b>text</b>
  <c>
    <d>5000</d>
  </c>

  なデータがあった場合

  .a = 100
  .b = "text"
  .c.d = 5000

  なデータ構造を作成する

*/


  HSQUIRRELVM v = m_SquirrelVM;
  sq_newtable(v);

  if( !reader.IsEmpty() )
  {
    reader.AscendNode();
    MakeStorageTable( v, reader );
  }

  return Wakeup( ret );
}

FUNCTIONRESULT  SquirrelWrapper::MakeStorageTag( HSQUIRRELVM v, Maid::XMLReader& reader )
{
  const String type = reader.GetAttribute( STORAGEKEYTYPE ).GetStr();

  if( type==STORAGEKEY_TABLE )
  {
    const std::wstring key = String::ConvertMAIDtoUNICODE(reader.GetNodeName());
    sq_pushstring(v, key.c_str(), -1);
    sq_newtable(v);
    reader.AscendNode();
    MakeStorageTable( v, reader );
    reader.DescendNode();
    sq_createslot(v, -3);

  }else if( type==STORAGEKEY_ARRAY )
  {
    const std::wstring key = String::ConvertMAIDtoUNICODE(reader.GetNodeName());
    sq_pushstring(v, key.c_str(), -1);
    sq_newarray(v,0);
    reader.AscendNode();
    MakeStorageArray( v, reader );
    reader.DescendNode();
    sq_createslot(v, -3);
  }

  return FUNCTIONRESULT_OK;
}

FUNCTIONRESULT  SquirrelWrapper::MakeStorageArray( HSQUIRRELVM v, Maid::XMLReader& reader )
{
  while( true )
  {
    if( reader.IsEndNode() ) { break; }

    switch( reader.GetNodeType() )
    {
    case XMLReader::NODETYPE_EMPTY: {}break;
    case XMLReader::NODETYPE_ELEMENT:
      {
        const String type = reader.GetAttribute( STORAGEKEYTYPE ).GetStr();
        const std::wstring key = String::ConvertMAIDtoUNICODE(reader.GetNodeName());

        if( type==STORAGEKEY_STRING )
        {
          const std::wstring val = String::ConvertMAIDtoUNICODE(reader.GetString());
          sq_pushstring(v, val.c_str(), -1);
          sq_arrayappend(v, -2); 
        }
        else if( type==STORAGEKEY_BOOL )
        {
          SQBool b = SQFalse;
          if( reader.GetString()==MAIDTEXT("true") ) { b = SQTrue; }
          sq_pushbool(v, b);
          sq_arrayappend(v, -2); 
        }
        else if( type==STORAGEKEY_FLOAT )
        {
          const float val = reader.GetFloat();
          sq_pushfloat(v, val);
          sq_arrayappend(v, -2); 
        }
        else if( type==STORAGEKEY_INTEGER )
        {
          const int val = reader.GetInteger();
          sq_pushinteger(v, val);
          sq_arrayappend(v, -2); 
        }
        else if( type==STORAGEKEY_NULL )
        {
          sq_pushnull(v);
          sq_arrayappend(v, -2); 
        }
      }break;

    case XMLReader::NODETYPE_TAG: 
      {
        MakeStorageTag( v, reader );
      }break;
    }
    reader.NextNode();
  }


  return FUNCTIONRESULT_OK;
}

FUNCTIONRESULT  SquirrelWrapper::MakeStorageTable( HSQUIRRELVM v, Maid::XMLReader& reader )
{
  while( true )
  {
    if( reader.IsEndNode() ) { break; }

    switch( reader.GetNodeType() )
    {
    case XMLReader::NODETYPE_EMPTY: {}break;
    case XMLReader::NODETYPE_ELEMENT:
      {
        const String type = reader.GetAttribute( STORAGEKEYTYPE ).GetStr();
        const std::wstring key = String::ConvertMAIDtoUNICODE(reader.GetNodeName());

        sq_pushstring(v, key.c_str(), -1);
        if( type==STORAGEKEY_STRING )
        {
          const std::wstring val = String::ConvertMAIDtoUNICODE(reader.GetString());
          sq_pushstring(v, val.c_str(), -1);
          sq_createslot(v, -3); 
        }
        else if( type==STORAGEKEY_BOOL )
        {
          SQBool b = SQFalse;
          if( reader.GetString()==MAIDTEXT("true") ) { b = SQTrue; }
          sq_pushbool(v, b);
          sq_createslot(v, -3); 
        }
        else if( type==STORAGEKEY_FLOAT )
        {
          const float val = reader.GetFloat();
          sq_pushfloat(v, val);
          sq_createslot(v, -3); 
        }
        else if( type==STORAGEKEY_INTEGER )
        {
          const int val = reader.GetInteger();
          sq_pushinteger(v, val);
          sq_createslot(v, -3); 
        }
      }break;

    case XMLReader::NODETYPE_TAG: 
      {
        MakeStorageTag( v, reader );
      }break;
    }
    reader.NextNode();
  }

  return FUNCTIONRESULT_OK;
}


FUNCTIONRESULT SquirrelWrapper::WakeupStorageSave( RETURNCODE& ret )
{
  return Wakeup( ret );
}



FUNCTIONRESULT SquirrelWrapper::Wakeup( RETURNCODE& ret )
{
  MAID_ASSERT( sq_getvmstate(m_SquirrelVM)!=SQ_VMSTATE_SUSPENDED, "suspend中ではありません" );
  HSQUIRRELVM v = m_SquirrelVM;

  const SQBool raise = IsRaiseError();
  const SQBool input = SQTrue;

  ret = RETURNCODE();

  const SQRESULT ret_wakeup = sq_wakeupvm(v, input, SQTrue, raise, SQFalse);
  if( SQ_FAILED(ret_wakeup) )
  {
    WriteErrorLog();
    return FUNCTIONRESULT_ERROR; 
  }

  return UpdateReturnCode( ret );
}

