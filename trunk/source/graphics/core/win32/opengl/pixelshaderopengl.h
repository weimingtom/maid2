#ifndef maid2_graphics_core_win32_opengl_pixelshaderopengl_h
#define maid2_graphics_core_win32_opengl_pixelshaderopengl_h


#include"../../../../config/define.h"
#include"../../ipixelshader.h"
#include"common.h"


namespace Maid { namespace Graphics {

  class PixelShaderOpenGL 
    : public IPixelShader
		, public IOpenGLObject
  {
  public:
    PixelShaderOpenGL();
    ~PixelShaderOpenGL();

    void Initialize( const void* pData, size_t length );
    void Finalize();

    GLuint GetID() const;

  private:
    GLuint m_ID;

  };

  typedef	boost::shared_ptr<PixelShaderOpenGL>	SPPIXELSHADEROPENGL;

}}

#endif