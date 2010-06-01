#include"openglextension.h"
#include"../../../../auxiliary/debug/warning.h"


namespace Maid { namespace Graphics {

void OpenGLExtension::Initialize( OpenGL32DLL& gldll )
{
  {
    const char* p = (const char*)gldll.glGetString(GL_EXTENSIONS);

    if( p!=NULL )
    {
      const char* pBegin = p;
      const char* pEnd = p;

      while( true )
      {
        if( *pEnd=='\0' )
        {
          const unt len = pEnd-pBegin;
          std::string s = std::string(pBegin,len);
          m_IsExtentions.insert( s );
          break;
        }

        if( *pEnd==' ' )
        {
          const unt len = pEnd-pBegin;
          std::string s = std::string(pBegin,len);
          m_IsExtentions.insert( s );
          pBegin = pEnd+1;
        }
        pEnd++;
      }
    }
  }

//  if( IsExtensionSupported("GL_ARB_vertex_buffer_object") )
//  各APIごとに↑の文字列みたいな判別ができるハズなんだけど、対応一覧表がサッパリわからないので無視
//  NULLかどうかでいいやね
//  GLEW http://glew.sourceforge.net/ から引っこ抜くのがいいかな
  {
    m_glGenBuffers    = (GLGENBUFFERS)gldll.wglGetProcAddress("glGenBuffers");
		if( m_glGenBuffers==NULL ) { MAID_WARNING( "glGenBuffers()が存在してません"); }
    m_glBindBuffer    = (GLBINDBUFFER)gldll.wglGetProcAddress("glBindBuffer");
		if( m_glBindBuffer==NULL ) { MAID_WARNING( "glBindBuffer()が存在してません"); }
    m_glBufferData    = (GLBUFFERDATA)gldll.wglGetProcAddress("glBufferData");
		if( m_glBufferData==NULL ) { MAID_WARNING( "glBufferData()が存在してません"); }
    m_glGetBufferParameteriv = (GLGETBUFFERPARAMETERIV)gldll.wglGetProcAddress("glGetBufferParameteriv");
		if( m_glGetBufferParameteriv==NULL ) { MAID_WARNING( "glGetBufferParameteriv()が存在してません"); }
    m_glMapBuffer = (GLMAPBUFFER)gldll.wglGetProcAddress("glMapBuffer");
		if( m_glMapBuffer==NULL ) { MAID_WARNING( "glMapBuffer()が存在してません"); }
    m_glUnmapBuffer = (GLUNMAPBUFFER)gldll.wglGetProcAddress("glUnmapBuffer");
		if( m_glUnmapBuffer==NULL ) { MAID_WARNING( "glUnmapBuffer()が存在してません"); }


    m_glDeleteBuffers = (GLDELETEBUFFERS)gldll.wglGetProcAddress("glDeleteBuffers");
		if( m_glDeleteBuffers==NULL ) { MAID_WARNING( "glDeleteBuffers()が存在してません"); }


    m_glVertexPointer = (GLVERTEXPOINTER)gldll.wglGetProcAddress("glVertexPointer");
		if( m_glVertexPointer==NULL ) { MAID_WARNING( "glVertexPointer()が存在してません"); }
    m_glNormalPointer = (GLNORMALPOINTER)gldll.wglGetProcAddress("glNormalPointer");
		if( m_glNormalPointer==NULL ) { MAID_WARNING( "glNormalPointer()が存在してません"); }
    m_glTexCoordPointer = (GLTEXCOORDPOINTER)gldll.wglGetProcAddress("glTexCoordPointer");
		if( m_glTexCoordPointer==NULL ) { MAID_WARNING( "glTexCoordPointer()が存在してません"); }
    m_glColorPointer = (GLCOLORPOINTER)gldll.wglGetProcAddress("glColorPointer");
		if( m_glColorPointer==NULL ) { MAID_WARNING( "glColorPointer()が存在してません"); }

    m_glActiveTexture = (GLACTIVETEXTURE)gldll.wglGetProcAddress("glActiveTexture");
		if( m_glActiveTexture==NULL ) { MAID_WARNING( "glActiveTexture()が存在してません"); }

  }

  {
    m_glCreateShader  = (GLCREATESHADER)gldll.wglGetProcAddress("glCreateShader");
		if( m_glCreateShader==NULL ) { MAID_WARNING( "glCreateShader()が存在してません"); }
    m_glShaderSource  = (GLSHADERSOURCE)gldll.wglGetProcAddress("glShaderSource");
		if( m_glShaderSource==NULL ) { MAID_WARNING( "glShaderSource()が存在してません"); }
    m_glCompileShader = (GLCOMPILESHADER)gldll.wglGetProcAddress("glCompileShader");
		if( m_glCompileShader==NULL ) { MAID_WARNING( "glCompileShader()が存在してません"); }
    m_glDeleteShader = (GLCOMPILESHADER)gldll.wglGetProcAddress("glDeleteShader");
		if( m_glDeleteShader==NULL ) { MAID_WARNING( "glDeleteShader()が存在してません"); }

    m_glGetShaderiv = (GLGETSHADERIV)gldll.wglGetProcAddress("glGetShaderiv");
		if( m_glGetShaderiv==NULL ) { MAID_WARNING( "glGetShaderiv()が存在してません"); }
    m_glGetShaderInfoLog = (GLGETSHADERINFOLOG)gldll.wglGetProcAddress("glGetShaderInfoLog");
		if( m_glGetShaderInfoLog==NULL ) { MAID_WARNING( "glGetShaderInfoLog()が存在してません"); }
  }
  {
    m_glCreateProgram = (GLCREATEPROGRAM)gldll.wglGetProcAddress("glCreateProgram");
		if( m_glCreateProgram==NULL ) { MAID_WARNING( "glCreateProgram()が存在してません"); }
    m_glAttachShader = (GLATTACHSHADER)gldll.wglGetProcAddress("glAttachShader");
		if( m_glAttachShader==NULL ) { MAID_WARNING( "glAttachShader()が存在してません"); }
    m_glDetachShader = (GLDETACHSHADER)gldll.wglGetProcAddress("glDetachShader");
		if( m_glDetachShader==NULL ) { MAID_WARNING( "glDetachShader()が存在してません"); }
    m_glLinkProgram = (GLLINKPROGRAM)gldll.wglGetProcAddress("glLinkProgram");
		if( m_glLinkProgram==NULL ) { MAID_WARNING( "glLinkProgram()が存在してません"); }
    m_glDeleteProgram = (GLDELETEPROGRAM)gldll.wglGetProcAddress("glDeleteProgram");
		if( m_glDeleteProgram==NULL ) { MAID_WARNING( "glDeleteProgram()が存在してません"); }
    m_glUseProgram = (GLUSEPROGRAM)gldll.wglGetProcAddress("glUseProgram");
		if( m_glUseProgram==NULL ) { MAID_WARNING( "glUseProgram()が存在してません"); }
  }

  {
    m_glDrawArrays = (GLDRAWARRAYS)gldll.wglGetProcAddress("glDrawArrays");
		if( m_glDrawArrays==NULL ) { MAID_WARNING( "glDrawArrays()が存在してません"); }
  }
  {
    m_glGetUniformLocation = (GLUNIFORMLOCATION)gldll.wglGetProcAddress("glGetUniformLocation");
		if( m_glGetUniformLocation==NULL ) { MAID_WARNING( "glGetUniformLocation()が存在してません"); }
    m_glUniform1f = (GLUNIFORM1F)gldll.wglGetProcAddress("glUniform1f");
		if( m_glUniform1f==NULL ) { MAID_WARNING( "glUniform1f()が存在してません"); }
    m_glUniform2f = (GLUNIFORM2F)gldll.wglGetProcAddress("glUniform2f");
		if( m_glUniform2f==NULL ) { MAID_WARNING( "glUniform2f()が存在してません"); }
    m_glUniform3f = (GLUNIFORM3F)gldll.wglGetProcAddress("glUniform3f");
		if( m_glUniform3f==NULL ) { MAID_WARNING( "glUniform3f()が存在してません"); }
    m_glUniform4f = (GLUNIFORM4F)gldll.wglGetProcAddress("glUniform4f");
		if( m_glUniform4f==NULL ) { MAID_WARNING( "glUniform4f()が存在してません"); }

    m_glUniform1i = (GLUNIFORM1I)gldll.wglGetProcAddress("glUniform1i");
		if( m_glUniform1i==NULL ) { MAID_WARNING( "glUniform1i()が存在してません"); }
    m_glUniform2i = (GLUNIFORM2I)gldll.wglGetProcAddress("glUniform2i");
		if( m_glUniform2i==NULL ) { MAID_WARNING( "glUniform2i()が存在してません"); }
    m_glUniform3i = (GLUNIFORM3I)gldll.wglGetProcAddress("glUniform3i");
		if( m_glUniform3i==NULL ) { MAID_WARNING( "glUniform3i()が存在してません"); }
    m_glUniform4i = (GLUNIFORM4I)gldll.wglGetProcAddress("glUniform4i");
		if( m_glUniform4i==NULL ) { MAID_WARNING( "glUniform4i()が存在してません"); }



  }
}

bool OpenGLExtension::IsExtensionSupported( const std::string& str ) const
{
  return m_IsExtentions.find(str)!=m_IsExtentions.end();
}



void OpenGLExtension::glGenBuffers(GLsizei n, GLuint *buffers)
{
  m_glGenBuffers( n, buffers );
}

void OpenGLExtension::glBindBuffer(GLenum target, GLuint buffer)
{
  m_glBindBuffer( target, buffer );
}

void OpenGLExtension::glBufferData(GLenum target, int size, const GLvoid *data, GLenum usage)
{
  m_glBufferData( target, size, data, usage );
}

void OpenGLExtension::glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
  m_glVertexPointer( size, type, stride, pointer );
}

void OpenGLExtension::glNormalPointer( GLenum type, GLsizei stride, const GLvoid* pointer)
{
  m_glNormalPointer( type, stride, pointer );
}

void OpenGLExtension::glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
  m_glTexCoordPointer( size, type, stride, pointer );
}

void OpenGLExtension::glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
  m_glColorPointer( size, type, stride, pointer );
}

void OpenGLExtension::glActiveTexture(	GLenum texture )
{
  m_glActiveTexture( texture );
}


void* OpenGLExtension::glMapBuffer( GLenum target, GLenum access )
{
  return m_glMapBuffer( target, access );
}

GLboolean OpenGLExtension::glUnmapBuffer( GLenum target )
{
  return m_glUnmapBuffer( target );
}

void OpenGLExtension::glDeleteBuffers(GLsizei n, const GLuint *buffers)
{
  m_glDeleteBuffers( n, buffers );
}




GLuint OpenGLExtension::glCreateShader( GLenum type)
{
  return m_glCreateShader( type );
}

void OpenGLExtension::glShaderSource( GLuint shader, GLsizei count, const GLchar** string, const GLint* length )
{
  m_glShaderSource( shader, count, string, length );
}

void OpenGLExtension::glCompileShader( GLuint shader )
{
  m_glCompileShader( shader );
}

void OpenGLExtension::glDeleteShader( GLuint shader )
{
  m_glDeleteShader( shader );
}

void OpenGLExtension::glGetShaderiv(	GLuint shader, GLenum pname, GLint* params )
{
  m_glGetShaderiv( shader, pname, params );
}

void OpenGLExtension::glGetShaderInfoLog( GLuint shader,  GLsizei maxLength,	GLsizei* length, GLchar* infoLog )
{
  m_glGetShaderInfoLog( shader, maxLength, length, infoLog );
}



GLuint OpenGLExtension::glCreateProgram()
{
  return m_glCreateProgram();
}

void OpenGLExtension::glAttachShader( GLuint program, GLuint shader )
{
  m_glAttachShader( program, shader );
}

void OpenGLExtension::glDetachShader( GLuint program, GLuint shader )
{
  m_glDetachShader( program, shader );
}

void OpenGLExtension::glLinkProgram( GLuint program )
{
  m_glLinkProgram( program );
}

void OpenGLExtension::glDeleteProgram( GLuint program )
{
  m_glDeleteProgram( program );
}

void OpenGLExtension::glUseProgram( GLuint program )
{
  m_glUseProgram( program );
}




void OpenGLExtension::glDrawArrays( GLenum mode, GLint first, GLsizei count )
{
  m_glDrawArrays( mode, first, count );
}

GLint OpenGLExtension::glGetUniformLocation(	GLuint  	program, 	const GLchar * 	name)
{
  return m_glGetUniformLocation(program,name);
}

void OpenGLExtension::glUniform1f(	GLint location,	GLfloat v0)
{
  m_glUniform1f( location, v0 );
}

void OpenGLExtension::glUniform2f(	GLint location,	GLfloat v0,	GLfloat v1)
{
  m_glUniform2f( location, v0, v1 );
}

void OpenGLExtension::glUniform3f(	GLint location,	GLfloat v0,	GLfloat v1,	GLfloat v2)
{
  m_glUniform3f( location, v0, v1, v2 );
}

void OpenGLExtension::glUniform4f(	GLint location,	GLfloat v0,	GLfloat v1,	GLfloat v2,	GLfloat v3)
{
  m_glUniform4f( location, v0, v1, v2, v3 );
}

void OpenGLExtension::glUniform1i(	GLint location,	GLint v0)
{
  m_glUniform1i( location, v0 );
}

void OpenGLExtension::glUniform2i(	GLint location,	GLint v0,	GLint v1)
{
  m_glUniform2i( location, v0, v1 );
}

void OpenGLExtension::glUniform3i(	GLint location,	GLint v0,	GLint v1,	GLint v2)
{
  m_glUniform3i( location, v0, v1, v2 );
}

void OpenGLExtension::glUniform4i(	GLint location,	GLint v0,	GLint v1,	GLint v2,	GLint v3)
{
  m_glUniform4i( location, v0, v1, v2, v3 );
}

}}

