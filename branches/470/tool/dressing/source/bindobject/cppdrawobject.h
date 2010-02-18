#ifndef tool_dressing_bindobject_cppdrawobject_h
#define tool_dressing_bindobject_cppdrawobject_h

#include"cppdrawtype.h"

class CppDrawObject
{
public:
  static void Register();
  CppDrawObject();
  virtual ~CppDrawObject();

  float x;
  float y;
  float z;
  float time;

  HSQOBJECT Type;
  HSQOBJECT Parent;  //  このオブジェクトの親(NULLなこともある)

  const CppDrawObject* GetParent() const;
  CppDrawType*   GetType();

  Maid::POINT3DF CalcGlobalPos() const;

private:
  // コピー禁止
   CppDrawObject& operator=(const CppDrawObject& obj) { return *this; }
};

// CppDrawObject専用アロケータ
class CppDrawObjectAllocator {
public:
	static SQInteger New(HSQUIRRELVM vm) {
		CppDrawObject* p = new CppDrawObject();
		sq_setinstanceup(vm, 1, p);
		sq_setreleasehook(vm, 1, &Delete);
		return 0;
	}

	static SQInteger Copy(HSQUIRRELVM vm, SQInteger idx, const void* value) {

    MAID_ASSERT( true, "どんなときにくるか調査中" );
/*
		const CppDrawObject* src = reinterpret_cast<const CppDrawObject*>(value);
		CppDrawObject* dst = new CppDrawObject();

    sq_pushobject( vm, src->DrawType );
    sq_clone( vm, -1 );
    const SQRESULT ret_getstack = sq_getstackobj(vm, 2, &dst->DrawType);
    sq_pop(vm,2);

		sq_setinstanceup(vm, idx, dst);
		sq_setreleasehook(vm, idx, &Delete);
*/
		return 0;
	}

	static SQInteger Delete(SQUserPointer ptr, SQInteger size) {
		CppDrawObject* instance = reinterpret_cast<CppDrawObject*>(ptr);
		delete instance;
		return 0;
	}
};



#endif
