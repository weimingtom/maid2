#ifndef tool_dressing_bindobject_colorrect_h
#define tool_dressing_bindobject_colorrect_h


#include"cppdrawtype.h"


class ColorRect 
  : public CppDrawType
{
public:
  static void Register();
  int CenterX;
  int CenterY;
  int Width;
  int Height;

  HSQOBJECT Matrix;

  float ColorR;
  float ColorG;
  float ColorB;
  float Alpha;

  ColorRect();
  ~ColorRect();
  virtual void Draw( float time, const Maid::POINT3DF& pos );
};


// CppDrawObject専用アロケータ
class ColorRectAllocator {
public:
	static SQInteger New(HSQUIRRELVM vm) {
		ColorRect* p = new ColorRect();
		sq_setinstanceup(vm, 1, p);
		sq_setreleasehook(vm, 1, &Delete);
		return 0;
	}

	static SQInteger Copy(HSQUIRRELVM vm, SQInteger idx, const void* value) {

    MAID_ASSERT( true, "どんなときにくるか調査中" );
/*
		const ColorRect* src = reinterpret_cast<const ColorRect*>(value);
		ColorRect* dst = new ColorRect();

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
		ColorRect* instance = reinterpret_cast<ColorRect*>(ptr);
		delete instance;
		return 0;
	}
};

#endif
