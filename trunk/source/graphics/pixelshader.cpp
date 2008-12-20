#include"pixelshader.h"


namespace Maid
{
  PixelShader::PixelShader(){}

  PixelShader::PixelShader( const PixelShader& rha )
    :JobCacheTemplate(rha)
  {
    if( rha.m_ShaderCode.empty() ) { return; }
    Create(rha.m_ShaderCode); 
  }

  PixelShader::~PixelShader()
  {
    Reset();
  }

  PixelShader& PixelShader::operator = ( const PixelShader& rha )
  {
    Reset();
    if( !rha.m_ShaderCode.empty() )
    {
      Create(rha.m_ShaderCode);
    }
    return *this;
  }

  void PixelShader::Create( const String& ShaderCode )
  {
    m_ShaderCode = ShaderCode;
    Start( m_ShaderCode );
  }

  bool PixelShader::IsCompiling()const
  {
    if( m_ShaderCode.empty() ) { return false; }
    return IsExecuting();
  }





  PixelShader::INFOMAP  PixelShader::s_InfoMap;

  class PixelShaderJob : public IJobFunction
  {
  public:
    ~PixelShaderJob()
    {
      pDevice->DeleteObject(hObject);
    }

    //  入力
    String Text;
    Graphics::SPDEVICE pDevice;

    //  出力 
    Graphics::hPixelShader hObject;

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
      hObject = pDevice->CreatePixelShader(&(Binary[0]),Binary.size());
    }
  };

	void PixelShader::JobCopy( const PixelShaderJob& job )
  {
    IPixelShader::Set( job.hObject );
  }

  void PixelShader::JobClear( const PixelShaderJob& job )
  {
    IPixelShader::Clear();
  }

  void PixelShader::JobSetup( PixelShaderJob& job )
  {
    job.Text    = m_ShaderCode;
    job.pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();
  }

}

