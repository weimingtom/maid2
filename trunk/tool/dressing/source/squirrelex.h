#ifndef tool_dressing_squirrelex_h
#define tool_dressing_squirrelex_h

#include"import/squirrel/include/squirrel.h"
#include"import/sqrat/include/sqrat.h"


SQUserPointer sq_objtoinstance(HSQOBJECT * po);

namespace Sqrat
{

template <class C>
inline SQInteger sqHSQOBJECTSet(HSQUIRRELVM vm) {
	C* ptr = NULL;
	sq_getinstanceup(vm, 1, (SQUserPointer*)&ptr, NULL);
	
	typedef HSQOBJECT C::*M;
	M* memberPtr = NULL;
	sq_getuserdata(vm, -1, (SQUserPointer*)&memberPtr, NULL); // Get Member...
	M member = *memberPtr;
 
  const SQBool retbool = sq_release( vm, &(ptr->*member) );

  const SQRESULT ret_getstack = sq_getstackobj(vm, 2, &(ptr->*member));
  sq_addref(vm, &(ptr->*member));

  return 0;
}

template <class C>
inline SQInteger sqHSQOBJECTGet(HSQUIRRELVM vm) {
	C* ptr = NULL;
	sq_getinstanceup(vm, 1, (SQUserPointer*)&ptr, NULL);
	
	typedef HSQOBJECT C::*M;
	M* memberPtr = NULL;
	sq_getuserdata(vm, -1, (SQUserPointer*)&memberPtr, NULL); // Get Member...
	M member = *memberPtr;
	
  sq_pushobject( vm, ptr->*member );

	return 1;
}

template<class C, class A>
class ClassEx : public Class<C,A> {
public:

  ClassEx(HSQUIRRELVM v = DefaultVM::Get(), bool createClass = true) : Class<C,A>(v, createClass){}

	ClassEx& SqObject(const SQChar* name, HSQOBJECT C::* var) {
		// Add the getter
		BindAccessor(name, &var, sizeof(var), &sqHSQOBJECTGet<C>, ClassType<C>::GetTable());

		// Add the setter
		BindAccessor(name, &var, sizeof(var), &sqHSQOBJECTSet<C>, ClassType<C>::SetTable());

		return *this;
	}
};

template<class C, class B, class A>
class DerivedClassEx : public DerivedClass<C,B,A> {
public:

  DerivedClassEx(HSQUIRRELVM v = DefaultVM::Get()) : DerivedClass<C,B,A>(v){}

	DerivedClassEx& SqObject(const SQChar* name, HSQOBJECT C::* var) {
		// Add the getter
		BindAccessor(name, &var, sizeof(var), &sqHSQOBJECTGet<C>, ClassType<C>::GetTable());

		// Add the setter
		BindAccessor(name, &var, sizeof(var), &sqHSQOBJECTSet<C>, ClassType<C>::SetTable());

		return *this;
	}
};
}

#endif