#ifndef maid2_graphics_core_win32_opengl_openglextension_h
#define maid2_graphics_core_win32_opengl_openglextension_h


#include"../../../../config/define.h"
#include"../../../../framework/win32/dllwrapper.h"

#include"opengl32dll.h"

#include"registry/gl3.h"
#include"registry/glext.h"

//  これを含めるとコンパイルエラーになる。　なぜ？
//#include"registry/glxext.h"

#include"registry/wglext.h"


#include<set>

namespace Maid { namespace Graphics {

  class OpenGLExtension
  {
  public:
    void Initialize( OpenGL32DLL& gldll );

    void glGenBuffers(GLsizei n, GLuint *buffers);
    void glBindBuffer(GLenum target, GLuint buffer);
    void glBufferData(GLenum target, int size, const GLvoid *data, GLenum usage);
    void glGetBufferParameteriv( GLenum target, GLenum value, GLint* data);
    void* glMapBuffer( GLenum target, GLenum access );
    GLboolean glUnmapBuffer( GLenum target );

    void glDeleteBuffers(GLsizei n, const GLuint *buffers);


    void glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
    void glNormalPointer( GLenum type, GLsizei stride, const GLvoid* pointer);
    void glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
    void glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
    void glActiveTexture(	GLenum texture );



    GLuint glCreateShader(GLenum type);
    void glShaderSource(GLuint shader, GLsizei count, const GLchar** string, const GLint* length );
    void glCompileShader(GLuint shader );
    void glDeleteShader(GLuint shader );
    void glGetShaderiv(	GLuint shader, GLenum pname, GLint* params );
    void glGetShaderInfoLog( GLuint shader,  GLsizei maxLength,	GLsizei* length, GLchar* infoLog );


    GLuint glCreateProgram();
    void  glAttachShader( GLuint program, GLuint shader );
    void  glDetachShader( GLuint program, GLuint shader );
    void  glLinkProgram( GLuint program );
    void  glDeleteProgram( GLuint program );

    void  glUseProgram( GLuint program );
    GLint glGetUniformLocation(	GLuint  	program, 	const GLchar * 	name);
    void glUniform1f(	GLint location,	GLfloat v0);
    void glUniform2f(	GLint location,	GLfloat v0,	GLfloat v1);
    void glUniform3f(	GLint location,	GLfloat v0,	GLfloat v1,	GLfloat v2);
    void glUniform4f(	GLint location,	GLfloat v0,	GLfloat v1,	GLfloat v2,	GLfloat v3);
    void glUniform1i(	GLint location,	GLint v0);
    void glUniform2i(	GLint location,	GLint v0,	GLint v1);
    void glUniform3i(	GLint location,	GLint v0,	GLint v1,	GLint v2);
    void glUniform4i(	GLint location,	GLint v0,	GLint v1,	GLint v2,	GLint v3);


    void   glDrawArrays( GLenum mode, GLint first, GLsizei count );

  private:
    bool IsExtensionSupported( const std::string& str ) const;
    std::set<std::string> m_IsExtentions;

  private:
    typedef void (APIENTRY * GLGENBUFFERS)    (GLsizei n, GLuint *buffers);        GLGENBUFFERS     m_glGenBuffers;
    typedef void (APIENTRY * GLBINDBUFFER)    (GLenum target, GLuint buffer);      GLBINDBUFFER     m_glBindBuffer;
    typedef void (APIENTRY * GLBUFFERDATA)    (GLenum target, int size, const GLvoid *data, GLenum usage);  GLBUFFERDATA m_glBufferData;
    typedef void (APIENTRY * GLGETBUFFERPARAMETERIV)    (GLenum target, GLenum value, GLint* data);  GLGETBUFFERPARAMETERIV m_glGetBufferParameteriv;
    typedef void*     (APIENTRY * GLMAPBUFFER)  (GLenum target, GLenum access);  GLMAPBUFFER     m_glMapBuffer;
    typedef GLboolean (APIENTRY * GLUNMAPBUFFER)(GLenum target);                 GLUNMAPBUFFER   m_glUnmapBuffer;

    typedef void (APIENTRY * GLDELETEBUFFERS) (GLsizei n, const GLuint *buffers);  GLDELETEBUFFERS  m_glDeleteBuffers;

    typedef void (APIENTRY * GLVERTEXPOINTER)  ( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);  GLVERTEXPOINTER m_glVertexPointer;
    typedef void (APIENTRY * GLNORMALPOINTER)  ( GLenum type, GLsizei stride, const GLvoid* pointer);              GLNORMALPOINTER m_glNormalPointer;
    typedef void (APIENTRY * GLTEXCOORDPOINTER)( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);  GLTEXCOORDPOINTER m_glTexCoordPointer;
    typedef void (APIENTRY * GLCOLORPOINTER)( GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);  GLCOLORPOINTER m_glColorPointer;



    typedef GLuint (APIENTRY * GLCREATESHADER)  (GLenum type);  GLCREATESHADER m_glCreateShader;
    typedef void (APIENTRY * GLSHADERSOURCE)    (GLuint shader, GLsizei count, const GLchar** string, const GLint* length );  GLSHADERSOURCE m_glShaderSource;
    typedef void (APIENTRY * GLCOMPILESHADER)   (GLuint shader );  GLCOMPILESHADER m_glCompileShader;
    typedef void (APIENTRY * GLDELETESHADER)   (GLuint shader );  GLDELETESHADER m_glDeleteShader;
    typedef void (APIENTRY * GLGETSHADERIV)       (	GLuint shader, GLenum pname, GLint* params );  GLGETSHADERIV m_glGetShaderiv;
    typedef void (APIENTRY * GLGETSHADERINFOLOG)  ( GLuint shader, GLsizei maxLength,	GLsizei* length, GLchar* infoLog );  GLGETSHADERINFOLOG m_glGetShaderInfoLog;


    typedef GLuint (APIENTRY * GLCREATEPROGRAM)();  GLCREATEPROGRAM m_glCreateProgram;
    typedef void (APIENTRY * GLATTACHSHADER)(GLuint,GLuint);  GLATTACHSHADER m_glAttachShader;
    typedef void (APIENTRY * GLDETACHSHADER)(GLuint,GLuint);  GLDETACHSHADER m_glDetachShader;
    typedef void (APIENTRY * GLLINKPROGRAM)   (GLuint program );  GLLINKPROGRAM m_glLinkProgram;
    typedef void (APIENTRY * GLDELETEPROGRAM)(GLuint);  GLDELETEPROGRAM m_glDeleteProgram;

    typedef void (APIENTRY * GLUSEPROGRAM)(GLuint);  GLUSEPROGRAM m_glUseProgram;
    typedef void (APIENTRY * GLDRAWARRAYS)( GLenum mode, GLint first, GLsizei count );  GLDRAWARRAYS m_glDrawArrays;

		typedef void (APIENTRY*GLACTIVETEXTURE)( GLenum texture);  GLACTIVETEXTURE  m_glActiveTexture;

		typedef GLint (APIENTRY*GLUNIFORMLOCATION)( GLuint program, const GLchar*	name);  GLUNIFORMLOCATION  m_glGetUniformLocation;
		typedef void (APIENTRY*GLUNIFORM1F)( GLint,	GLfloat);  GLUNIFORM1F  m_glUniform1f;
		typedef void (APIENTRY*GLUNIFORM2F)( GLint,	GLfloat, GLfloat);  GLUNIFORM2F  m_glUniform2f;
		typedef void (APIENTRY*GLUNIFORM3F)( GLint,	GLfloat, GLfloat, GLfloat);  GLUNIFORM3F  m_glUniform3f;
		typedef void (APIENTRY*GLUNIFORM4F)( GLint,	GLfloat, GLfloat, GLfloat, GLfloat);  GLUNIFORM4F  m_glUniform4f;
		typedef void (APIENTRY*GLUNIFORM1I)( GLint,	GLint);  GLUNIFORM1I  m_glUniform1i;
		typedef void (APIENTRY*GLUNIFORM2I)( GLint,	GLint, GLint);  GLUNIFORM2I  m_glUniform2i;
		typedef void (APIENTRY*GLUNIFORM3I)( GLint,	GLint, GLint, GLint);  GLUNIFORM3I  m_glUniform3i;
		typedef void (APIENTRY*GLUNIFORM4I)( GLint,	GLint, GLint, GLint, GLint);  GLUNIFORM4I  m_glUniform4i;


  };

}}

#endif