#include"deviced3d10_0.h"

#include"bufferd3d10.h"
#include"texture2dd3d10.h"
#include"rendertargetd3d10.h"
#include"depthstencild3d10.h"
#include"materiald3d10.h"
#include"inputlayoutd3d10.h"
#include"rasterizerstated3d10.h"
#include"samplerstated3d10.h"
#include"depthstencilstated3d10.h"
#include"blendstated3d10.h"
#include"vertexshaderd3d10.h"
#include"pixelshaderd3d10.h"


#include"../../../../auxiliary/debug/trace.h"
#include"debug.h"


namespace Maid { namespace Graphics {


SPBUFFER DeviceD3D10_0::CreateBuffer( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data )
{
  D3D10_BUFFER_DESC dec;
  {
    dec.ByteWidth = param.Length;
    dec.Usage = RESOURCEUSAGEtoD3D10_USAGE(param.Usage);
    dec.BindFlags = param.BindFlags;
    dec.CPUAccessFlags = param.CPUAccessFlags;
    dec.MiscFlags      = param.MiscFlags;
  }


  D3D10_SUBRESOURCE_DATA dat;
  D3D10_SUBRESOURCE_DATA* ptr_dat = NULL;
  if( data!=NULL )
  {
    ptr_dat = &dat;
    dat.pSysMem = data->pData;
    dat.SysMemPitch      = data->Pitch;
    dat.SysMemSlicePitch = data->Slice;
  }
 
  SPD3D10BUFFER pBuffer;
  {
    ID3D10Buffer* p = NULL;
    const HRESULT ret = m_pDevice->CreateBuffer( &dec, ptr_dat, &p );
    if( FAILED(ret) ) { MAID_WARNING("ID3D10Device::CreateBuffer " << ret ); }
    pBuffer.reset(p);
  }

  return SPBUFFER( new BufferD3D10(param,pBuffer) );
}


SPTEXTURE2D DeviceD3D10_0::CreateTexture2D( const CREATERETEXTURE2DPARAM& param, const SUBRESOURCE* data )
{
  D3D10_TEXTURE2D_DESC dec;
  {
    dec.Width  = param.Size.w;
    dec.Height = param.Size.h;
    dec.MipLevels = param.MipLevels;
    dec.ArraySize = param.ArraySize;
    dec.Format = PIXELFORMATtoDXGI_FORMAT(param.Format);
    dec.SampleDesc.Count = param.Sample.Count;
    dec.SampleDesc.Quality = param.Sample.Quality;
    dec.Usage = RESOURCEUSAGEtoD3D10_USAGE(param.Usage);

    dec.BindFlags = param.BindFlags;
    dec.CPUAccessFlags = param.CPUAccessFlags;
    dec.MiscFlags      = param.MiscFlags;
  }


  std::vector<D3D10_SUBRESOURCE_DATA> dat;
  D3D10_SUBRESOURCE_DATA* ptr_dat = NULL;

  if( data!=NULL )
  {
    dat.resize( param.MipLevels );

    for( int i=0; i<param.MipLevels; ++i )
    {
      dat[i].pSysMem = data[i].pData;
      dat[i].SysMemPitch      = data[i].Pitch;
      dat[i].SysMemSlicePitch = data[i].Slice;
    }

    ptr_dat = &(dat[0]);
  }

  SPD3D10TEXTURE2D pBuffer;
  {
    ID3D10Texture2D* p = NULL;
    const HRESULT ret = m_pDevice->CreateTexture2D( &dec, ptr_dat, &p );
    if( FAILED(ret) ) 
    {
      MAID_WARNING("ID3D10Device::CreateTexture2D " << ret << " " << DebugStringD3D10(dec) << " " << DebugStringD3D10(ptr_dat) );
      return SPTEXTURE2D();
    }

    pBuffer.reset(p);
  }

  return SPTEXTURE2D( new Texture2DD3D10(param,pBuffer) );
}

SPRENDERTARGET DeviceD3D10_0::CreateRenderTarget( const SPRESOURCE& resource, const CREATERENDERTARGETPARAM* param )
{
  ID3D10Resource* pResource = NULL;
  {
    switch( resource->GetType() )
    {
    case IResource::TYPE_BUFFER:    { pResource = static_cast<BufferD3D10*   >(resource.get())->pBuffer.get();  }break;
    case IResource::TYPE_TEXTURE2D: { pResource = static_cast<Texture2DD3D10*>(resource.get())->pTexture.get(); }break;
    }
  }

  D3D10_RENDER_TARGET_VIEW_DESC* p_desc=NULL;
  D3D10_RENDER_TARGET_VIEW_DESC desc;

  if( param!=NULL )
  {
    ZERO( &desc, sizeof(desc) );
    p_desc = &desc;
    desc.Format = PIXELFORMATtoDXGI_FORMAT(param->Format);
    desc.ViewDimension = RENDERTARGETDIMENSIONtoD3D10_RTV_DIMENSION(param->Dimension);
  }

  SPD3D10RENDERTARGETVIEW pView;
  {
    ID3D10RenderTargetView* p = NULL;
    const HRESULT ret = m_pDevice->CreateRenderTargetView( pResource, p_desc, &p );
    if( FAILED(ret) ) { MAID_WARNING("ID3D10Device::CreateRenderTargetView " << ret ); }
    pView.reset(p);
  }

  CREATERENDERTARGETPARAM result;
  {
    D3D10_RENDER_TARGET_VIEW_DESC dat;
    pView->GetDesc( &dat );

    result.Format = DXGI_FORMATtoPIXELFORMAT(dat.Format);
    result.Dimension = D3D10_RTV_DIMENSIONtoRENDERTARGETDIMENSION(dat.ViewDimension);

  }
  return SPRENDERTARGET( new RenderTargetD3D10(resource, result, pView ) );
}

SPDEPTHSTENCIL DeviceD3D10_0::CreateDepthStencil( const SPRESOURCE& resource, const CREATEDEPTHSTENCILPARAM* param )
{
  ID3D10Resource* pResource = NULL;
  {
    switch( resource->GetType() )
    {
    case IResource::TYPE_BUFFER:    { pResource = static_cast<BufferD3D10*   >(resource.get())->pBuffer.get();  }break;
    case IResource::TYPE_TEXTURE2D: { pResource = static_cast<Texture2DD3D10*>(resource.get())->pTexture.get(); }break;
    }
  }

  D3D10_DEPTH_STENCIL_VIEW_DESC* p_desc=NULL;
  D3D10_DEPTH_STENCIL_VIEW_DESC desc;

  if( param!=NULL )
  {
    p_desc = &desc;
    ZERO( &desc, sizeof(desc) );
    desc.Format = PIXELFORMATtoDXGI_FORMAT(param->Format);
    desc.ViewDimension = DEPTHSTENCILDIMENSIONtoD3D10_DSV_DIMENSION(param->Dimension);

    #pragma COMPILERMSG("desc.union はめんどいのでスルーする")	
  }

  SPD3D10DEPTHSTENCILVIEW pView;
  {
    ID3D10DepthStencilView* p = NULL;
    const HRESULT ret = m_pDevice->CreateDepthStencilView( pResource, p_desc, &p );
    if( FAILED(ret) ) { MAID_WARNING("ID3D10Device::CreateDepthStencilView " << ret ); }
    pView.reset(p);
  }

  CREATEDEPTHSTENCILPARAM result;
  {
    D3D10_DEPTH_STENCIL_VIEW_DESC dat;
    pView->GetDesc( &dat );

    result.Format = DXGI_FORMATtoPIXELFORMAT(dat.Format);
    result.Dimension = D3D10_DSV_DIMENSIONtoDEPTHSTENCILDIMENSION(dat.ViewDimension);
  }

  return SPDEPTHSTENCIL( new DepthStencilD3D10(resource, result, pView ) );
}

SPMATERIAL DeviceD3D10_0::CreateMaterial( const SPRESOURCE& resource, const CREATEMATERIALPARAM* param )
{
  ID3D10Resource* pResource = NULL;
  {
    switch( resource->GetType() )
    {
    case IResource::TYPE_BUFFER:    { pResource = static_cast<BufferD3D10*   >(resource.get())->pBuffer.get();  }break;
    case IResource::TYPE_TEXTURE2D: { pResource = static_cast<Texture2DD3D10*>(resource.get())->pTexture.get(); }break;
    }
  }

  D3D10_SHADER_RESOURCE_VIEW_DESC* p_desc=NULL;
  D3D10_SHADER_RESOURCE_VIEW_DESC desc;

  if( param!=NULL )
  {
    p_desc = &desc;
    ZERO( &desc, sizeof(desc) );

    desc.Format = PIXELFORMATtoDXGI_FORMAT(param->Format);
    desc.ViewDimension = MATERIALDIMENSIONtoD3D10_SRV_DIMENSION(param->Dimension);

    #pragma COMPILERMSG("desc.union はめんどいのでスルーする")	
  }

  SPD3D10SHADERRESOURCEVIEW pView;
  {
    ID3D10ShaderResourceView* p = NULL;
    const HRESULT ret = m_pDevice->CreateShaderResourceView( pResource, p_desc, &p );
    if( FAILED(ret) ) { MAID_WARNING("ID3D10Device::CreateShaderResourceView " << ret ); }

    pView.reset(p);
  }

  CREATEMATERIALPARAM result;
  {
    D3D10_SHADER_RESOURCE_VIEW_DESC dat;
    pView->GetDesc( &dat );

    result.Format = DXGI_FORMATtoPIXELFORMAT(dat.Format);
    result.Dimension = D3D10_SRV_DIMENSION_MATERIALDIMENSION(dat.ViewDimension);
  }

  return SPMATERIAL( new MaterialD3D10(resource, result, pView ) );
}




SPVERTEXSHADER DeviceD3D10_0::CreateVertexShader( const void* pData, size_t Length )
{
  SPD3D10VERTEXSHADER pShader;
  {
    ID3D10VertexShader* p = NULL;
    const HRESULT ret = m_pDevice->CreateVertexShader( pData, Length, &p );
    if( FAILED(ret) ) { MAID_WARNING("ID3D10Device::CreateVertexShader " << ret << " " << Length ); }
    pShader.reset(p);
  }

  return SPVERTEXSHADER( new VertexShaderD3D10(pShader) );
}

SPPIXELSHADER DeviceD3D10_0::CreatePixelShader( const void* pData, size_t Length )
{
  SPD3D10PIXELSHADER pShader;
  {
    ID3D10PixelShader* p = NULL;
    const HRESULT ret = m_pDevice->CreatePixelShader( pData, Length, &p );
    if( FAILED(ret) ) { MAID_WARNING("ID3D10Device::CreatePixelShader " << ret << " " << Length ); }
    pShader.reset(p);
  }

  return SPPIXELSHADER( new PixelShaderD3D10(pShader) );
}


SPSAMPLERSTATE DeviceD3D10_0::CreateSamplerState( const SAMPLERSTATEPARAM& Option )
{
  D3D10_SAMPLER_DESC desc = {};

  desc.Filter = FILTERtoD3D10_FILTER(Option.MinFilter,Option.MagFilter,Option.MipFilter);

  desc.AddressU = ADDRESStoD3D10_TEXTURE_ADDRESS_MODE(Option.AddressU);
  desc.AddressV = ADDRESStoD3D10_TEXTURE_ADDRESS_MODE(Option.AddressV);
  desc.AddressW = ADDRESStoD3D10_TEXTURE_ADDRESS_MODE(Option.AddressW);

  desc.MipLODBias = Option.MipLODBias;
  desc.MaxAnisotropy = Option.MaxAnisotropy;
  desc.ComparisonFunc = CMPTYPEtoD3D10_COMPARISON_FUNC(Option.ComparisonFunc);
  desc.MinLOD = Option.MinLOD;
  desc.MaxLOD = Option.MaxLOD;

  desc.BorderColor[0] = Option.BorderColor.GetR();
  desc.BorderColor[1] = Option.BorderColor.GetG();
  desc.BorderColor[2] = Option.BorderColor.GetB();
  desc.BorderColor[3] = Option.BorderColor.GetA();

  SPD3D10SAMPLERSTATE pState;
  {
    ID3D10SamplerState* p = NULL;
    const HRESULT ret = m_pDevice->CreateSamplerState( &desc, &p );
    if( FAILED(ret) ) { MAID_WARNING("ID3D10Device::CreateSamplerState " << ret ); }
    pState.reset(p);
  }

  return SPSAMPLERSTATE( new SamplerStateD3D10(Option,pState) );
}

SPRASTERIZERSTATE DeviceD3D10_0::CreateRasterizerState( const RASTERIZERSTATEPARAM& Option )
{
  D3D10_RASTERIZER_DESC desc = {};

  desc.FillMode = FILLtoD3D10_FILL_MODE(Option.Fill );
  desc.CullMode = CULLINGtoD3D10_CULL_MODE(Option.Culling );

  desc.MultisampleEnable = booltoD3D10BOOL(Option.MultiSample );

  SPD3D10RASTERIZERSTATE pState;
  {
    ID3D10RasterizerState* p = NULL;
    const HRESULT ret = m_pDevice->CreateRasterizerState( &desc, &p );
    if( FAILED(ret) ) { MAID_WARNING("ID3D10Device::CreateRasterizerState " << ret ); }
    pState.reset(p);
  }

  return SPRASTERIZERSTATE( new RasterizerStateD3D10(Option,pState) );
}

SPDEPTHSTENCILSTATE DeviceD3D10_0::CreateDepthStencilState( const DEPTHSTENCILSTATEPARAM& Option )
{
  D3D10_DEPTH_STENCIL_DESC desc = {};

	ZeroMemory( &desc, sizeof( desc ) );

  desc.DepthEnable = booltoD3D10BOOL( Option.DepthTest );
  desc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
  desc.DepthFunc = CMPTYPEtoD3D10_COMPARISON_FUNC(Option.DepthFunc);

  desc.StencilEnable = booltoD3D10BOOL( Option.StencilTest );
  desc.StencilReadMask  = D3D10_DEFAULT_STENCIL_READ_MASK;
  desc.StencilWriteMask = D3D10_DEFAULT_STENCIL_WRITE_MASK;

  desc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
  desc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
  desc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
  desc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

  desc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
  desc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_KEEP;
  desc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
  desc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;


  SPD3D10DEPTHSTENCILSTATE pState;
  {
    ID3D10DepthStencilState* p = NULL;
    const HRESULT ret = m_pDevice->CreateDepthStencilState( &desc, &p );
    if( FAILED(ret) ) { MAID_WARNING("ID3D10Device::CreateDepthStencilState " << ret ); }
    pState.reset(p);
  }

  return SPDEPTHSTENCILSTATE( new DepthStencilStateD3D10(pState) );
}


SPBLENDSTATE DeviceD3D10_0::CreateBlendState( const BLENDSTATEPARAM& Option )
{
  D3D10_BLEND_DESC desc = {};
  ZeroMemory( &desc, sizeof( desc ) );

  desc.AlphaToCoverageEnable = booltoD3D10BOOL(Option.AlphaToCoverageEnable);

  for( int i=0; i<NUMELEMENTS(desc.BlendEnable); ++i )
  {
    desc.BlendEnable[i] = booltoD3D10BOOL(Option.Target[i].Enable);
    desc.RenderTargetWriteMask[i] = Option.Target[i].WriteMask;
  }

  desc.SrcBlend = TARGETBLENDtoD3D10_BLEND(Option.Target[0].ColorSrc);
  desc.DestBlend = TARGETBLENDtoD3D10_BLEND(Option.Target[0].ColorDst);

  desc.BlendOp = TARGETOPERATIONtoD3D10_BLEND_OP(Option.Target[0].ColorOp);

  desc.SrcBlendAlpha = TARGETBLENDtoD3D10_BLEND(Option.Target[0].AlphaSrc);
  desc.DestBlendAlpha = TARGETBLENDtoD3D10_BLEND(Option.Target[0].AlphaDst);
  desc.BlendOpAlpha = TARGETOPERATIONtoD3D10_BLEND_OP(Option.Target[0].AlphaOp);


  SPD3D10BLENDSTATE pState;
  {
    ID3D10BlendState* p = NULL;
    const HRESULT ret = m_pDevice->CreateBlendState( &desc, &p );
    if( FAILED(ret) ) { MAID_WARNING("ID3D10Device::CreateBlendState " << ret << " " << DebugStringD3D10(desc) ); }
    pState.reset(p);
  }

  return SPBLENDSTATE( new BlendStateD3D10(Option,pState) );
}

SPINPUTLAYOUT DeviceD3D10_0::CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength )
{
  std::vector<D3D10_INPUT_ELEMENT_DESC> desc(Count);

  for( int i=0; i<Count; ++i )
  {
    D3D10_INPUT_ELEMENT_DESC& dst = desc[i];
    const INPUT_ELEMENT& src = Element[i];

    dst.SemanticName = src.SemanticName.c_str();
    dst.SemanticIndex = src.SemanticIndex;
    dst.Format = INPUT_ELEMENTTYPEtoDXGI_FORMAT(src.Type);
    dst.InputSlot = src.SlotNo;
    dst.AlignedByteOffset = src.Offset;
    dst.InputSlotClass  = D3D10_INPUT_PER_VERTEX_DATA ;
    dst.InstanceDataStepRate = 0;
  }

  SPD3D10INPUTLAYOUT pLayout;
  {
    ID3D10InputLayout* p = NULL;
    const HRESULT ret = m_pDevice->CreateInputLayout( &(desc[0]), Count, pShaderBytecodeWithInputSignature, BytecodeLength, &p  );
    if( FAILED(ret) ) { MAID_WARNING("ID3D10Device::CreateInputLayout " << ret ); }
    pLayout.reset(p);
  }


  return SPINPUTLAYOUT( new InputLayoutD3D10(pLayout) );
}

bool DeviceD3D10_0::CompileShaderLanguage( const String& Source, CODETYPE type, std::vector<unt08>& Binary, String& ErrorMessage )
{
  const std::string SrcData = String::ConvertMAIDtoSJIS(Source);

  //  固定機能を使う場合は言語サイズが4バイト以下かつ
  //  全部数字であることが条件なのでそれを元に作る
  //  いろいろしらべたけど、シェーダーバイナリの最小は32byteっぽいので、これなら自由にやれるはず
  if( SrcData.length()<=4 )
  {
    const unt32 no = atoi( SrcData.c_str() );

    if( 100<=no && no<=199 )
    {
      //  vertex shader はここで作る
      CreateDefaultVertexShader( no, Binary );
      return true;
    }
    else if( 200<=no && no<=299 ) 
    {
      //  PixelShader の場合は CreatePixelShader で作ってもらう
      CreateDefaultPixelShader( no, Binary );
      return true;
    }
    else { MAID_ASSERT( true, "このデバイスでは対応していません" ); }
  }

  //  そうでない場合は普通にやる
  ID3D10Blob*  pShader=NULL;
  ID3D10Blob*  pErrorMsgs=NULL;
  HRESULT ret = S_OK;

  std::vector<std::string>  Profile;
  {
    switch( type )
    {
    case CODETYPE_VERTEXSHADER:
      {
//        Profile.push_back("vs_2_0");
//        Profile.push_back("vs_2_x");
//        Profile.push_back("vs_3_0");
        Profile.push_back("vs_4_0");
      }break;
    case CODETYPE_PIXELSHADER: 
      {
//        Profile.push_back("ps_2_0");
//        Profile.push_back("ps_2_x");
//        Profile.push_back("ps_3_0");
        Profile.push_back("ps_4_0");
      }break;
    }
  }

  for( int i=0; i<(int)Profile.size(); ++i )
  {
    ret = m_ShaderCompilerLast(
      SrcData.c_str(),
      SrcData.length(),
      NULL,
      NULL,
      NULL,
      "main",
      Profile[i].c_str(),
      0,
      0,
      NULL,
      &pShader,
      &pErrorMsgs,
      NULL
      );

    if( FAILED(ret) )
    {
      std::string str = (char*)pErrorMsgs->GetBufferPointer();
      pErrorMsgs->Release();
    }
    if( SUCCEEDED(ret) ) { break; }
  }

  if( FAILED(ret) ) 
  {
    std::string str = (char*)pErrorMsgs->GetBufferPointer();
    ErrorMessage    = String::ConvertSJIStoMAID(str);
    pErrorMsgs->Release();
    goto MAID_ERROR;  
  }

/*  //  逆アセ用コメント
  {
    LPD3DXBUFFER pDis;

    HRESULT a = D3DXDisassembleShader(
	   (DWORD*)pShader->GetBufferPointer(),
	  FALSE,
	  NULL,
	  &pDis
	);
  const char* text = (char*)pDis->GetBufferPointer();

  MAID_WARNING( text );
  pDis->Release();
  }
//*/
  const int len = pShader->GetBufferSize();

  Binary.resize( pShader->GetBufferSize() );
  memcpy( &(Binary[0]), pShader->GetBufferPointer(), pShader->GetBufferSize()  );

MAID_ERROR:
   if( pShader!=NULL ) { pShader->Release(); }
   if( pErrorMsgs!=NULL ) { pErrorMsgs->Release(); }
   return SUCCEEDED(ret);
}



}}


