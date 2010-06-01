#include"opengl32dll.h"


namespace Maid { namespace Graphics {

void OpenGL32DLL::Initialize()
{
  m_OpenGLDLL.Load( MAIDTEXT("opengl32.dll") );

  m_wglCreateContext = (WGLCREATECONTEXT)m_OpenGLDLL.GetProcAddress(MAIDTEXT("wglCreateContext"));
  m_wglDeleteContext = (WGLDELETECONTEXT)m_OpenGLDLL.GetProcAddress(MAIDTEXT("wglDeleteContext"));
  m_wglMakeCurrent   = (WGLMAKECURRENT)m_OpenGLDLL.GetProcAddress(MAIDTEXT("wglMakeCurrent"));
  m_wglGetProcAddress= (WGLGETPROCADDRESS)m_OpenGLDLL.GetProcAddress(MAIDTEXT("wglGetProcAddress"));

  m_wglGetCurrentContext= (WGLGETCURRENTCONTEXT)m_OpenGLDLL.GetProcAddress(MAIDTEXT("wglGetCurrentContext"));
  m_wglGetCurrentDC     = (WGLGETCURRENTDC)m_OpenGLDLL.GetProcAddress(MAIDTEXT("wglGetCurrentDC"));
  m_wglShareLists     = (WGLSHARELISTS)m_OpenGLDLL.GetProcAddress(MAIDTEXT("wglShareLists"));

  m_glGetError= (GLGETERROR)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glGetError"));
  m_glFlush   = (GLFLUSH)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glFlush"));

  m_glClear        = (GLCLEAR)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glClear"));
  m_glClearColor   = (GLCLEARCOLOR)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glClearColor"));
  m_glClearDepth   = (GLCLEARDEPTH)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glClearDepth"));
  m_glClearStencil = (GLCLEARSTENCIL)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glClearStencil"));
  m_glViewport     = (GLVIEWPORT)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glViewport"));
  m_glDepthRange   = (GLDEPTHRANGE)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glDepthRange"));

  m_glGetString = (GLGETSTRING)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glGetString"));

  m_glEnableClientState  = (GLENABLECLIENTSTATE)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glEnableClientState"));
  m_glDisableClientState = (GLDISABLECLIENTSTATE)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glDisableClientState"));

  m_glEnable  = (GLENABLE)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glEnable"));
  m_glDisable = (GLDISABLE)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glDisable"));
  m_glBlendFunc = (GLBLENDFUNC)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glBlendFunc"));

  m_glGenTextures = (GLGENTEXTURES)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glGenTextures"));
  m_glBindTexture = (GLBINDTEXTURE)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glBindTexture"));
  m_glTexImage2D = (GLTEXIMAGE2D)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glTexImage2D"));
  m_glDeleteTextures = (GLDELETETEXTURES)m_OpenGLDLL.GetProcAddress(MAIDTEXT("glDeleteTextures"));

}

HGLRC OpenGL32DLL::wglCreateContext( HDC hDC )
{
  return m_wglCreateContext( hDC );
}

BOOL OpenGL32DLL::wglDeleteContext( HGLRC hRC )
{
  return m_wglDeleteContext( hRC );
}

BOOL OpenGL32DLL::wglMakeCurrent( HDC hDC, HGLRC hRC )
{
  return m_wglMakeCurrent( hDC, hRC );
}


PROC OpenGL32DLL::wglGetProcAddress( LPCSTR str )
{
  return m_wglGetProcAddress( str );
}

HGLRC OpenGL32DLL::wglGetCurrentContext()
{
  return m_wglGetCurrentContext();
}

HDC OpenGL32DLL::wglGetCurrentDC()
{
  return m_wglGetCurrentDC();
}

BOOL  OpenGL32DLL::wglShareLists( HGLRC lhs, HGLRC rhs )
{
  return m_wglShareLists(lhs,rhs);
}


GLenum OpenGL32DLL::glGetError()
{
  return m_glGetError();
}

void OpenGL32DLL::glClear(GLbitfield mask )
{
  m_glClear( mask );
}

void OpenGL32DLL::glClearColor(GLclampf r, GLclampf g, GLclampf b, GLclampf a)
{
  m_glClearColor( r, g, b, a );
}
void OpenGL32DLL::glClearDepth(GLclampd d)
{
  m_glClearDepth( d );
}

void OpenGL32DLL::glClearStencil(GLint s)
{
  m_glClearStencil( s );
}

void OpenGL32DLL::glFlush()
{
  m_glFlush();
}

void OpenGL32DLL::glViewport( GLint x, GLint y, GLint width, GLint height )
{
  m_glViewport( x, y, width, height );
}

void OpenGL32DLL::glDepthRange( GLclampd znear, GLclampd zfar )
{
  m_glDepthRange( znear, zfar );

}


const GLubyte* OpenGL32DLL::glGetString( GLenum num )
{
  return m_glGetString( num );
}


void OpenGL32DLL::glEnableClientState( GLenum a )
{
  m_glEnableClientState( a );
}

void OpenGL32DLL::glDisableClientState( GLenum a )
{
  m_glDisableClientState( a );
}


void OpenGL32DLL::glEnable( GLenum a )
{
  m_glEnable( a );
}

void OpenGL32DLL::glDisable( GLenum a )
{
  m_glDisable( a );
}

void OpenGL32DLL::glBlendFunc( GLenum src, GLenum dst )
{
  m_glBlendFunc( src, dst );
}


void OpenGL32DLL::glGenTextures( GLsizei n, GLuint* textures )
{
  m_glGenTextures( n, textures );
}

void OpenGL32DLL::glBindTexture( GLenum target, GLuint texture )
{
  m_glBindTexture( target, texture );
}

void OpenGL32DLL::glTexImage2D( GLenum target, GLint level, GLint components, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels )
{
  m_glTexImage2D( target, level, components, width, height, border, format, type, pixels );
}

void OpenGL32DLL::glDeleteTextures( GLsizei n , const GLuint* textures )
{
  m_glDeleteTextures( n, textures );
}





}}


