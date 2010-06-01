#include"vertexshader.h"


namespace Maid
{


#if 0
  VertexShader::CACHE::INFOMAP  VertexShader::CACHE::s_InfoMap;


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
#endif

#if 0
  VertexShader::SHAREDDATA VertexShader::s_SharedData;

  VertexShader::~VertexShader()
  {
    Delete();
  }

  void VertexShader::Create( const String& ShaderCode )
  {
    Delete();
    if( ShaderCode.empty() ) { return ; }

    SHAREDDATA::iterator ite = s_SharedData.find( ShaderCode );
    if( ite!=s_SharedData.end() )
    {
      m_pShaderInfo = ite->second;
      IVertexShader::Set( m_pShaderInfo->pShader );
    }
    else
    {
      m_Compiler.Compile( ShaderCode, ShaderCompiler::CODETYPE_VERTEXSHADER, GlobalPointer<GraphicsCore>::Get()->GetDevice() );
    }

    m_ShaderCode = ShaderCode;
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

    VertexShader* pThis = const_cast<VertexShader*>(this);

    SPSHADERINFO pInfo;
    SHAREDDATA::iterator ite = s_SharedData.find( m_ShaderCode );
    if( ite==s_SharedData.end() )
    {
      //  次はシェーダー作成処理
      GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get(); 
      
      Graphics::SPVERTEXSHADER pShader = pCore->GetDevice()->CreateVertexShader(&(code[0]),code.size());

      if( pShader.get()==NULL ) { return false; }

      {
        SPSHADERINFO p( new SHADERINFO );

        p->pShader = pShader;

        pInfo = p;
        s_SharedData[m_ShaderCode] = p;
      }
    }else
    {
     pInfo = ite->second;
    }

    pThis->m_pShaderInfo = pInfo;
    pThis->IVertexShader::Set( m_pShaderInfo->pShader );
    return false;
  }

  void VertexShader::Delete()
  {
    if( m_ShaderCode.empty() ) { return ; }

    const String tmp = m_ShaderCode;
    {
      IVertexShader::Clear();
      m_pShaderInfo = SPSHADERINFO();
      m_Compiler = ShaderCompiler();
      m_ShaderCode = String();
    }

    SHAREDDATA::iterator ite = s_SharedData.find( tmp );
    if( ite!=s_SharedData.end() )
    {
      if( ite->second.unique() )
      {
        s_SharedData.erase(ite);
      }
    }
  }
#endif


  VertexShader::~VertexShader()
  {
    Delete();
  }

  void VertexShader::Create( const String& ShaderCode )
  {
    Delete();
    if( ShaderCode.empty() ) { return ; }

    if( m_Table.IsExist(ShaderCode) )
    {
      m_Table.Load( ShaderCode );
      IVertexShader::Set( m_Table.GetValue().pShader );
    }else
    {
      m_Compiler.Compile( ShaderCode, ShaderCompiler::CODETYPE_VERTEXSHADER, GlobalPointer<GraphicsCore>::Get()->GetDevice() );
    }
  }

  bool VertexShader::IsCompiling()const
  {
    //  ポインタが存在してるならすでに作ってる
    if( IVertexShader::Get().get()!=NULL ) { return false; }

    const String& shader_code = m_Compiler.GetShaderLanguage();
    if( shader_code.empty() ) { return false; }

    //  処理が終わるまでは忙しい
    if( m_Compiler.IsCompiling() ) { return true; }

    //  ここまできたらコンパイルが終わっている
    const std::vector<unt08>& code = m_Compiler.GetByteCode();
    if( code.empty() ) { return false; }

    VertexShader* pThis = const_cast<VertexShader*>(this);

    if( m_Table.IsExist(shader_code) )
    {
      pThis->m_Table.Load( shader_code );
    }else
    {
      GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get(); 
      
      Graphics::SPVERTEXSHADER pShader = pCore->GetDevice()->CreateVertexShader(&(code[0]),code.size());

      if( pShader.get()==NULL ) { return false; }

      SHADERINFO info;
      info.pShader = pShader;
      pThis->m_Table.Create( shader_code, info );
    }

    pThis->IVertexShader::Set( m_Table.GetValue().pShader );
    return false;
  }

  void VertexShader::Delete()
  {
    IVertexShader::Clear();
    m_Compiler = ShaderCompiler();
  }
}

