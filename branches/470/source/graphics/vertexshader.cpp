#include"vertexshader.h"


namespace Maid
{
  VertexShader::CACHE::INFOMAP  VertexShader::CACHE::s_InfoMap;
  ThreadMutex  VertexShader::CACHE::s_Mutex;


  //! シェーダーを作成する
  /*!
      作成しますが文字列が"0100" - "0199" までの場合、デフォルトのシェーダーを作成する必要があります。

      @param  ShaderCode  [i ]  コンパイルするシェーダー
     
  */

  void VertexShader::Create( const String& ShaderCode )
  {
    IVertexShader::Clear();
    if( ShaderCode.empty() ) { return ; }
    m_Compiler.Compile( ShaderCode, ShaderCompiler::CODETYPE_VERTEXSHADER, GlobalPointer<GraphicsCore>::Get()->GetDevice() );
  }

  bool VertexShader::IsCompiling()const
  {
    //  ポインタが存在してるならすでに作ってる
    if( IVertexShader::Get().get()!=NULL ) { return false; }

    //  処理が終わるまでは忙しい
    if( m_Compiler.IsCompiling() ) { return true; }

    //  ここまできたらコンパイルが終わっている
    const std::vector<unt08>& code = m_Compiler.GetByteCode();
    if( code.empty() ) { return false; }

    //  次はシェーダー作成処理
    if( m_Cache.IsEmpty() )
    { //  m_Cache が動いてない==まだ作ってないなので、作成する
      const String& key = m_Compiler.GetShaderLanguage();
      const std::vector<unt08>& byte = m_Compiler.GetByteCode();
      const_cast<VertexShader*>(this)->m_Cache.Start( KEEPOUT::vsInput(key, byte, GlobalPointer<GraphicsCore>::Get()->GetDevice() ) );
    }

    if( m_Cache.IsExecuting() ) { return true; }

    const_cast<VertexShader*>(this)->IVertexShader::Set( m_Cache.GetOutput().pShader );
    return false;
  }

}

