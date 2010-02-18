#include"deviced3d11warp.h"


namespace Maid { namespace Graphics {

DeviceD3D11WARP::DeviceD3D11WARP( const DllWrapper& dll, Window& Windw )
  :DeviceD3D11( dll, SPDXGIADAPTER(), Windw )
{

}


FUNCTIONRESULT DeviceD3D11WARP::CreateDeviceAndSwapChain( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter, DXGI_SWAP_CHAIN_DESC& desc, SPD3D11DEVICE& pDevice, SPDXGISWAPCHAIN& pSwapChain )
{
  typedef HRESULT (WINAPI *FUNCTIONPTR)(IDXGIAdapter*,D3D_DRIVER_TYPE,HMODULE,UINT,const D3D_FEATURE_LEVEL*,UINT,UINT, const DXGI_SWAP_CHAIN_DESC*,IDXGISwapChain**,ID3D11Device**, D3D_FEATURE_LEVEL*, ID3D11DeviceContext** );
	FUNCTIONPTR createdevice = (FUNCTIONPTR)dll.GetProcAddress(MAIDTEXT("D3D11CreateDeviceAndSwapChain"));

  const D3D_FEATURE_LEVEL FeatureEnum[] = 
  {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
//      D3D_FEATURE_LEVEL_9_3,
//      D3D_FEATURE_LEVEL_9_2,
//      D3D_FEATURE_LEVEL_9_1,
  };

  IDXGISwapChain* p=NULL;
  ID3D11Device* pDev = NULL;

  const HRESULT ret = createdevice( 
      NULL, 
      D3D_DRIVER_TYPE_WARP, 
      NULL, 
      0, 
      FeatureEnum,
      NUMELEMENTS(FeatureEnum),
      D3D11_SDK_VERSION, 
      &desc,
      &p,
      &pDev,
      NULL,
      NULL
      );


  if( FAILED(ret) ) { MAID_WARNING("D3D11CreateDeviceAndSwapChain()"); return FUNCTIONRESULT_ERROR; }

  pDevice.reset(pDev);
  pSwapChain.reset(p);

  return FUNCTIONRESULT_OK;
}



}}


