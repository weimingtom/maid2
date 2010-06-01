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
#include"../auxiliary/globalkeyvaluetable.h"

#include"graphicsobjecttemplate.h"
#include"graphicscore.h"
#include"shadercompiler.h"

namespace Maid
{

#if 0
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
#endif

#if 0
  class PixelShader
    :public IPixelShader
    ,private GlobalPointer<GraphicsCore>
  {
  public:
    virtual ~PixelShader();
    void Create( const String& ShaderCode );
    bool IsCompiling()const;

  private:
    void Delete();

    String          m_ShaderCode;
    ShaderCompiler  m_Compiler;

    struct SHADERINFO
    {
      Graphics::SPPIXELSHADER pShader;
    };
    typedef	boost::shared_ptr<SHADERINFO>	SPSHADERINFO;

    SPSHADERINFO  m_pShaderInfo;

    typedef std::map<String,SPSHADERINFO> SHAREDDATA;
    static SHAREDDATA s_SharedData;
  };
#endif

  class PixelShader
    :public IPixelShader
    ,private GlobalPointer<GraphicsCore>
  {
  public:
    virtual ~PixelShader();
    void Create( const String& ShaderCode );
    bool IsCompiling()const;

  private:
    void Delete();

    ShaderCompiler  m_Compiler;

    struct SHADERINFO
    {
      Graphics::SPPIXELSHADER pShader;
    };

    GlobalKeyValueTable<String,SHADERINFO> m_Table;
  };
}


#endif