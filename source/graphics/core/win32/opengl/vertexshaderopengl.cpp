#include"vertexshaderopengl.h"


namespace Maid { namespace Graphics {

static const GLuint SHADEREMPTY = 0;

VertexShaderOpenGL::VertexShaderOpenGL()
  :m_ID(SHADEREMPTY)
{
}

VertexShaderOpenGL::~VertexShaderOpenGL()
{
  Finalize();
}

void VertexShaderOpenGL::Initialize( const void* pData, size_t length )
{
  Finalize();

  m_ID = s_pExt->glCreateShader(GL_VERTEX_SHADER);
  s_pExt->glShaderSource(m_ID, 1, (const GLchar**)&pData,NULL);
  s_pExt->glCompileShader(m_ID);//コンパイル

  //  シェーダコンパイルエラー表示
  {
    int logSize;
    int length;

    s_pExt->glGetShaderiv(m_ID, GL_INFO_LOG_LENGTH , &logSize);

    if (logSize > 1)
    {
      std::vector<char> s_logBuffer(logSize);
      s_pExt->glGetShaderInfoLog(m_ID, logSize, &length, &(s_logBuffer[0]) );

      MAID_WARNING( &(s_logBuffer[0]) );
    }

  }
}

void VertexShaderOpenGL::Finalize()
{
  if( m_ID==SHADEREMPTY ) { return ; }

	s_pExt->glDeleteShader( m_ID );
	m_ID = SHADEREMPTY;
}

GLuint VertexShaderOpenGL::GetID() const
{
  return m_ID;
}

}}

