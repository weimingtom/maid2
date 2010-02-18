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
#include"../auxiliary/string.h"

#include"graphicsobjecttemplate.h"
#include"graphicscore.h"
#include"shadercompiler.h"

namespace Maid
{
  namespace KEEPOUT
  {
    class vsInput : public IJobInput
    {
    public:
      vsInput(){}
      vsInput( const String& str, const std::vector<unt08>& code, const Graphics::SPDEVICE& p )
        :Key(str), ByteCode(code), pDevice(p){}
      String Key;
      std::vector<unt08> ByteCode;
      Graphics::SPDEVICE pDevice;
    };

    inline bool operator < ( const vsInput& lha, const vsInput& rha ) 
    {
      return lha.Key < rha.Key; 
    }

    class vsOutput : public IJobOutput
    {
    public:
      Graphics::SPVERTEXSHADER pShader;
    };

    class vsFunction : public IJobFunction
    {
    public:
      void Execute( const IJobInput& Input, IJobOutput& Output )
      {
        const vsInput&  in  = static_cast<const vsInput&>(Input);
        vsOutput& out = static_cast<vsOutput&>(Output);

        out.pShader = in.pDevice->CreateVertexShader(&(in.ByteCode[0]),in.ByteCode.size());
      }
    };
  }


  class VertexShader 
    :public IVertexShader
    ,private GlobalPointer<GraphicsCore>

  {
  public:
    void Create( const String& ShaderCode );
    bool IsCompiling()const;

  private:
    ShaderCompiler  m_Compiler;
    typedef JobCacheTemplate<KEEPOUT::vsInput,KEEPOUT::vsFunction, KEEPOUT::vsOutput> CACHE;
    CACHE m_Cache;
  };
}

#endif