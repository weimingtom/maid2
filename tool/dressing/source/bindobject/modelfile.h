#ifndef tool_dressing_bindobject_modelfile_h
#define tool_dressing_bindobject_modelfile_h


#include"cppdrawtype.h"


class ModelFile 
  : public CppDrawType
{
public:
  static void Register();
  float Alpha;

  HSQOBJECT Matrix;

  Maid::ModelMQO Model;

  Sqrat::string GetFileName() const;
  void SetFileName(const Sqrat::string& name);

  ModelFile();
  ~ModelFile();
  virtual void Draw( float time, const Maid::POINT3DF& pos );
};


// ModelFile専用アロケータ
class ModelFileAllocator {
public:
	static SQInteger New(HSQUIRRELVM vm) {
		ModelFile* p = new ModelFile();
		sq_setinstanceup(vm, 1, p);
		sq_setreleasehook(vm, 1, &Delete);
		return 0;
	}

	static SQInteger Copy(HSQUIRRELVM vm, SQInteger idx, const void* value) {

    MAID_ASSERT( true, "どんなときにくるか調査中" );
/*
		const ModelFile* src = reinterpret_cast<const ModelFile*>(value);
		ModelFile* dst = new ModelFile();

    sq_pushobject( vm, src->Matrix );
    sq_clone( vm, -1 );
    const SQRESULT ret_getstack = sq_getstackobj(vm, 2, &dst->Matrix);
    sq_pop(vm,2);

		sq_setinstanceup(vm, idx, dst);
		sq_setreleasehook(vm, idx, &Delete);
*/
		return 0;
	}

	static SQInteger Delete(SQUserPointer ptr, SQInteger size) {
		ModelFile* instance = reinterpret_cast<ModelFile*>(ptr);
		delete instance;
		return 0;
	}
};

#endif
