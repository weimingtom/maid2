#ifndef tool_dressing_bindobject_imagerect_h
#define tool_dressing_bindobject_imagerect_h


#include"cppdrawtype.h"


class ImageRect 
  : public CppDrawType
{
public:
  static void Register();
  int CenterX;
  int CenterY;
  int RectX;
  int RectY;
  int RectW;
  int RectH;
  float Alpha;

  HSQOBJECT Matrix;

  Maid::Texture2D Texture;

  Sqrat::string GetFileName() const;
  void SetFileName(const Sqrat::string& name);

  ImageRect();
  ~ImageRect();
  virtual void Draw( float time, const Maid::POINT3DF& pos );
};


// ImageRect専用アロケータ
class ImageRectAllocator {
public:
	static SQInteger New(HSQUIRRELVM vm) {
		ImageRect* p = new ImageRect();
		sq_setinstanceup(vm, 1, p);
		sq_setreleasehook(vm, 1, &Delete);
		return 0;
	}

	static SQInteger Copy(HSQUIRRELVM vm, SQInteger idx, const void* value) {

    MAID_ASSERT( true, "どんなときにくるか調査中" );
/*
		const ImageRect* src = reinterpret_cast<const ImageRect*>(value);
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
		ImageRect* instance = reinterpret_cast<ImageRect*>(ptr);
		delete instance;
		return 0;
	}
};

#endif
