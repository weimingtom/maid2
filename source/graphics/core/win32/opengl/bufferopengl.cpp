#include"bufferopengl.h"

namespace Maid { namespace Graphics {

static const GLuint BUFFEREMPTY = 0;

BufferOpenGL::BufferOpenGL( const CREATEBUFFERPARAM& param )
  :IBuffer(param)
  ,m_ID(BUFFEREMPTY)
{
}

BufferOpenGL::~BufferOpenGL()
{
  Finalize();
}

void BufferOpenGL::Initialize()
{
  Finalize();

  const size_t len = IBuffer::GetParam().Length;

	s_pExt->glGenBuffers( 1, &m_ID );
	s_pExt->glBindBuffer( GL_ARRAY_BUFFER, m_ID );
	s_pExt->glBufferData( GL_ARRAY_BUFFER, len, NULL, GL_STATIC_DRAW );
}


void BufferOpenGL::Initialize( const void* pData, size_t length )
{
  Initialize();

  void* p = Lock();
  memcpy( p, pData, length );
  UnLock();

}

void BufferOpenGL::Finalize()
{
  if( m_ID!=BUFFEREMPTY )
  {
	  s_pExt->glDeleteBuffers( 1, &m_ID );
    m_ID = BUFFEREMPTY;
  }
}

GLuint BufferOpenGL::GetID() const
{
  return m_ID;
}


void* BufferOpenGL::Lock()
{
	s_pExt->glBindBuffer( GL_ARRAY_BUFFER, m_ID );
  return s_pExt->glMapBuffer( GL_ARRAY_BUFFER, GL_READ_WRITE );
}

void  BufferOpenGL::UnLock()
{
  const GLboolean ret = s_pExt->glUnmapBuffer( GL_ARRAY_BUFFER );
}




}}


