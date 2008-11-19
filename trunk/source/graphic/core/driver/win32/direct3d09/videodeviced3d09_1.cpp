#include"videodeviced3d09.h"


#include"../../../../../auxiliary/exception.h"
#include"../../../../../auxiliary/debug/trace.h"
#include"../../../../../auxiliary/debug/warning.h"
#include"../../../../../auxiliary/string.h"

namespace Maid
{


/*!
 	@class	VideoDeviceD3D09 videodeviced3d09.h
 	@brief	レンダリングドライバ Direct3D9 Ver.
 */

VideoDeviceD3D09::VideoDeviceD3D09( const SPD3DDEVICE& pDev, Window& win )
  :m_pDevice(pDev), m_Window(win)
{
}

void VideoDeviceD3D09::SetupDevice()
{
  D3DPRESENT_PARAMETERS param;
  {
    SPD3DSWAPCHAIN pSwap;
    {
      IDirect3DSwapChain9* p;
      const HRESULT ret = m_pDevice->GetSwapChain( 0, &p );
    	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::GetSwapChain()")); }
      pSwap.reset( p );
    }
    pSwap->GetPresentParameters( &param );
  }
  D3DDISPLAYMODE CurrentDiplayMode;
  {
    const HRESULT ret = m_pDevice->GetDisplayMode( 0, &CurrentDiplayMode );
    if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::GetDisplayMode()")); }
  }

  {
    const HRESULT ret = m_pDevice->GetCreationParameters( &m_CreateParams );
   	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::GetCreationParameters()")); }
  }

  m_ScreenMode.DisplayMode.Size = SIZE2DI(param.BackBufferWidth,param.BackBufferHeight );
  m_ScreenMode.DisplayMode.Format = D3DFORMATtoPIXELFORMAT(CurrentDiplayMode.Format);
  m_ScreenMode.DisplayMode.RefreshRate = 0;

  m_ScreenMode.IsWaitVSync = param.PresentationInterval != D3DPRESENT_INTERVAL_IMMEDIATE;

  m_WindowModeStyle = m_Window.GetStyle();

  {
    IDirect3DSurface9* p;
    const HRESULT ret = m_pDevice->GetRenderTarget( 0, &p );
    if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::GetRenderTarget()")); }
    m_pDefaultRenderTarget.reset( p );
  }

  {
    IDirect3DSurface9* p;
    const HRESULT ret = m_pDevice->GetDepthStencilSurface( &p );
    if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::GetDepthStencilSurface()")); }
    m_pDefaultDepthStencil.reset( p );
  }

}

void VideoDeviceD3D09::SerchDisplayMode( std::vector<DISPLAYMODE>& DisplayMode )
{
  MAID_ASSERT( !DisplayMode.empty(), "引数はemptyにしてください" );

  static const D3DFORMAT DISPLAYFMT[] =
  {
    //  作成できるバックバッファの列挙。
    //  これはD3D9の仕様で決まっている。
    D3DFMT_R5G6B5,
    D3DFMT_X1R5G5B5,
    D3DFMT_A1R5G5B5,
    D3DFMT_X8R8G8B8,
    D3DFMT_A8R8G8B8,
    D3DFMT_A2R10G10B10,
  };

  SPD3D pD3D = GetD3D();
  const UINT ADAPTERID = m_CreateParams.AdapterOrdinal;

  for( int displayno=0; displayno<NUMELEMENTS(DISPLAYFMT); ++displayno )
  {
    const D3DFORMAT AdapterFormat = DISPLAYFMT[displayno];
    const int ModeCount = pD3D->GetAdapterModeCount( ADAPTERID, AdapterFormat );

    for( int ModeNo=0; ModeNo<ModeCount; ++ModeNo )
    {
      D3DDISPLAYMODE mode;
      const HRESULT ret = pD3D->EnumAdapterModes( ADAPTERID, AdapterFormat, ModeNo, &mode );
      if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3D::EnumAdapterModes()")); }

      DISPLAYMODE m;

      m.Size = SIZE2DI(mode.Width,mode.Height);
      m.Format = D3DFORMATtoPIXELFORMAT(mode.Format);
      m.RefreshRate = mode.RefreshRate;

      DisplayMode.push_back( m );
    }
  }

}

void VideoDeviceD3D09::SerchEnableFormat( ENABLEFORMAT& caps )
{
  MAID_ASSERT( !caps.Texture.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.DepthStencil.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.RenderTargetTexture.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.RenderTargetSurface.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.IndexBit.empty(), "引数はemptyにしてください" );

  static const D3DFORMAT s_TEXTURE_FORMAT[] =
  {
    //  作成できるテクスチャフォーマット,RenderSurface,RenderTextueの限界。
    D3DFMT_A8,
    D3DFMT_P8,
    D3DFMT_R3G3B2,

    D3DFMT_A8P8,
    D3DFMT_R5G6B5,
    D3DFMT_X1R5G5B5,
    D3DFMT_A1R5G5B5,
    D3DFMT_A4R4G4B4,
    D3DFMT_A8R3G3B2,
    D3DFMT_X4R4G4B4,

    D3DFMT_R8G8B8,

    D3DFMT_A8R8G8B8,
    D3DFMT_X8R8G8B8,
    D3DFMT_A8B8G8R8,
    D3DFMT_X8B8G8R8,
    D3DFMT_A2B10G10R10,
    D3DFMT_A2R10G10B10,
    D3DFMT_G16R16,

    D3DFMT_A16B16G16R16,

    D3DFMT_DXT1,
    D3DFMT_DXT2,
    D3DFMT_DXT3,
    D3DFMT_DXT4,
    D3DFMT_DXT5,
    D3DFMT_R16F,
    D3DFMT_G16R16F,
    D3DFMT_A16B16G16R16F,
    D3DFMT_R32F,
    D3DFMT_G32R32F,
    D3DFMT_A32B32G32R32F,
  };

  static const D3DFORMAT s_DEPTH_STENCIL_FORMAT[] =
  {
    //  作成できる深度バッファの限界。
    D3DFMT_D32,
    D3DFMT_D15S1,
    D3DFMT_D24S8,
    D3DFMT_D24X8,
    D3DFMT_D24X4S4,
    D3DFMT_D24FS8,
    D3DFMT_D16,
  };

  static const D3DFORMAT s_INDEX_FORMAT[] =
  {
    //  作成できるインデックスバッファの限界。
    D3DFMT_INDEX16,
    D3DFMT_INDEX32,
  };

  SPD3D pD3D = GetD3D();
  const UINT AdapterID = m_CreateParams.AdapterOrdinal;
  const D3DDEVTYPE DeviceType = m_CreateParams.DeviceType;
  D3DDISPLAYMODE CurrentDiplayMode;

  const HRESULT ret = m_pDevice->GetDisplayMode( 0, &CurrentDiplayMode );
  if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::GetDisplayMode()")); }

  {
    //  作成できるテクスチャのフォーマットを調べる
    for( int i=0; i<NUMELEMENTS(s_TEXTURE_FORMAT); ++i )
    {
      const D3DFORMAT fmt = s_TEXTURE_FORMAT[i];
      const HRESULT ret = pD3D->CheckDeviceFormat(
                                      AdapterID, DeviceType, CurrentDiplayMode.Format,
                                      0, D3DRTYPE_TEXTURE,
                                      fmt                                      
                                      );

      if( ret==D3D_OK ) { caps.Texture.push_back(D3DFORMATtoPIXELFORMAT(fmt)); }
    }
  }
  {
    //  作成できるレンダーテクスチャのフォーマットを調べる
    for( int i=0; i<NUMELEMENTS(s_TEXTURE_FORMAT); ++i )
    {
      const D3DFORMAT fmt = s_TEXTURE_FORMAT[i];
      const HRESULT ret = pD3D->CheckDeviceFormat(
                                      AdapterID, DeviceType, CurrentDiplayMode.Format,
                                      D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE,
                                      fmt                                      
                                      );
      if( ret==D3D_OK ) { caps.RenderTargetTexture.push_back(D3DFORMATtoPIXELFORMAT(fmt)); }
    }
  }
  {
    //  作成できるレンダーサーフェスのフォーマットを調べる
    for( int i=0; i<NUMELEMENTS(s_TEXTURE_FORMAT); ++i )
    {
      const D3DFORMAT fmt = s_TEXTURE_FORMAT[i];
      const HRESULT ret = pD3D->CheckDeviceFormat(
                                      AdapterID, DeviceType, CurrentDiplayMode.Format,
                                      D3DUSAGE_RENDERTARGET, D3DRTYPE_SURFACE,
                                      fmt                                      
                                      );

      if( ret==D3D_OK ) { caps.RenderTargetSurface.push_back(D3DFORMATtoPIXELFORMAT(fmt)); }
    }
  }

  {
    //  作成できるDepthStencilフォーマットを調べる
    for( int i=0; i<NUMELEMENTS(s_DEPTH_STENCIL_FORMAT); ++i )
    {
      const D3DFORMAT fmt = s_DEPTH_STENCIL_FORMAT[i];
      const HRESULT ret = pD3D->CheckDeviceFormat(
                                      AdapterID, DeviceType, CurrentDiplayMode.Format,
                                      D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE,
                                      fmt                                      
                                      );

      if( ret==D3D_OK ) { caps.DepthStencil.push_back(D3DFORMATtoDEPTHSTENCILFORMAT(fmt)); }
    }
  }

  {
    //  作成できるDepthStencilフォーマットを調べる
    for( int i=0; i<NUMELEMENTS(s_INDEX_FORMAT); ++i )
    {
      const D3DFORMAT fmt = s_INDEX_FORMAT[i];
      const HRESULT ret = pD3D->CheckDeviceFormat(
                                      AdapterID, DeviceType, CurrentDiplayMode.Format,
                                      0, D3DRTYPE_INDEXBUFFER,
                                      fmt                                      
                                      );

      if( ret==D3D_OK )
      {
        int bit = 0;
        switch( fmt )
        {
        case D3DFMT_INDEX16: { bit = 16; }break;
        case D3DFMT_INDEX32: { bit = 32; }break;
        }
        caps.IndexBit.push_back(bit); 
      }
    }
  }
}


SPD3D VideoDeviceD3D09::GetD3D()
{
  SPD3D pD3D;
  {
    IDirect3D9* p;
    const HRESULT ret = m_pDevice->GetDirect3D( &p );
    if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::GetDirect3D()")); }
    pD3D.reset( p );
  }

  return pD3D;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! VideoDeviceD3D09::Flip() を参照
void VideoDeviceD3D09::Flip()
{
	const HRESULT ret = m_pDevice->Present( NULL, NULL, NULL, NULL );
	if( ret==D3DERR_DEVICELOST ) { throw DeviceLost(); }
	ef( FAILED(ret) ) { MAID_THROWEXCEPTION( MAIDTEXT("IDirect3DDevice9::Present") ); }
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! VideoDeviceD3D09::Reset() 参照
void	VideoDeviceD3D09::Reset( const SCREENMODE& Mode )
{
/*
	for( int i=0; i<8; ++i ) { m_pDevice->SetTexture( i, NULL ); }
	for( int i=0; i<8; ++i ) { m_pDevice->SetStreamSource( i, NULL, 0, 0 ); }

	m_pDevice->SetIndices( NULL );
	m_pDevice->SetPixelShader( NULL );
	m_pDevice->SetVertexShader( NULL );
	m_pDevice->SetVertexDeclaration( NULL );

	D3DMATRIX view, proj;
	m_pDevice->GetTransform( D3DTS_VIEW, &view );
	m_pDevice->GetTransform( D3DTS_PROJECTION, &proj );

	ReleaseRTSurface();

	//	フルスクリーン -> フルスクリーンの場合も考えて
	//	ウィンドウ -> フルスクリーン　の時だけウィンドウ座標を取る

	if( m_DeviceInfo.Windowed==TRUE && Mode.Mode!=SCREENMODE::TYPE_WINDOW )
	{
		UpdateWindowInfo();
	}

	InitializePresertParameters( m_DeviceInfo, Mode );


	const HRESULT ret = m_pDevice->Reset( &m_DeviceInfo );
	if( FAILED(ret) ) { return; }

	if( m_DeviceInfo.Windowed==TRUE )
	{
		//	フルスクリーンからウィンドウに戻ってきたらウィンドウを再配置
		for( MySTL::list<SWAPWINDOWINFO>::iterator ite=s_WindowInfoList.begin();
					ite!=s_WindowInfoList.end(); ++ite )
		{
			SWAPWINDOWINFO& info = *ite;
			::MoveWindow( info.hWnd, info.Rect.x,  info.Rect.y, info.Rect.w, info.Rect.h, TRUE );
		}

		RECT	rc = { 0, 0, m_DeviceInfo.BackBufferWidth, m_DeviceInfo.BackBufferHeight };
		AdjustWindowRect( &rc, m_WindowModeStyle, FALSE );
		const int w = rc.right  - rc.left;
		const int h = rc.bottom - rc.top;

		::SetWindowLong( m_hWnd, GWL_STYLE, m_WindowModeStyle );
		::SetWindowPos( m_hWnd, HWND_NOTOPMOST,
				m_WindowModePos.x, m_WindowModePos.y,
				w, h,
				SWP_DRAWFRAME|SWP_SHOWWINDOW );
		::InvalidateRect(NULL, NULL, FALSE);
	}else
	{
		::SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP|WS_EX_TOPMOST );
		::SetWindowPos( m_hWnd, HWND_TOPMOST,
				0, 0,
				Mode.Size.w, Mode.Size.h,
				SWP_DRAWFRAME|SWP_SHOWWINDOW );
	}


	CreateRTSurface();

//	m_pDevice->EvictManagedResources();
	m_pDevice->SetTransform( D3DTS_VIEW, &view );
	m_pDevice->SetTransform( D3DTS_PROJECTION, &proj );

	m_pDevice->GetDeviceCaps(&m_DeviceCaps);
*/
}

VideoDevice::DEVICESTATE VideoDeviceD3D09::TestCooperativeLevel()
{
	const HRESULT ret = m_pDevice->TestCooperativeLevel();

	if( ret==D3D_OK ) { return DEVICESTATE_OK; }
	ef( ret==D3DERR_DEVICENOTRESET ) { return DEVICESTATE_NOTRESET; }

	return DEVICESTATE_LOST;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! VideoDeviceD3D09::BeginScene() を参照
void VideoDeviceD3D09::BeginScene()
{
	const HRESULT ret = m_pDevice->BeginScene();
	if( FAILED(ret) ){ MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice::BeginScene()") ); }
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! VideoDeviceD3D09::EndScene() を参照
void VideoDeviceD3D09::EndScene()
{
	const HRESULT ret = m_pDevice->EndScene();
	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice::EndScene()")); }
}

#if 0
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//!	指定したモードの D3DPRESENT_PARAMETERS を作成する
/*!	
	@param	Info		[ o]	作成先	
 	@param	Mode		[i ]	スクリーンモード
 */
void	CVideoCardDriverD3D9::InitializePresertParameters( D3DPRESENT_PARAMETERS& Info, const SCREENMODE& Mode  )
{
	m_ScreenMode = Mode;

	//	各モード共通の設定
	ZeroMemory( &Info, sizeof(Info) );
	Info.BackBufferWidth = Mode.Size.w;
	Info.BackBufferHeight= Mode.Size.h;
	Info.hDeviceWindow   = m_hWnd;
	Info.EnableAutoDepthStencil = TRUE;
	Info.AutoDepthStencilFormat = D3DFMT_D16;
	Info.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER ;
//	Info.Flags = 0 ;
	Info.BackBufferFormat = GetBackBufferFormatD3D( Mode.Mode );


	{
		//	スクリーンモードで違う個所の設定
		//	ウィンドウモードだったら BackBuffer は１。 Flip はコピー
		//	フルスクリーンだったら BackBuffer 任意。 Flip はコピーor交換＋VSync任意

		if( Mode.Mode==SCREENMODE::TYPE_WINDOW )
		{
			Info.Windowed = TRUE;  
			Info.BackBufferCount = 1;
			Info.SwapEffect = D3DSWAPEFFECT_COPY;
			Info.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
			Info.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		}
		else
		{
			Info.Windowed = FALSE;

			if( IsAspectLock() )
			{
				Info.BackBufferWidth = m_AspectLock.w;
				Info.BackBufferHeight= m_AspectLock.h;
			}


			if( Mode.IsWaitVSync )
			{
				Info.BackBufferCount = 1; 
				Info.SwapEffect = D3DSWAPEFFECT_FLIP;
				Info.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT  ;
			}
			else
			{	//	コピー
				Info.SwapEffect = D3DSWAPEFFECT_COPY;	
				Info.BackBufferCount = 1;
				Info.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
			}

			//	ハードウェアが指定したリフレッシュレートに対応しているならそれにする
			if( SerchDisplayMode( Mode.Size.w, Mode.Size.h, Mode.RefreshRate, Info.BackBufferFormat ) )
			{
				Info.FullScreen_RefreshRateInHz = Mode.RefreshRate;
			}else{
				Info.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
			}
		}
	}
}


void CVideoCardDriverD3D9::UpdateWindowInfo()
{
	{
		RECT rc;
		GetWindowRect( m_hWnd, &rc );

		m_WindowModePos = POINT2DI(rc.left,rc.top);
		m_WindowModeStyle = ::GetWindowLong( m_hWnd, GWL_STYLE );
	}

	s_WindowInfoList.clear();

	EnumWindows( EnumWindowsProc, (LPARAM)&s_WindowInfoList );

	for( MySTL::list<SWAPWINDOWINFO>::iterator ite=s_WindowInfoList.begin();
							ite!=s_WindowInfoList.end(); ++ite )
	{
		if( ite->hWnd==m_hWnd )
		{
			s_WindowInfoList.erase( ite );
			break;
		}
	}
}
#endif

}

