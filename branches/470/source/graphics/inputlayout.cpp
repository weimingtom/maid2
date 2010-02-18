#include"inputlayout.h"


namespace Maid
{
  InputLayout::CACHE::INFOMAP  InputLayout::CACHE::s_InfoMap;
  ThreadMutex  InputLayout::CACHE::s_Mutex;

  void InputLayout::Create( const Graphics::INPUT_ELEMENT* Element, int Count, const String& ShaderCode )
  {
    IInputLayout::Clear();
    m_Element = std::vector<Graphics::INPUT_ELEMENT>(Element,Element+Count);
    m_Compiler.Compile( ShaderCode, ShaderCompiler::CODETYPE_VERTEXSHADER, GlobalPointer<GraphicsCore>::Get()->GetDevice() );
  }

  bool InputLayout::IsCompiling()const
  {
    //  ポインタが存在してるならすでに作ってる
    if( IInputLayout::Get().get()!=NULL ) { return false; }

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
      const_cast<InputLayout*>(this)->m_Cache.Start( KEEPOUT::ilInput(key, m_Element, byte, GlobalPointer<GraphicsCore>::Get()->GetDevice() ) );
    }

    if( m_Cache.IsExecuting() ) { return true; }

    const_cast<InputLayout*>(this)->IInputLayout::Set( m_Cache.GetOutput().pLayout );
    return false;
  }
}