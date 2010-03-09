#include"shadercompiler.h"


namespace Maid
{
  /*!
      @class  ShaderCompiler shadercompiler.h
      @brief  各種シェーダーを別スレッドでコンパイル、キャッシュしてくれるクラス
  \n          コンパイルはデバイスの仕事なので、このクラスはデバイスが違っても変更しなくてすむ
  */

  ShaderCompiler::CACHE::INFOMAP  ShaderCompiler::CACHE::s_InfoMap;
  ThreadMutex  ShaderCompiler::CACHE::s_Mutex;

  namespace KEEPOUT
  {
    void ShaderFunction::Execute( const IJobInput& Input, IJobOutput& Output )
    {
        const ShaderInput&  in  = static_cast<const ShaderInput&>(Input);
        ShaderOutput& out = static_cast<ShaderOutput&>(Output);

        String ErrorMSG;

        const bool IsSuccess = in.pCompiler->CompileShaderLanguage(in.ShaderLanguage,in.Type, out.ByteCode,ErrorMSG);

        //  失敗したらエラー出す
        if( !IsSuccess )
        {
          MAID_WARNING( MAIDTEXT("シェーダーのコンパイルに失敗\n") << ErrorMSG << in.ShaderLanguage );
        }
    }
  }




  void ShaderCompiler::Compile( const String& ShaderLanguage, CODETYPE Type, const Graphics::SPDEVICE& pDevice )
  {
    m_Cache.Start( KEEPOUT::ShaderInput(ShaderLanguage,(Graphics::IDevice::CODETYPE)Type,pDevice) );
  }

  bool ShaderCompiler::IsCompiling()const
  {
    return m_Cache.IsExecuting();
  }

  const std::vector<unt08>& ShaderCompiler::GetByteCode() const
  {
    return m_Cache.GetOutput().ByteCode;
  }

  const String& ShaderCompiler::GetShaderLanguage() const
  {
    return m_Cache.GetInput().ShaderLanguage;
  }


}

