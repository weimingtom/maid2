#include"devicelist.h"
#include"../../../auxiliary/macro.h"

#include"direct3d09/deviced3d09.h"
#include"fontdevice.h"

namespace Maid { namespace Graphics {

DeviceList::DeviceList( Window& window )
  :m_Window(window)
{

}

void DeviceList::Initialize()
{
  InitD3D09();
  InitD3D10();

}

//  DeviceID の割り振りルール
//  D3DXX が２ケタ　adapter番号が２ケタとする
//  たとえば Dx9  で adapter:1 なら  0901
//  たとえば Dx10 で adapter:5 なら  1005



void DeviceList::GetList( std::vector<INFO>& ret )
{
  INFO info;
  info.DeviceID = 0;
  info.Name = MAIDTEXT("Direct3D9");

  ret.push_back( info );
}

SPDEVICE DeviceList::Create( int DeviceID, const IDeviceList::CREATEPARAM& Info )
{
#pragma  COMPILERMSG( "当面は引数に関係なくD3D9 HALです" )

  SPDEVICE pDevice;

  {
    const UINT adapter = D3DADAPTER_DEFAULT;
    const bool STAL = Info.IsSoftwareVertexProcessing;

    pDevice.reset( new DeviceD3D09( m_pDirect3D09, adapter, STAL, m_Window ) );
  }

  return pDevice;
}

SPFONTDEVICE DeviceList::CreateFontDevice()
{
  return SPFONTDEVICE( new FontDevice );
}

void DeviceList::InitD3D09()
{
	m_D3D9DLL.Load( MAIDTEXT("D3D9.dll") );

	{
		typedef IDirect3D9* (WINAPI *FUNCTIONPTR)(UINT);
		FUNCTIONPTR pd3dCreate9 = (FUNCTIONPTR)m_D3D9DLL.GetProcAddress(MAIDTEXT("Direct3DCreate9"));

		IDirect3D9* pD3D = pd3dCreate9( D3D_SDK_VERSION );

		if( pD3D==NULL ) { MAID_WARNING( MAIDTEXT("Direct3DCreate9 の失敗") ); return ; }

		m_pDirect3D09.reset( pD3D );
	}

}

void DeviceList::InitD3D10()
{
	m_DXGIDLL.Load( MAIDTEXT("dxgi.dll") );
	m_D3D10DLL.Load( MAIDTEXT("D3D10.dll") );

  {
		typedef HRESULT (WINAPI *FUNCTIONPTR)(REFIID, void **);
		FUNCTIONPTR create = (FUNCTIONPTR)m_DXGIDLL.GetProcAddress(MAIDTEXT("CreateDXGIFactory"));

    IDXGIFactory * pFactory = NULL;
    const HRESULT ret = create(__uuidof(IDXGIFactory), (void**)(&pFactory) );

    if( FAILED(ret) ) { MAID_WARNING( MAIDTEXT("CreateDXGIFactory の失敗") ); return ; }

    m_pDXGIFactory.reset(pFactory);
  }
}


}}



