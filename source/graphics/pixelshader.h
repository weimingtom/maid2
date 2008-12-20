#ifndef maid2_graphics_pixelshader_h
#define maid2_graphics_pixelshader_h

/*!
    @file
    @brief  pixelshader
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/globalpointer.h"
#include"../auxiliary/jobcachetemplate.h"

#include"graphicsobjecttemplate.h"
#include"core/core.h"

namespace Maid
{
  class PixelShaderJob;
  class PixelShader
      :public IPixelShader
      ,public GlobalPointer<Graphics::Core>
      ,public JobCacheTemplate<String,PixelShaderJob>
  {
  public:
    PixelShader();
    PixelShader( const PixelShader& rha );
    ~PixelShader();
    PixelShader& operator = ( const PixelShader& rha );

    void Create( const String& ShaderCode );
    bool IsCompiling()const;

  protected:
    void JobCopy( const PixelShaderJob& job );
    void JobClear( const PixelShaderJob& job );
    void JobSetup( PixelShaderJob& job );

  private:
    String  m_ShaderCode;
  };	
 
	
}


#endif