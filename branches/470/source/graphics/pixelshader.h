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
#include"graphicscore.h"
#include"shadercompiler.h"

namespace Maid
{
  namespace KEEPOUT
  {
    class psInput : public IJobInput
    {
    public:
      psInput(){}
      psInput( const String& str, const std::vector<unt08>& code, const Graphics::SPDEVICE& p )
        :Key(str), ByteCode(code), pDevice(p){}
      String Key;
      std::vector<unt08> ByteCode;
      Graphics::SPDEVICE pDevice;
    };

    inline bool operator < ( const psInput& lha, const psInput& rha ) 
    {
      return lha.Key < rha.Key; 
    }

    class psOutput : public IJobOutput
    {
    public:
      Graphics::SPPIXELSHADER pShader;
    };

    class psFunction : public IJobFunction
    {
    public:
      void Execute( const IJobInput& Input, IJobOutput& Output )
      {
        const psInput&  in  = static_cast<const psInput&>(Input);
        psOutput& out = static_cast<psOutput&>(Output);

        out.pShader = in.pDevice->CreatePixelShader(&(in.ByteCode[0]),in.ByteCode.size());
      }
    };
  }


  class PixelShader 
    :public IPixelShader
    ,private GlobalPointer<GraphicsCore>

  {
  public:
    void Create( const String& ShaderCode );
    bool IsCompiling()const;

  private:
    ShaderCompiler  m_Compiler;
    typedef JobCacheTemplate<KEEPOUT::psInput,KEEPOUT::psFunction, KEEPOUT::psOutput> CACHE;
    CACHE m_Cache;
  }; 
	
}


#endif