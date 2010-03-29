#ifndef tool_dressing_bindobject_imagerect3d_h
#define tool_dressing_bindobject_imagerect3d_h


#include"cppdrawtype.h"


class ImageRect3D 
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

  ImageRect3D();
  ~ImageRect3D();
  virtual void Draw( float time, const Maid::POINT3DF& pos );
};


// ImageRect3D専用アロケータ
class ImageRect3DAllocator {
public:
	static SQInteger New(HSQUIRRELVM vm) {
		ImageRect3D* p = new ImageRect3D();
		sq_setinstanceup(vm, 1, p);
		sq_setreleasehook(vm, 1, &Delete);
		return 0;
	}

	static SQInteger Copy(HSQUIRRELVM vm, SQInteger idx, const void* value) {

    MAID_ASSERT( true, "どんなときにくるか調査中" );
/*
		const ImageRect3D* src = reinterpret_cast<const ImageRect3D*>(value);
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
		ImageRect3D* instance = reinterpret_cast<ImageRect3D*>(ptr);
		delete instance;
		return 0;
	}
};

#endif
