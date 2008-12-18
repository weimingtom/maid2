#include"vertexshader.h"


namespace Maid
{

  VertexShader::STORAGEMAP  VertexShader::s_Storage;

  class VertexShaderCompiler : public IStorageFunction
  {
  public:
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

	void VertexShaderBase::DeleteFirst( const String& id  )
  {
    if( IVertexShader::Get().ID==Graphics::hObject::ID_NONE ) { return ; }

    const Graphics::SPDEVICE& pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();
    pDevice->DeleteObject( IVertexShader::Get() );
  }

	void VertexShaderBase::LoadEndFirst( const IStorageFunction* param, OPTION& option  )
  {
    const VertexShaderCompiler* p = static_cast<const VertexShaderCompiler*>(param);

    if( p->IsSucess() )
    {
      option.hObject = Graphics::hVertexShader();
    }else
    {
      option.hObject = p->hObject;
    }
    IVertexShader::Set( option.hObject );
  }

	void VertexShaderBase::LoadEnd( const OPTION& option  )
  {
    IVertexShader::Set( option.hObject );
  }

  IStorageFunction* VertexShaderBase::CreateFunction( const String& id )
  {
    VertexShaderCompiler* pRet = new VertexShaderCompiler;
    pRet->Text  = id;
    pRet->pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();
    return pRet;
  }

  String VertexShaderBase::ConvertID( const String& id )
  {
    return id; 
  }

}

