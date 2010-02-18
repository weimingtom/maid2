#include"deviced3d10_1warp.h"


namespace Maid { namespace Graphics {

DeviceD3D10_1WARP::DeviceD3D10_1WARP( const DllWrapper& dll, Window& Windw )
  :DeviceD3D10_1( dll, SPDXGIADAPTER(), Windw )
{

}

FUNCTIONRESULT DeviceD3D10_1WARP::CreateDeviceAndSwapChain( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter, DXGI_SWAP_CHAIN_DESC& desc, SPD3D10DEVICE& pDevice, SPDXGISWAPCHAIN& pSwapChain )
{
  typedef HRESULT (WINAPI *FUNCTIONPTR)(IDXGIAdapter*,D3D10_DRIVER_TYPE,HMODULE,UINT,D3D10_FEATURE_LEVEL1,UINT,DXGI_SWAP_CHAIN_DESC*,IDXGISwapChain**,ID3D10Device1**);
	FUNCTIONPTR createdevice = (FUNCTIONPTR)dll.GetProcAddress(MAIDTEXT("D3D10CreateDeviceAndSwapChain1"));

  IDXGISwapChain* p=NULL;
  ID3D10Device1* pDev = NULL;


  const HRESULT ret = createdevice( 
      pAdapter.get(), 
      D3D10_DRIVER_TYPE_WARP, 
      NULL, 
      0, 
      D3D10_FEATURE_LEVEL_10_1, 
      D3D10_1_SDK_VERSION, 
      &desc,
      &p,
      &pDev 
      );


  if( FAILED(ret) ) { MAID_WARNING("D3D10CreateDeviceAndSwapChain1()"); return FUNCTIONRESULT_ERROR; }

  pDevice.reset(pDev);
  pSwapChain.reset(p);

  return FUNCTIONRESULT_OK;
}



}}


