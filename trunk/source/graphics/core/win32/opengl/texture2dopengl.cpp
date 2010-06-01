#include"texture2dopengl.h"


namespace Maid { namespace Graphics {

static const GLuint HANDLE_EMPTY = 0;

Texture2DOpenGL::Texture2DOpenGL( const CREATERETEXTURE2DPARAM& param )
  :ITexture2D(param)
  ,m_ID(HANDLE_EMPTY)
{

}

Texture2DOpenGL::~Texture2DOpenGL()
{
  Finalize();
}

void Texture2DOpenGL::Initialize(  const SUBRESOURCE* data )
{
  Finalize();

  s_pDLL->glGenTextures(1 , &m_ID);
  s_pDLL->glBindTexture(GL_TEXTURE_2D , m_ID);

  const CREATERETEXTURE2DPARAM& p = GetParam();
  const GLint fmt = PIXELFORMATtoInternalFormat(p.Format);
  const GLvoid* pixels = data==NULL? NULL:  data->pData;

  s_pDLL->glTexImage2D(GL_TEXTURE_2D,0,fmt, p.Size.w, p.Size.h, 0, fmt, GL_UNSIGNED_BYTE, pixels );

}

void Texture2DOpenGL::Finalize()
{
  if( m_ID==HANDLE_EMPTY ) { return ; }

	s_pDLL->glDeleteTextures( 1, &m_ID );
	m_ID = HANDLE_EMPTY;
}

GLuint Texture2DOpenGL::GetID() const
{
  return m_ID;
}



}}

