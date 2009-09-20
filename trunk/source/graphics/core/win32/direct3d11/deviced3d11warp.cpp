#include"deviced3d11warp.h"


namespace Maid { namespace Graphics {

DeviceD3D11WARP::DeviceD3D11WARP( const DllWrapper& dll, const SPDXGIFACTORY& pFactory, Window& Windw )
  :DeviceD3D11( dll, pFactory, SPDXGIADAPTER(), Windw )
{

}

SPD3D11DEVICE DeviceD3D11WARP::CreateDevice( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter )
{
  typedef HRESULT (WINAPI *FUNCTIONPTR)(IDXGIAdapter*,D3D_DRIVER_TYPE,HMODULE,UINT,CONST D3D_FEATURE_LEVEL*, UINT,UINT,ID3D11Device**, D3D_FEATURE_LEVEL* pLevel, ID3D11DeviceContext* pContext );
	FUNCTIONPTR createdevice = (FUNCTIONPTR)dll.GetProcAddress(MAIDTEXT("D3D11CreateDevice"));

  if( createdevice==NULL ) { MAID_WARNING("load失敗"); return SPD3D11DEVICE(); }

  ID3D11Device* pDev = NULL;

  const D3D_FEATURE_LEVEL FeatureEnum[] = 
  {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
      D3D_FEATURE_LEVEL_9_3,
      D3D_FEATURE_LEVEL_9_2,
      D3D_FEATURE_LEVEL_9_1,
  };

  D3D_FEATURE_LEVEL ret_level;

  const HRESULT ret = createdevice(
    NULL,
    D3D_DRIVER_TYPE_WARP,
    NULL,
    0,
    FeatureEnum,
    NUMELEMENTS(FeatureEnum),
    D3D11_SDK_VERSION,
    &pDev,
    &ret_level,
    NULL
    );

  if( FAILED(ret) ) { MAID_WARNING("D3D11CreateDevice()"); return SPD3D11DEVICE(); }

  return SPD3D11DEVICE(pDev);
}



}}


