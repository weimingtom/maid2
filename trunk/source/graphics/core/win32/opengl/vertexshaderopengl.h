#ifndef maid2_graphics_core_win32_opengl_vertexshaderopengl_h
#define maid2_graphics_core_win32_opengl_vertexshaderopengl_h


#include"../../../../config/define.h"
#include"../../ivertexshader.h"
#include"common.h"


namespace Maid { namespace Graphics {

  class VertexShaderOpenGL 
    : public IVertexShader
		, public IOpenGLObject
  {
  public:
    VertexShaderOpenGL();
    ~VertexShaderOpenGL();

    void Initialize( const void* pData, size_t length );
    void Finalize();

    GLuint GetID() const;

  private:
    GLuint m_ID;
  };

  typedef	boost::shared_ptr<VertexShaderOpenGL>	SPVERTEXSHADEROPENGL;

}}

#endif