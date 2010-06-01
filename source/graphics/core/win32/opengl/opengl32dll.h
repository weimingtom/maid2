#ifndef maid2_graphics_core_win32_opengl_opengl32dll_h
#define maid2_graphics_core_win32_opengl_opengl32dll_h


#include"../../../../config/define.h"
#include"../../../../framework/win32/dllwrapper.h"

#include <gl/gl.h>
#include <gl/glu.h>


namespace Maid { namespace Graphics {

  class OpenGL32DLL
  {
  public:
    void Initialize();

    HGLRC wglCreateContext( HDC hDC );
    BOOL  wglDeleteContext( HGLRC hRC );
    BOOL  wglMakeCurrent( HDC hDC, HGLRC hRC );
    PROC  wglGetProcAddress( LPCSTR );

    HGLRC wglGetCurrentContext();
    HDC   wglGetCurrentDC();
    BOOL  wglShareLists( HGLRC, HGLRC );

    const GLubyte* glGetString( GLenum );
    GLenum glGetError();

    void glClear(GLbitfield);
    void glClearColor(GLclampf,GLclampf,GLclampf,GLclampf);
    void glClearDepth(GLclampd);
    void glClearStencil(GLint);
    void glFlush();
    void glViewport( GLint x, GLint y, GLint width, GLint height );
    void glDepthRange( GLclampd znear, GLclampd zfar );


    void glEnableClientState( GLenum a );
    void glDisableClientState( GLenum a );
    void glEnable( GLenum cap );
    void glDisable( GLenum cap );

    void glBlendFunc( GLenum src, GLenum dst );

    void glGenTextures( GLsizei n, GLuint* textures );
    void glBindTexture( GLenum target, GLuint texture );
    void glTexImage2D( GLenum target, GLint level, GLint components, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels );
    void glDeleteTextures( GLsizei n , const GLuint* textures );


  private:
    DllWrapper    m_OpenGLDLL;

		typedef HGLRC  (WINAPI*WGLCREATECONTEXT)(HDC);    WGLCREATECONTEXT m_wglCreateContext;
		typedef BOOL   (WINAPI*WGLDELETECONTEXT)(HGLRC);  WGLDELETECONTEXT m_wglDeleteContext;
		typedef BOOL   (WINAPI*WGLMAKECURRENT)(HDC,HGLRC);WGLMAKECURRENT   m_wglMakeCurrent;
		typedef PROC   (WINAPI*WGLGETPROCADDRESS)(LPCSTR);WGLGETPROCADDRESS   m_wglGetProcAddress;
		typedef HGLRC  (WINAPI*WGLGETCURRENTCONTEXT)();    WGLGETCURRENTCONTEXT m_wglGetCurrentContext;
		typedef HDC    (WINAPI*WGLGETCURRENTDC)();    WGLGETCURRENTDC m_wglGetCurrentDC;
		typedef BOOL   (WINAPI*WGLSHARELISTS)(HGLRC, HGLRC);    WGLSHARELISTS m_wglShareLists;


		typedef GLenum   (WINAPI*GLGETERROR)();GLGETERROR   m_glGetError;

		typedef void   (WINAPI*GLFLUSH)();GLFLUSH   m_glFlush;

		typedef void   (WINAPI*GLCLEAR)(GLbitfield);    GLCLEAR   m_glClear;
		typedef void   (WINAPI*GLCLEARCOLOR)(GLclampf,GLclampf,GLclampf,GLclampf);GLCLEARCOLOR   m_glClearColor;
		typedef void   (WINAPI*GLCLEARDEPTH)(GLclampd); GLCLEARDEPTH   m_glClearDepth;
		typedef void   (WINAPI*GLCLEARSTENCIL)(GLint);  GLCLEARSTENCIL   m_glClearStencil;
		typedef void   (WINAPI*GLVIEWPORT)(GLint,GLint,GLint,GLint);  GLVIEWPORT   m_glViewport;
		typedef void   (WINAPI*GLDEPTHRANGE)(GLclampd,GLclampd);  GLDEPTHRANGE   m_glDepthRange;

		typedef const GLubyte*   (WINAPI*GLGETSTRING)(GLenum);  GLGETSTRING   m_glGetString;

		typedef void   (WINAPI*GLENABLECLIENTSTATE)(GLenum);   GLENABLECLIENTSTATE   m_glEnableClientState;
		typedef void   (WINAPI*GLDISABLECLIENTSTATE)(GLenum);  GLDISABLECLIENTSTATE   m_glDisableClientState;

		typedef void   (WINAPI*GLENABLE)(GLenum);   GLENABLE   m_glEnable;
		typedef void   (WINAPI*GLDISABLE)(GLenum);  GLDISABLE  m_glDisable;

		typedef void   (WINAPI*GLBLENDFUNC)(GLenum,GLenum);  GLBLENDFUNC  m_glBlendFunc;

		typedef void   (WINAPI*GLGENTEXTURES)( GLsizei, GLuint*);  GLGENTEXTURES  m_glGenTextures;
		typedef void   (WINAPI*GLBINDTEXTURE)( GLenum, GLuint );  GLBINDTEXTURE  m_glBindTexture;
		typedef void   (WINAPI*GLTEXIMAGE2D)( GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid* );  GLTEXIMAGE2D  m_glTexImage2D;
		typedef void   (WINAPI*GLDELETETEXTURES)( GLsizei, const GLuint*);  GLDELETETEXTURES  m_glDeleteTextures;


  };

}}

#endif