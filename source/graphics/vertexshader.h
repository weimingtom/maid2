#ifndef maid2_graphics_vertexshader_h
#define maid2_graphics_vertexshader_h

/*!
    @file
    @brief  vertexshader
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"
#include"../auxiliary/jobcachetemplate.h"

#include"graphicsobjecttemplate.h"
#include"core/core.h"

namespace Maid
{
  class VertexShaderJob;
  class VertexShader
      :public IVertexShader
      ,public GlobalPointer<Graphics::Core>
      ,public JobCacheTemplate<String,VertexShaderJob>
  {
  public:
    VertexShader();
    VertexShader( const VertexShader& rha );
    ~VertexShader();
    VertexShader& operator = ( const VertexShader& rha );

    void Create( const String& ShaderCode );
    bool IsCompiling()const;

  protected:
    void JobCopy( const VertexShaderJob& job );
    void JobClear( const VertexShaderJob& job );
    void JobSetup( VertexShaderJob& job );

  private:
    String  m_ShaderCode;
  };	
 
	
}

#endif