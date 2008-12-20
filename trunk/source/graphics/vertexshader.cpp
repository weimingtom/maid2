#include"vertexshader.h"


namespace Maid
{
  VertexShader::VertexShader(){}

  VertexShader::VertexShader( const VertexShader& rha )
    :JobCacheTemplate(rha)
  {
    if( rha.m_ShaderCode.empty() ) { return; }
    Create(rha.m_ShaderCode); 
  }

  VertexShader::~VertexShader()
  {
    Reset();
  }

  VertexShader& VertexShader::operator = ( const VertexShader& rha )
  {
    Reset();
    if( !rha.m_ShaderCode.empty() )
    {
      Create(rha.m_ShaderCode);
    }
    return *this;
  }

  void VertexShader::Create( const String& ShaderCode )
  {
    m_ShaderCode = ShaderCode;
    Start( m_ShaderCode );
  }

  bool VertexShader::IsCompiling()const
  {
    if( m_ShaderCode.empty() ) { return false; }
    return IsExecuting();
  }





  VertexShader::INFOMAP  VertexShader::s_InfoMap;

  class VertexShaderJob : public IJobFunction
  {
  public:
    ~VertexShaderJob()
    {
      pDevice->DeleteObject(hObject);
    }

    //  入力
    String Text;
    Graphics::SPDEVICE pDevice;

    //  出力 
    Graphics::hVertexShader hObject;

    void Execute()
    {
      std::vector<unt08> Binary;
      String ErrorMSG;

      const bool IsSuccess = pDevice->CompileShaderLanguage(Text,Binary,ErrorMSG);

      if( !IsSuccess )
      {
        OnFaile(ErrorMSG);
        return ;
      }
      hObject = pDevice->CreateVertexShader(&(Binary[0]),Binary.size());
    }
  };

	void VertexShader::JobCopy( const VertexShaderJob& job )
  {
    IVertexShader::Set( job.hObject );
  }

  void VertexShader::JobClear( const VertexShaderJob& job )
  {
    IVertexShader::Clear();
  }

  void VertexShader::JobSetup( VertexShaderJob& job )
  {
    job.Text    = m_ShaderCode;
    job.pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();
  }


}

