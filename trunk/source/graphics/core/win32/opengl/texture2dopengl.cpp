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

  const CREATERETEXTURE2DPARAM& p = GetParam();
  const GLint fmt = PIXELFORMATtoInternalFormat(p.Format);
  const GLvoid* pixels = data==NULL? NULL:  data->pData;

  if( fmt==GL_DEPTH_COMPONENT )
  {
    s_pExt->glGenRenderbuffers( 1, &m_ID );
    s_pExt->glBindRenderbuffer( GL_RENDERBUFFER, m_ID );
    s_pExt->glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, p.Size.w, p.Size.h );
  }else
  {
    s_pDLL->glGenTextures(1 , &m_ID);
    s_pDLL->glBindTexture(GL_TEXTURE_2D , m_ID);
    s_pDLL->glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

    if( p.MipLevels<=1 )
    {
      s_pDLL->glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
      s_pDLL->glTexImage2D(GL_TEXTURE_2D,0,fmt, p.Size.w, p.Size.h, 0, fmt, GL_UNSIGNED_BYTE, pixels );
    }else
    {
      s_pDLL->glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);

      int w = p.Size.w;
      int h = p.Size.h;
      for( int i=0; i<p.MipLevels; ++i )
      {
        s_pDLL->glTexImage2D(GL_TEXTURE_2D,i,fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data[i].pData );
        w = std::max(1,w/2);
        h = std::max(1,h/2);
      }
    }

  }

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

