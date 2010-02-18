#include"deviced3d10_1.h"

#include"bufferd3d10.h"
#include"texture2dd3d10.h"
#include"materiald3d10.h"
#include"blendstated3d10.h"

namespace Maid { namespace Graphics {

DeviceD3D10_1::DeviceD3D10_1( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter, Window& Windw )
  :DeviceD3D10_0( dll, pAdapter, Windw )
{

}

FUNCTIONRESULT DeviceD3D10_1::CreateDeviceAndSwapChain( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter, DXGI_SWAP_CHAIN_DESC& desc, SPD3D10DEVICE& pDevice, SPDXGISWAPCHAIN& pSwapChain )
{
  typedef HRESULT (WINAPI *FUNCTIONPTR)(IDXGIAdapter*,D3D10_DRIVER_TYPE,HMODULE,UINT,D3D10_FEATURE_LEVEL1,UINT,DXGI_SWAP_CHAIN_DESC*,IDXGISwapChain**,ID3D10Device1**);
	FUNCTIONPTR createdevice = (FUNCTIONPTR)dll.GetProcAddress(MAIDTEXT("D3D10CreateDeviceAndSwapChain1"));

  static const D3D10_FEATURE_LEVEL1 levelAttempts[] = 
  { 
      D3D10_FEATURE_LEVEL_10_1, 
      D3D10_FEATURE_LEVEL_10_0, 
//      D3D10_FEATURE_LEVEL_9_3, 
//      D3D10_FEATURE_LEVEL_9_2, 
//      D3D10_FEATURE_LEVEL_9_1, 
  };

  IDXGISwapChain* p=NULL;
  ID3D10Device1* pDev = NULL;

  for ( int i=0; i<NUMELEMENTS(levelAttempts); ++i ) 
  { 
    const HRESULT ret = createdevice( 
        pAdapter.get(), 
        D3D10_DRIVER_TYPE_HARDWARE, 
        NULL, 
        0, 
        levelAttempts[i], 
        D3D10_1_SDK_VERSION, 
        &desc,
        &p,
        &pDev 
        );

    if( SUCCEEDED(ret) ) { break; }
  }

  if( pDev==NULL || p==NULL ) { MAID_WARNING("D3D10CreateDeviceAndSwapChain1()"); return FUNCTIONRESULT_ERROR; }

  pDevice.reset(pDev);
  pSwapChain.reset(p);

  return FUNCTIONRESULT_OK;
}

SPMATERIAL     DeviceD3D10_1::CreateMaterial( const SPRESOURCE& resource, const CREATEMATERIALPARAM* param )
{
  ID3D10Resource* pResource = NULL;
  {
    switch( resource->GetType() )
    {
    case IResource::TYPE_BUFFER:    { pResource = static_cast<BufferD3D10*   >(resource.get())->pBuffer.get();  }break;
    case IResource::TYPE_TEXTURE2D: { pResource = static_cast<Texture2DD3D10*>(resource.get())->pTexture.get(); }break;
    }
  }

  D3D10_SHADER_RESOURCE_VIEW_DESC1* p_desc=NULL;
  D3D10_SHADER_RESOURCE_VIEW_DESC1 desc;

  if( param!=NULL )
  {
    p_desc = &desc;
    ZERO( &desc, sizeof(desc) );

    desc.Format = PIXELFORMATtoDXGI_FORMAT(param->Format);
    desc.ViewDimension = MATERIALDIMENSIONtoD3D10_SRV_DIMENSION1(param->Dimension);

    #pragma COMPILERMSG("desc.union はめんどいのでスルーする")	
  }

  SPD3D10SHADERRESOURCEVIEW pView;
  {
    ID3D10ShaderResourceView1* p = NULL;
    const HRESULT ret = static_cast<ID3D10Device1*>(GetDevice().get())->CreateShaderResourceView1( pResource, p_desc, &p );
    if( FAILED(ret) ) { MAID_WARNING("ID3D10Device::CreateShaderResourceView1 " << ret ); }
    pView.reset(p);
  }

  CREATEMATERIALPARAM result;
  {
    D3D10_SHADER_RESOURCE_VIEW_DESC1 dat;
    static_cast<ID3D10ShaderResourceView1*>(pView.get())->GetDesc1( &dat );

    result.Format = DXGI_FORMATtoPIXELFORMAT(dat.Format);
    result.Dimension = D3D10_SRV_DIMENSION1_MATERIALDIMENSION(dat.ViewDimension);
  }

  return SPMATERIAL( new MaterialD3D10(resource, result, pView ) );
}

SPBLENDSTATE   DeviceD3D10_1::CreateBlendState( const BLENDSTATEPARAM& Option )
{
  D3D10_BLEND_DESC1 desc = {};

  desc.AlphaToCoverageEnable = booltoD3D10BOOL(Option.AlphaToCoverageEnable);
  desc.IndependentBlendEnable = booltoD3D10BOOL(Option.IndependentBlendEnable);

  for( int i=0; i<NUMELEMENTS(desc.RenderTarget); ++i )
  {
    D3D10_RENDER_TARGET_BLEND_DESC1& dst = desc.RenderTarget[i];
    const BLENDSTATEPARAM::TARGET& src = Option.Target[i];

    dst.BlendEnable = booltoD3D10BOOL(src.Enable);

    dst.SrcBlend    = TARGETBLENDtoD3D10_BLEND(src.ColorSrc);
    dst.DestBlend   = TARGETBLENDtoD3D10_BLEND(src.ColorDst);
    dst.BlendOp     = TARGETOPERATIONtoD3D10_BLEND_OP(src.ColorOp);

    dst.SrcBlendAlpha = TARGETBLENDtoD3D10_BLEND(src.AlphaSrc);
    dst.DestBlendAlpha = TARGETBLENDtoD3D10_BLEND(src.AlphaDst);
    dst.BlendOpAlpha = TARGETOPERATIONtoD3D10_BLEND_OP(src.AlphaOp);

    dst.RenderTargetWriteMask = src.WriteMask;
  }



  SPD3D10BLENDSTATE pState;
  {
    ID3D10BlendState1* p = NULL;
    const HRESULT ret = static_cast<ID3D10Device1*>(GetDevice().get())->CreateBlendState1( &desc, &p );
    if( FAILED(ret) ) { MAID_WARNING("ID3D10Device::CreateBlendState1 " << ret ); }
    pState.reset(p);
  }

  return SPBLENDSTATE( new BlendStateD3D10(Option,pState) );
}


}}


