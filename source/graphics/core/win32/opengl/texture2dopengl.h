#ifndef maid2_graphics_core_win32_opengl_texture2dopengl_h
#define maid2_graphics_core_win32_opengl_texture2dopengl_h


#include"../../../../config/define.h"
#include"../../itexture2d.h"

#include"common.h"


namespace Maid { namespace Graphics {

  class Texture2DOpenGL 
    : public ITexture2D
		, public IOpenGLObject
  {
  public:
    Texture2DOpenGL( const CREATERETEXTURE2DPARAM& param );
    virtual ~Texture2DOpenGL();

    void Initialize(  const SUBRESOURCE* data );
    void Finalize();

    GLuint GetID() const;

  private:
    GLuint  m_ID;
  };

}}

#endif