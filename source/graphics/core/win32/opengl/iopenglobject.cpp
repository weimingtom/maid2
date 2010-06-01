#include"iopenglobject.h"


namespace Maid { namespace Graphics {

OpenGL32DLL*     IOpenGLObject::s_pDLL;
OpenGLExtension* IOpenGLObject::s_pExt;


void IOpenGLObject::SetRuntime( OpenGL32DLL* dll, OpenGLExtension* ext )
{
  s_pDLL = dll;
  s_pExt = ext;
}




}}


