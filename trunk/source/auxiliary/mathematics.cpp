#include"mathematics.h"

#include"debug/warning.h"


namespace Maid
{

#ifdef USE_DIRECT3DX9
  static HMODULE s_hModule = NULL;
  void D3DXMathInitialize()
  {

    const wchar_t* filename[] =
    {
      L"d3dx9_40.dll",
      L"d3dx9_39.dll",
      L"d3dx9_38.dll",
      L"d3dx9_37.dll",
      L"d3dx9_36.dll",
      L"d3dx9_35.dll",
      L"d3dx9_34.dll",
      L"d3dx9_33.dll",
      L"d3dx9_32.dll",
    };

    for( int i=0; i<NUMELEMENTS(filename); ++i )
    {
	    HMODULE hInst  = ::LoadLibrary( filename[i] );
      if( hInst==NULL ) { continue; }
      s_hModule = hInst;
      break;
    }

    if( s_hModule==NULL )
    {
      ::MessageBox( NULL, L"D3DX9.dllが見つかりませんでした。\nDirectXランタイムをインストールしてください。", L"エラー", MB_OK );
      MAID_WARNING("d3dx.dllの読み込みに失敗");
      return ; 
    }

    d3dxmatrix::Initialize( s_hModule );
    d3dxquaternion::Initialize( s_hModule );
    d3dxvector2d::Initialize( s_hModule );
    d3dxvector3d::Initialize( s_hModule );
    d3dxvector4d::Initialize( s_hModule );
  }

  void D3DXMathFinalize()
  {
    d3dxmatrix::Finalize();
    d3dxquaternion::Finalize();
    d3dxvector2d::Finalize();
    d3dxvector3d::Finalize();
    d3dxvector4d::Finalize();

    ::FreeLibrary( s_hModule );
  }

#endif
}
