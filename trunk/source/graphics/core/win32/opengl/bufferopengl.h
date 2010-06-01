#ifndef maid2_graphics_core_win32_opengl_bufferopengl_h
#define maid2_graphics_core_win32_opengl_bufferopengl_h


#include"../../../../config/define.h"

#include"../../ibuffer.h"
#include"common.h"

namespace Maid { namespace Graphics {

  class BufferOpenGL 
    : public IBuffer
    , public IOpenGLObject
  {
  public:
    BufferOpenGL( const CREATEBUFFERPARAM& param );
    ~BufferOpenGL();

    void Initialize();
    void Initialize( const void* pData, size_t length );
    void Finalize();

    GLuint GetID() const;

    void* Lock();
    void  UnLock();

  private:
    GLuint m_ID;
  };

  typedef	boost::shared_ptr<BufferOpenGL>	SPBUFFEROPENGL;

}}

#endif