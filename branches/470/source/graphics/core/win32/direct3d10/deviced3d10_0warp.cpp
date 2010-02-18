#include"deviced3d10_0warp.h"


namespace Maid { namespace Graphics {

DeviceD3D10_0WARP::DeviceD3D10_0WARP( const DllWrapper& dll, Window& Windw )
  :DeviceD3D10_0( dll, SPDXGIADAPTER(), Windw )
{

}

FUNCTIONRESULT DeviceD3D10_0WARP::CreateDeviceAndSwapChain( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter, DXGI_SWAP_CHAIN_DESC& desc, SPD3D10DEVICE& pDevice, SPDXGISWAPCHAIN& pSwapChain )
{

  typedef HRESULT (WINAPI *FUNCTIONPTR)(IDXGIAdapter*,D3D10_DRIVER_TYPE,HMODULE,UINT,UINT,DXGI_SWAP_CHAIN_DESC*,IDXGISwapChain**,ID3D10Device**);
  FUNCTIONPTR createdevice = (FUNCTIONPTR)dll.GetProcAddress(MAIDTEXT("D3D10CreateDeviceAndSwapChain"));

  IDXGISwapChain* p=NULL;
  ID3D10Device* pDev = NULL;

  const HRESULT ret = createdevice(
    NULL,
    D3D10_DRIVER_TYPE_WARP,
    NULL,
    0,
    D3D10_SDK_VERSION,
    &desc,
    &p,
    &pDev
    );

  if( FAILED(ret) ) { MAID_WARNING("D3D10CreateDeviceAndSwapChain()"); return FUNCTIONRESULT_ERROR; }

  pDevice.reset(pDev);
  pSwapChain.reset(p);

  return FUNCTIONRESULT_OK;

}



}}


