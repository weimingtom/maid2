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
      ShaderInput( const String& str, const Graphics::SPDEVICE& p )
        :ShaderLanguage(str), pCompiler(p){}
      String ShaderLanguage;
      Graphics::SPDEVICE pCompiler;
    };
    inline bool operator < ( const ShaderInput& lha, const ShaderInput& rha ) 
    {
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
      void Execute( const IJobInput& Input, IJobOutput& Output )
      {
        const ShaderInput&  in  = static_cast<const ShaderInput&>(Input);
        ShaderOutput& out = static_cast<ShaderOutput&>(Output);

        std::vector<unt08> Binary;
        String ErrorMSG;

        const bool IsSuccess = in.pCompiler->CompileShaderLanguage(in.ShaderLanguage,out.ByteCode,ErrorMSG);

        //  失敗したらエラー出す
        if( !IsSuccess ) { MAID_WARNING( MAIDTEXT("シェーダーのコンパイルに失敗\n") << ErrorMSG ); }
      }
    };
  }


  class ShaderCompiler
  {
  public:
    void Compile( const String& ShaderLanguage, const Graphics::SPDEVICE& pDevice );
    bool IsCompiling()const;

    const std::vector<unt08>& GetByteCode() const;
    const String& GetShaderLanguage() const;

  private:
    typedef JobCacheTemplate<KEEPOUT::ShaderInput,KEEPOUT::ShaderFunction, KEEPOUT::ShaderOutput> CACHE;
    CACHE m_Cache;
  };	
}

#endif