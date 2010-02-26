#include"stdafx.h"
#include"squirrelwrapper.h"
#include"../../../source/storage/fileio/filereadnormal.h"

#include <stdarg.h>


using namespace Maid;
using namespace Sqrat;



void printfunc(HSQUIRRELVM v, const SQChar *s, ...) 
{ 
  wchar_t buf[256];

  va_list arglist; 
  va_start(arglist, s); 
  vswprintf_s(buf, s, arglist);
  va_end(arglist); 

  MAID_WARNING( String::ConvertUNICODEtoMAID(buf));
}



struct StringReader
{
  const String& str;
  String::size_type pos;

  StringReader( const String& s ):str(s),pos(0){}
};

static SQInteger ReadString(SQUserPointer p )
{
	SQInteger ret = 0;
  StringReader* pReader = (StringReader*)p;

  if( pReader->pos < pReader->str.length() )
  {
    ret = pReader->str[pReader->pos];

    pReader->pos += 1;
  }

  return ret;
}

SquirrelWrapper::SquirrelWrapper()
  :m_SquirrelVM(NULL)
  ,m_IsRaiseError(NULL)
{

}

SquirrelWrapper::~SquirrelWrapper()
{
  Finalize();
}

FUNCTIONRESULT SquirrelWrapper::Initialize( int StackSize, bool raiseerror )
{
  HSQUIRRELVM v = sq_open(StackSize);

  if( v==NULL ) { return FUNCTIONRESULT_ERROR; }

  sq_setprintfunc(v, printfunc); //sets the print function
  sqstd_seterrorhandlers(v); //registers the default error handlers
  DefaultVM::Set(v);

  m_IsRaiseError = raiseerror;
  m_SquirrelVM = v;

  SetupBindObject();

  return FUNCTIONRESULT_OK;
}

void SquirrelWrapper::Finalize()
{
  HSQUIRRELVM v = m_SquirrelVM;

  if( v==NULL ) { return ; }

  ReleaseBindObject();
  DefaultVM::Set(NULL);
	sq_close(v); 
  m_SquirrelVM = NULL;
}


FUNCTIONRESULT SquirrelWrapper::BindFunction( SQFUNCTION func, const Maid::String& Name )
{
  const std::wstring name = String::ConvertMAIDtoUNICODE(Name);

  HSQUIRRELVM v = m_SquirrelVM;
  sq_pushroottable(v);
  sq_pushstring(v, name.c_str(), -1);
  sq_newclosure(v, func, 0);
  sq_createslot(v, -3); 
  sq_pop(v, 1);

  return FUNCTIONRESULT_OK;
}



FUNCTIONRESULT SquirrelWrapper::Import( const Maid::String& SourceCode, const Maid::String& FileName )
{
  RETURNCODE retcode;

  const FUNCTIONRESULT ret = Execute( SourceCode, FileName, retcode );
  if( FUNCTIONRESULT_FAILE(ret) ) { return FUNCTIONRESULT_ERROR; }

  if( retcode.Code!=RETURNCODE::EXIT ) { return FUNCTIONRESULT_ERROR; }
  return FUNCTIONRESULT_OK;
}

FUNCTIONRESULT SquirrelWrapper::Execute( const Maid::String& SourceCode, const Maid::String& FileName, RETURNCODE& ret )
{
  HSQUIRRELVM v = m_SquirrelVM;

  //  squirrel用に情報を集める
  const std::wstring filename = String::ConvertMAIDtoUNICODE(FileName);
  const SQBool raise = IsRaiseError();
  StringReader reader(SourceCode);

  {
    {
      //  そしてコンパイル＆エントリポイントをスタックへ積む
      const SQRESULT ret = sq_compile( v, ReadString, &reader, filename.c_str(), raise );
      if( SQ_FAILED(ret) ) { WriteErrorLog(); return FUNCTIONRESULT_ERROR; }
    }

    {
      //  そんでもって呼び出して完了
      //  戻り値は必ずある設定にする
      sq_pushroottable(v);
      const SQRESULT ret_call = sq_call(v, 1,  SQTrue, raise );
      if( SQ_FAILED(ret_call) ) { WriteErrorLog(); return FUNCTIONRESULT_ERROR; }

      UpdateReturnCode( ret );
    }
  }

  return FUNCTIONRESULT_OK;
}

void SquirrelWrapper::ExitGameLoop()
{
  HSQUIRRELVM v = m_SquirrelVM;

  sq_pushroottable(v);
  sq_pushstring( v, L"ExitGameLoop", -1 );
  SQRESULT ret_admin = sq_get(v, -2);
  sq_pushroottable(v);

  const SQRESULT ret_call = sq_call(v, 1,  SQFalse, IsRaiseError() );
  sq_pop(v,3);
}

bool SquirrelWrapper::IsSuspended() const
{
  return sq_getvmstate(m_SquirrelVM)==SQ_VMSTATE_SUSPENDED;
}




FUNCTIONRESULT SquirrelWrapper::UpdateReturnCode( RETURNCODE& ret )
{
  HSQUIRRELVM v = m_SquirrelVM;

  ret.Param.clear();
  const SQRESULT ret_state = sq_getvmstate(v);
  if( ret_state == SQ_VMSTATE_SUSPENDED ) 
  {
    // suspendの場合、考えられるのは
    // import() 内
    // updateframe() 終了
    // なんにせよ、配列を投げるようにしてあるので、それを回収する
    const SQObjectType resulttype = sq_gettype(v, -1);
    MAID_ASSERT( resulttype!=OT_ARRAY, "array以外の物が来ています" );

    sq_pushnull(v);

    {
      //  配列の０番目は終了コードになってる
      const SQRESULT next_ret = sq_next(v,-2);
      if( SQ_FAILED(next_ret) ) { return FUNCTIONRESULT_ERROR; }
      const SQObjectType objtype = sq_gettype(v, -1);
      MAID_ASSERT( objtype!=OT_STRING, "予想外の値" );
      const SQChar* buf=NULL;
      const SQRESULT getstring_ret = sq_getstring(v, -1, &buf);
      if( SQ_FAILED(getstring_ret) ) { MAID_ASSERT( true, "予想外の値" ); }

      const String str = String::ConvertUNICODEtoMAID(buf);
           if( str==MAIDTEXT("exit"  )    ) { ret.Code = RETURNCODE::EXIT; }
      else if( str==MAIDTEXT("nextframe") ) { ret.Code = RETURNCODE::NEXTFRAME; }
      else if( str==MAIDTEXT("inputstate")) { ret.Code = RETURNCODE::INPUTSTATE; }
      else if( str==MAIDTEXT("scenefade")) { ret.Code = RETURNCODE::SCENEFADE; }
      else if( str==MAIDTEXT("storageload")) { ret.Code = RETURNCODE::STORAGELOAD; }
      else if( str==MAIDTEXT("storagesave")) { ret.Code = RETURNCODE::STORAGESAVE; }
      else { MAID_ASSERT( true, "予想外の値" ); }
      sq_pop( v, 2 );

    }

    while( true )
    {
      const SQRESULT next_ret = sq_next(v,-2);
      if( SQ_FAILED(next_ret) ) { break; }

      const SQObjectType objtype = sq_gettype(v, -1);
      switch( objtype )
      {
      case OT_INTEGER:
        {
          SQInteger val;
          const SQRESULT ret_ = sq_getinteger(v, -1, &val);
          if( SQ_FAILED(ret_) ) { MAID_ASSERT( true, "予想外の値" ); }
          ret.Param.push_back( String::ItoA(val) );
        }break;
      case OT_FLOAT:
        {
          SQFloat val;
          const SQRESULT ret_ = sq_getfloat(v, -1, &val);
          if( SQ_FAILED(ret_) ) { MAID_ASSERT( true, "予想外の値" ); }
          ret.Param.push_back( String::FtoA(val) );
        }break;
      case OT_STRING: 
        {
          const SQChar* buf=NULL;
          const SQRESULT ret_ = sq_getstring(v, -1, &buf);
          if( SQ_FAILED(ret_) ) { MAID_ASSERT( true, "予想外の値" ); }
          ret.Param.push_back( String::ConvertUNICODEtoMAID(buf) );
        }break;
      default: { MAID_ASSERT( true, "予想外の値" ); }break;
      }

      sq_pop( v, 2 );
    }
    sq_pop( v, 2 ); // push_null() と squirrelが戻してきたarray

  }
  else
  { //  suspendでない場合は正常終了してる
    ret.Code = RETURNCODE::EXIT;
    sq_pop( v, 1 ); // squirrelが戻してきたarray
  }

  return FUNCTIONRESULT_OK;
}

SQBool SquirrelWrapper::IsRaiseError() const
{
  return m_IsRaiseError? SQTrue:SQFalse;;
}

void SquirrelWrapper::WriteErrorLog()
{
  HSQUIRRELVM v = m_SquirrelVM;

  const SQChar* err = NULL;
	sq_getlasterror(v);

  const SQRESULT ret = sq_getstring(v,-1,&err);
  if(SQ_FAILED(ret)) { MAID_WARNING( "squirrelで用意されていないエラー" ); return ; }

  MAID_WARNING( String::ConvertUNICODEtoMAID(err) );

  sq_pop(v,1);
}
