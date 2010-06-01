#include"pixelshader.h"


namespace Maid
{
  PixelShader::~PixelShader()
  {
    Delete();
  }

  void PixelShader::Create( const String& ShaderCode )
  {
    Delete();
    if( ShaderCode.empty() ) { return ; }

    if( m_Table.IsExist(ShaderCode) )
    {
      m_Table.Load( ShaderCode );
      IPixelShader::Set( m_Table.GetValue().pShader );
    }else
    {
      m_Compiler.Compile( ShaderCode, ShaderCompiler::CODETYPE_PIXELSHADER, GlobalPointer<GraphicsCore>::Get()->GetDevice() );
    }
  }

  bool PixelShader::IsCompiling()const
  {
    //  ポインタが存在してるならすでに作ってる
    if( IPixelShader::Get().get()!=NULL ) { return false; }

    const String& shader_code = m_Compiler.GetShaderLanguage();
    if( shader_code.empty() ) { return false; }

    //  処理が終わるまでは忙しい
    if( m_Compiler.IsCompiling() ) { return true; }

    //  ここまできたらコンパイルが終わっている
    const std::vector<unt08>& code = m_Compiler.GetByteCode();
    if( code.empty() ) { return false; }

    PixelShader* pThis = const_cast<PixelShader*>(this);

    if( m_Table.IsExist(shader_code) )
    {
      pThis->m_Table.Load( shader_code );
    }else
    {
      GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get(); 
      
      Graphics::SPPIXELSHADER pShader = pCore->GetDevice()->CreatePixelShader(&(code[0]),code.size());

      if( pShader.get()==NULL ) { return false; }

      SHADERINFO info;
      info.pShader = pShader;
      pThis->m_Table.Create( shader_code, info );
    }

    pThis->IPixelShader::Set( m_Table.GetValue().pShader );
    return false;
  }

  void PixelShader::Delete()
  {
    IPixelShader::Clear();
    m_Compiler = ShaderCompiler();
  }
}

