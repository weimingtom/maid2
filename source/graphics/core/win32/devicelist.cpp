#include"devicelist.h"
#include"../../../auxiliary/macro.h"
#include"../../../framework/win32/shell.h"

#include"direct3d09/deviced3d09.h"
#include"fontdevice.h"

namespace Maid { namespace Graphics {

//  DeviceID の割り振りルール
//  D3DXX が３ケタ　adapter番号が３ケタとする
//  たとえば Dx9   で adapter:1 なら  090001
//  たとえば Dx10  で adapter:5 なら  100005
//  たとえば Dx10.1で adapter:5 なら  101005

//  それぞれのデバイスで adapter:0はデフォルトデバイスとする
inline int MakeID( int dx, int sub, int no )
{
  return dx*10000 + sub*1000 + no;
}

inline int MakeID( int dx, int no )
{
  return MakeID( dx, 0, no );
}


inline int GetDxVersion( int id ) { return id / 10000; }
inline int GetDxSubVersion( int id ) { return (id / 1000) %10; }
inline int GetDeviceID ( int id ) { return id % 1000; }


DeviceList::DeviceList( Window& window )
  :m_Window(window)
  ,m_IsD3D09(false)
  ,m_IsDXGI(false)
  ,m_IsD3D10(false)
  ,m_IsD3D10_1(false)
  ,m_IsD3D11(false)
{

}

void DeviceList::Initialize()
{
  InitializeD3D09();
  InitializeDXGI();
  InitializeD3D10();
//  InitializeD3D11();
}


void DeviceList::GetList( std::vector<INFO>& ret )
{
  FindAdapterD3D09( ret );
}

SPDEVICE DeviceList::Create( int DeviceID )
{
  SPDEVICE pDevice;

  const int dx = GetDxVersion(DeviceID);
  const int sub = GetDxSubVersion(DeviceID);
  const int id = GetDeviceID(DeviceID);

  switch( dx )
  {
  case 9:
    {
      UINT adapter = D3DADAPTER_DEFAULT;
      bool IsSoftwareVP = true;

      if( 2 < id ) { adapter = (id-2)/2; }
      IsSoftwareVP =  id%2 == 0;

      pDevice.reset( new DeviceD3D09( m_pDirect3D09, adapter, IsSoftwareVP, m_Window ) );

    }break;

  case 10:
    {

    }break;

  default:{ MAID_ASSERT( true, "デバイスが選択されていません " << DeviceID ); }break;
  }

  return pDevice;
}

SPFONTDEVICE DeviceList::CreateFontDevice()
{
  return SPFONTDEVICE( new FontDevice );
}


void DeviceList::InitializeD3D09()
{
  const String dllname = MAIDTEXT("d3d9.dll");

  if( !DllWrapper::IsExist(dllname) ) { return ; }

	m_D3D9DLL.Load( dllname );

	{
		typedef IDirect3D9* (WINAPI *FUNCTIONPTR)(UINT);
		FUNCTIONPTR pd3dCreate9 = (FUNCTIONPTR)m_D3D9DLL.GetProcAddress(MAIDTEXT("Direct3DCreate9"));

		IDirect3D9* pD3D = pd3dCreate9( D3D_SDK_VERSION );

		if( pD3D==NULL ) { MAID_WARNING( MAIDTEXT("Direct3DCreate9 の失敗") ); return ; }

		m_pDirect3D09.reset( pD3D );
	}

  m_IsD3D09 = true;
}

void DeviceList::FindAdapterD3D09( std::vector<INFO>& info )
{
  if( !m_IsD3D09 ) { return ; }

  //  dx9 DeviceID 生成ルール
  //  偶数は MixVertexProcessing
  //  奇数は SoftwareVertexProcessing

  //  最初はデフォルト D3DADAPTER_DEFAULT
  info.push_back( INFO(MakeID(9,0),MAIDTEXT("Direct3D9 :Default(SoftwareVertexProcessing)")) );
  info.push_back( INFO(MakeID(9,1),MAIDTEXT("Direct3D9 :Default(MixedVertexProcessing)"))    );


  const SPD3D09& pD3D = m_pDirect3D09;

  const UINT count = pD3D->GetAdapterCount();

  for( UINT i=0; i<count; ++i )
  {
    D3DADAPTER_IDENTIFIER9 dat;
    const HRESULT ret = pD3D->GetAdapterIdentifier( i, 0, &dat );

    if( FAILED(ret) ) { continue; }

    const int id_base = 2+i*2;
    const String tex = MAIDTEXT("Direct3D9 :") + String::ConvertSJIStoMAID(dat.Description);

    info.push_back( INFO(MakeID(9,id_base+0), tex + MAIDTEXT("(SoftwareVertexProcessing)")) );
    info.push_back( INFO(MakeID(9,id_base+1), tex + MAIDTEXT("(MixedVertexProcessing)")) );
  }
}



void DeviceList::InitializeDXGI()
{
  const String dllname = MAIDTEXT("dxgi.dll");

  if( !DllWrapper::IsExist(dllname) ) { return ; }

	m_DXGIDLL.Load( dllname );

  {
		typedef HRESULT (WINAPI *FUNCTIONPTR)(REFIID, void **);
		FUNCTIONPTR create = (FUNCTIONPTR)m_DXGIDLL.GetProcAddress(MAIDTEXT("CreateDXGIFactory"));

    IDXGIFactory * pFactory = NULL;
    const HRESULT ret = create(__uuidof(IDXGIFactory), (void**)(&pFactory) );

    if( FAILED(ret) ) { MAID_WARNING( MAIDTEXT("CreateDXGIFactory の失敗") ); return ; }

    m_pDXGIFactory.reset(pFactory);
  }

  m_IsDXGI = true;
}

void DeviceList::InitializeD3D10()
{
  {
    const String dllname = MAIDTEXT("d3d10.dll");
    if( !DllWrapper::IsExist(dllname) ) { return ; }
	  m_D3D10DLL.Load( dllname );

    m_IsD3D10 = true;
  }

  {
    const String dllname = MAIDTEXT("d3d10_1.dll");
    if( !DllWrapper::IsExist(dllname) ) { return ; }
	  m_D3D10_1DLL.Load( dllname );

    m_IsD3D10_1 = true;
  }

}


}}



