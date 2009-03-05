#include"deviced3d10_0warp.h"


namespace Maid { namespace Graphics {

DeviceD3D10_0WARP::DeviceD3D10_0WARP( const DllWrapper& dll, const DllWrapper& warp, const SPDXGIFACTORY& pFactory, Window& Windw )
  :DeviceD3D10_0( dll, pFactory, SPDXGIADAPTER(), Windw )
  ,m_WARPDLL(warp)
{

}

SPD3D10DEVICE DeviceD3D10_0WARP::CreateDevice( const DllWrapper& dll, const SPDXGIADAPTER& pAdapter )
{
  typedef HRESULT (WINAPI *FUNCTIONPTR)(IDXGIAdapter*,D3D10_DRIVER_TYPE,HMODULE,UINT,UINT,ID3D10Device**);
	FUNCTIONPTR createdevice = (FUNCTIONPTR)dll.GetProcAddress(MAIDTEXT("D3D10CreateDevice"));

  if( createdevice==NULL ) { MAID_WARNING("load失敗"); return SPD3D10DEVICE(); }

  ID3D10Device* pDev = NULL;
  const HRESULT ret = createdevice( 
      NULL, 
      D3D10_DRIVER_TYPE_SOFTWARE, 
      m_WARPDLL.GetModule(), 
      0, 
      D3D10_SDK_VERSION, 
      &pDev 
      );


  if( pDev==NULL ) 
  {
    MAID_WARNING("D3D10CreateDevice()");
    return SPD3D10DEVICE(); 
  }

  return SPD3D10DEVICE(pDev);
}

void DeviceD3D10_0WARP::SerchDisplayInfo( std::vector<DISPLAYINFO>& info ) const
{
  const SPDXGIFACTORY& pFactory = GetFactory();

  int count = 0;

  for( int adpter_no=0; ; ++adpter_no )
  {
    SPDXGIADAPTER pAdapter;
    {
      IDXGIAdapter* p = NULL;
      const HRESULT ret = pFactory->EnumAdapters(adpter_no, &p);
      if( ret==DXGI_ERROR_NOT_FOUND) { break; }
      pAdapter.reset(p);
    }

    for( int output_no=0; ; ++output_no )
    {
      SPDXGIOUTPUT pOut;
      {
        IDXGIOutput* p = NULL;
        const HRESULT ret = pAdapter->EnumOutputs(output_no, &p);
        if( ret==DXGI_ERROR_NOT_FOUND) { break; }
        pOut.reset(p);
      }

      DXGI_OUTPUT_DESC desc;

      const HRESULT ret = pOut->GetDesc( &desc );
      if( FAILED(ret) ) { MAID_WARNING("IDXGIOutput::GetDesc " << ret ); continue; }

      DISPLAYINFO dat;
      dat.Name = String::ConvertUNICODEtoMAID(desc.DeviceName);
      info.push_back( dat );
    }
  }
}


}}


