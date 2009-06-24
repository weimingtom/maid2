#ifndef maid2_graphics_shadercompiler_h
#define maid2_graphics_shadercompiler_h

/*!
    @file
    @brief  ShaderCompiler
 */


#include"../config/define.h"
#include"../auxiliary/debug/assert.h"
#include"../auxiliary/debug/warning.h"
#include"../auxiliary/jobcachetemplate.h"
#include"../auxiliary/string.h"

#include"core/idevice.h"

namespace Maid
{
  namespace KEEPOUT
  {
    class ShaderInput : public IJobInput
    {
    public:
      ShaderInput(){}
      ShaderInput( const String& str, const Graphics::IDevice::CODETYPE t, const Graphics::SPDEVICE& p )
        :ShaderLanguage(str), pCompiler(p),Type(t){}
      String ShaderLanguage;
      Graphics::SPDEVICE pCompiler;
      Graphics::IDevice::CODETYPE Type;
    };
    inline bool operator < ( const ShaderInput& lha, const ShaderInput& rha ) 
    {
	    if( lha.Type < rha.Type ) { return true;  }
	    else if( lha.Type > rha.Type ) { return false; }
      return lha.ShaderLanguage < rha.ShaderLanguage; 
    }

    class ShaderOutput : public IJobOutput
    {
    public:
      std::vector<unt08> ByteCode;
    };

    class ShaderFunction : public IJobFunction
    {
    public:
      void Execute( const IJobInput& Input, IJobOutput& Output );
    };
  }


  class ShaderCompiler
  {
  public:

    enum CODETYPE
    {
      CODETYPE_VERTEXSHADER = Graphics::IDevice::CODETYPE_VERTEXSHADER,
      CODETYPE_PIXELSHADER  = Graphics::IDevice::CODETYPE_PIXELSHADER,
    };

    void Compile( const String& ShaderLanguage, CODETYPE Type, const Graphics::SPDEVICE& pDevice );
    bool IsCompiling()const;

    const std::vector<unt08>& GetByteCode() const;
    const String& GetShaderLanguage() const;

  private:
    typedef JobCacheTemplate<KEEPOUT::ShaderInput,KEEPOUT::ShaderFunction, KEEPOUT::ShaderOutput> CACHE;
    CACHE m_Cache;
  };	
}

#endif