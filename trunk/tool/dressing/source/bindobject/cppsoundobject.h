#ifndef tool_dressing_bindobject_cppsoundobject_h
#define tool_dressing_bindobject_cppsoundobject_h

#include"cppsoundtype.h"

class CppSoundObject
{
public:
  static void Register();
  CppSoundObject();
  virtual ~CppSoundObject();


  bool GetPlay() const;
  void SetPlay(const bool& type);

  float GetVolume() const;
  void SetVolume(const float& vol);

  float GetPosition() const;
  void SetPosition(const float& time);

  HSQOBJECT Type;


private:
  CppSoundType*   GetType();
  const CppSoundType*   GetType()const;


private:
  // コピー禁止
   CppSoundObject& operator=(const CppSoundObject& obj) { return *this; }
};

// CppSoundObject専用アロケータ
class CppSoundObjectAllocator {
public:
	static SQInteger New(HSQUIRRELVM vm) {
		CppSoundObject* p = new CppSoundObject();
		sq_setinstanceup(vm, 1, p);
		sq_setreleasehook(vm, 1, &Delete);
		return 0;
	}

	static SQInteger Copy(HSQUIRRELVM vm, SQInteger idx, const void* value) {

    MAID_ASSERT( true, "どんなときにくるか調査中" );
/*
		const CppSoundObject* src = reinterpret_cast<const CppSoundObject*>(value);
		CppSoundObject* dst = new CppSoundObject();

    sq_pushobject( vm, src->Type );
    sq_clone( vm, -1 );
    const SQRESULT ret_getstack = sq_getstackobj(vm, 2, &dst->Type);
    sq_pop(vm,2);

		sq_setinstanceup(vm, idx, dst);
		sq_setreleasehook(vm, idx, &Delete);
*/
		return 0;
	}

	static SQInteger Delete(SQUserPointer ptr, SQInteger size) {
		CppSoundObject* instance = reinterpret_cast<CppSoundObject*>(ptr);
		delete instance;
		return 0;
	}
};



#endif
