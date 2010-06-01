#ifndef maid2_graphics_core_win32_opengl_iopenglobject_h
#define maid2_graphics_core_win32_opengl_iopenglobject_h


#include"../../../../config/define.h"

#include"opengl32dll.h"
#include"openglextension.h"


namespace Maid { namespace Graphics {

  class IOpenGLObject 
  {
  public:
    virtual ~IOpenGLObject(){}

    static void SetRuntime( OpenGL32DLL* dll, OpenGLExtension* ext );
  protected:
    static OpenGL32DLL*     s_pDLL;
    static OpenGLExtension* s_pExt;
  };

}}

#endif