#include"deviced3d09.h"

#include"texture2dd3d09.h"
#include"rendertargetd3d09.h"
#include"depthstencild3d09.h"
#include"drawcommandexecuted3d09.h"

namespace Maid { namespace Graphics {

DeviceD3D09::DeviceD3D09( IDirect3DDevice9* pDev, Window& win )
:m_pDevice(pDev), m_Window(win)
{
}


void DeviceD3D09::Initialize()
{
  m_pDevice->GetDeviceCaps( &m_DeviceCaps );

  D3DPRESENT_PARAMETERS param;
  {
    SPD3D09SWAPCHAIN pSwap;
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
    static const D3DFORMAT s_DEPTH_STENCIL_FORMAT[] =
    {
      //  作成できる深度バッファの限界。
      D3DFMT_D16,
      D3DFMT_D24X8,
      D3DFMT_D32,
      D3DFMT_D15S1,
      D3DFMT_D24X4S4,
      D3DFMT_D24S8,
      D3DFMT_D24FS8,
    };

    const UINT w = param.BackBufferWidth;
    const UINT h = param.BackBufferHeight;

    IDirect3DSurface9* pStencil = NULL;
    for( int i=0; i<NUMELEMENTS(s_DEPTH_STENCIL_FORMAT); ++i )
    {
      D3DFORMAT fmt = s_DEPTH_STENCIL_FORMAT[i];
      const HRESULT ret = m_pDevice->CreateDepthStencilSurface( 
        w, h, fmt,
        D3DMULTISAMPLE_NONE, 0,
        FALSE,
        &pStencil, NULL
        );

     if( SUCCEEDED(ret) ) { break; }
    }

    if( pStencil==NULL ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::CreateDepthStencilSurface()")); }

    {
      const HRESULT ret = m_pDevice->SetDepthStencilSurface(pStencil);
      if( FAILED(ret) )  { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::SetDepthStencilSurface()"));    }
    }
    m_pDefaultDepthStencil.reset(pStencil);
  }

  m_pDrawCommandExecute.reset(new DrawCommandExecuteD3D09(*this));
}

void DeviceD3D09::Finalize()
{
  m_pDefaultDepthStencil.reset();
  m_pDevice.reset();
}

void DeviceD3D09::SerchDisplayMode( std::vector<DISPLAYMODE>& DisplayMode )
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

  SPD3D09 pD3D = GetD3D();
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

void DeviceD3D09::SerchEnableFormat( ENABLEFORMAT& caps )
{
  MAID_ASSERT( !caps.Texture.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.DepthStencil.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.RenderTargetTexture.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.RenderTargetSurface.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.IndexBit.empty(), "引数はemptyにしてください" );

 #pragma  COMPILERMSG( "TODO:コメントアウトしてあるフォーマットはそのうち対応する" )

  static const D3DFORMAT s_TEXTURE_FORMAT[] =
  {
    //  作成できるテクスチャフォーマット,RenderSurface,RenderTextueの限界。
//    D3DFMT_A8,
    D3DFMT_P8,
//    D3DFMT_R3G3B2,

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
//    D3DFMT_G16R16,

    D3DFMT_A16B16G16R16,

    D3DFMT_DXT1,
    D3DFMT_DXT2,
    D3DFMT_DXT3,
    D3DFMT_DXT4,
    D3DFMT_DXT5,
    D3DFMT_R16F,
    D3DFMT_G16R16F,
    D3DFMT_A16B16G16R16F,
//    D3DFMT_R32F,
//    D3DFMT_G32R32F,
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

  SPD3D09 pD3D = GetD3D();
  const UINT AdapterID = m_CreateParams.AdapterOrdinal;
  const D3DDEVTYPE DeviceType = m_CreateParams.DeviceType;
  D3DDISPLAYMODE CurrentDiplayMode;
  D3DCAPS9 d3dCaps;

  {
    const HRESULT ret = m_pDevice->GetDeviceCaps( &d3dCaps );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetDeviceCaps()")); }
  }

  {
    const HRESULT ret = m_pDevice->GetDisplayMode( 0, &CurrentDiplayMode );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetDisplayMode()")); }
  }

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

      if( ret==D3D_OK ) 
      {
        PIXELFORMAT fmt = D3DFORMATtoPIXELFORMAT(fmt);
        if( fmt!=PIXELFORMAT_NONE ) { caps.Texture.push_back(fmt);  }
      }
    }

    caps.MaxTextureSize = SIZE2DI(d3dCaps.MaxTextureWidth,d3dCaps.MaxTextureHeight);
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
      if( ret==D3D_OK ) 
      {
        PIXELFORMAT fmt = D3DFORMATtoPIXELFORMAT(fmt);
        if( fmt!=PIXELFORMAT_NONE ) { caps.RenderTargetTexture.push_back(fmt);  }
      }
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

      if( ret==D3D_OK ) 
      {
        PIXELFORMAT fmt = D3DFORMATtoPIXELFORMAT(fmt);
        if( fmt!=PIXELFORMAT_NONE ) { caps.RenderTargetSurface.push_back(fmt);  }
      }
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

      if( ret==D3D_OK ) 
      {
        PIXELFORMAT fmt = D3DFORMATtoPIXELFORMAT(fmt);
        if( fmt!=PIXELFORMAT_NONE ) { caps.DepthStencil.push_back(fmt);  }
      }
    }
  }

  {
    //  作成できるindexフォーマットを調べる
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


SPD3D09 DeviceD3D09::GetD3D()
{
  SPD3D09 pD3D;
  {
    IDirect3D9* p;
    const HRESULT ret = m_pDevice->GetDirect3D( &p );
    if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::GetDirect3D()")); }
    pD3D.reset( p );
  }

  return pD3D;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! GraphicsDeviceD3D09::Flip() を参照
void DeviceD3D09::Present()
{
	const HRESULT ret = m_pDevice->Present( NULL, NULL, NULL, NULL );

 #pragma  COMPILERMSG( "TODO:ここにデバイスの復帰チェック" )

//	if( ret==D3DERR_DEVICELOST ) { throw DeviceLost(); }
//	ef( FAILED(ret) ) { MAID_THROWEXCEPTION( MAIDTEXT("IDirect3DDevice9::Present") ); }
}


SCREENMODE DeviceD3D09::GetScreenMode()
{
  D3DDISPLAYMODE mode={0};

  {
    const HRESULT ret = m_pDevice->GetDisplayMode( 0, &mode );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetDisplayMode()")); }
  }

  SCREENMODE ret;

  ret.DisplayMode.Size = SIZE2DI(mode.Width,mode.Height);
  ret.DisplayMode.RefreshRate = mode.RefreshRate;
  ret.DisplayMode.Format = D3DFORMATtoPIXELFORMAT(mode.Format);
  ret.IsFullScreen = m_CurrentMode.Windowed==FALSE;
  ret.IsWaitVSync  = m_CurrentMode.PresentationInterval!=D3DPRESENT_INTERVAL_IMMEDIATE;

  return ret;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! VideoDeviceD3D09::Reset() 参照
void	DeviceD3D09::SetScreenMode( const SCREENMODE& Mode )
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


SPDRAWCOMMANDEXECUTE DeviceD3D09::GetDrawCommandExecute()
{
  return m_pDrawCommandExecute;
}

SPDRAWCOMMANDCAPTURE DeviceD3D09::CreateDrawCommandCapture()
{
  return SPDRAWCOMMANDCAPTURE();
}

SPRENDERTARGET DeviceD3D09::GetDefaultRenderTarget()const
{
  IDirect3DSurface9* pTarget=NULL;

  {
    SPD3D09SWAPCHAIN pSwap;
    {
      IDirect3DSwapChain9* p;
      const HRESULT ret = m_pDevice->GetSwapChain( 0, &p );
    	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::GetSwapChain()")); goto MAID_ERROR; }
      pSwap.reset( p );
    }

    const HRESULT ret = pSwap->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pTarget );
   	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::GetSwapChain()")); goto MAID_ERROR; }
  }
  {
    D3DSURFACE_DESC desc;
    pTarget->GetDesc( &desc );

    SPTEXTURE2D pTex;
    {
      CREATERETEXTURE2DPARAM param;

      {
        param.ArraySize = 1;
        param.MipLevels = 0;
        param.Format    = D3DFORMATtoPIXELFORMAT(desc.Format);
        param.Sample.Count   = desc.MultiSampleType;
        param.Sample.Quality = desc.MultiSampleQuality;
        param.BindFlags = D3DUSAGEtoRESOURCEBINDFLAG(desc.Usage);
        param.Usage = RESOURCEUSAGE_GPUWRITE;
        param.CPUAccessFlags = 0;
        param.MiscFlags = 0;
      }

      pTex.reset( new Texture2DD3D09(param,pTarget) );
    }

    SPRENDERTARGET pRet;
    {
      CREATERENDERTARGETPARAM param;
      {
        param.Format    = D3DFORMATtoPIXELFORMAT(desc.Format);
        param.Dimension = CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2D;
      }
      pRet.reset( new RenderTargetD3D09(pTex,param) );
    }

    return pRet;
  }

MAID_ERROR:
  if( pTarget!=NULL ) { pTarget->Release(); }
  return SPRENDERTARGET();
}

SPDEPTHSTENCIL DeviceD3D09::GetDefaultDepthStencil()const
{
  D3DSURFACE_DESC desc;
  m_pDefaultDepthStencil->GetDesc( &desc );

  SPTEXTURE2D pTex;
  {
    CREATERETEXTURE2DPARAM param;

    {
      param.ArraySize = 1;
      param.MipLevels = 0;
      param.Format    = D3DFORMATtoPIXELFORMAT(desc.Format);
      param.Sample.Count   = desc.MultiSampleType;
      param.Sample.Quality = desc.MultiSampleQuality;
      param.BindFlags = D3DUSAGEtoRESOURCEBINDFLAG(desc.Usage);
      param.Usage = RESOURCEUSAGE_GPUWRITE;
      param.CPUAccessFlags = 0;
      param.MiscFlags = 0;
    }

    pTex.reset( new Texture2DD3D09(param,m_pDefaultDepthStencil) );
  }

  SPDEPTHSTENCIL pRet;
  {
    CREATEDEPTHSTENCILPARAM param;
    {
      param.Format    = D3DFORMATtoPIXELFORMAT(desc.Format);
      param.Dimension = CREATEDEPTHSTENCILPARAM::DIMENSION_TEXTURE2D;
      param.ReadOnlyUsage = CREATEDEPTHSTENCILPARAM::FLAG_READ_ONLY_DEPTH;

      if( WithStencil(param.Format) )
      {
        param.ReadOnlyUsage |= CREATEDEPTHSTENCILPARAM::FLAG_READ_ONLY_STENCIL;
      }
    }
    pRet.reset( new DepthStencilD3D09(pTex,param) );
  }

  return pRet;
}


}}


