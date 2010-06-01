#include"deviceopengl.h"


#include"texture2dopengl.h"
#include"rendertargetopengl.h"
/*
#include"depthstencild3d11.h"
*/
#include"drawcommandexecuteopengl.h"

namespace Maid { namespace Graphics {



DeviceOpenGL::DeviceOpenGL( Window& win )
  :m_Window(win)
  ,m_hDC(NULL)
  ,m_hGLContext(NULL)
{

}

void DeviceOpenGL::Initialize()
{
  {
    m_OpenGLDLL.Initialize();
  }

  m_hDC = ::GetDC( m_Window.GetHWND() );

	PIXELFORMATDESCRIPTOR pfd;
  {
    ZERO( &pfd, sizeof(pfd) );

	  pfd.nSize = sizeof( pfd );
	  pfd.nVersion = 2;
	  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	  pfd.iPixelType = PFD_TYPE_RGBA;
	  pfd.cColorBits = 32;
	  pfd.cDepthBits = 16;
	  pfd.iLayerType = PFD_MAIN_PLANE;
  }

  const int pixelformat = ChoosePixelFormat( m_hDC, &pfd );
	SetPixelFormat( m_hDC, pixelformat, &pfd );

	m_hGLContext = m_OpenGLDLL.wglCreateContext( m_hDC );

  m_OpenGLDLL.wglMakeCurrent( m_hDC, m_hGLContext );

//  ↓はやったほうがいいのかな？　pfdの値が変わる
//  DescribePixelFormat(m_hDC, pixelformat,  sizeof(PIXELFORMATDESCRIPTOR), &pfd);

  {
    m_OpenGLExtension.Initialize( m_OpenGLDLL );

    IOpenGLObject::SetRuntime( &m_OpenGLDLL, &m_OpenGLExtension );
  }
/*
  {
    SPD3D11DEVICECONTEXT pContext;
    {
      ID3D11DeviceContext* p;
      m_pDevice->GetImmediateContext( &p );
      pContext.reset(p);
    }

    //  D3D11はビューポートの設定を自動的にやってくれない
    D3D11_VIEWPORT vp;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    vp.Width    = m_Window.GetClientSize().w;
    vp.Height   = m_Window.GetClientSize().h;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    pContext->RSSetViewports( 1, &vp );

    m_pDrawCommandExecute.reset(new DrawCommandExecuteD3D11(pContext,GetCurrentRenderTarget()));
  }
*/
  {
    const int x = 0;
    const int y = 0;
    const int w = m_Window.GetClientSize().w;
    const int h = m_Window.GetClientSize().h;
    const float minz = 0.0f;
    const float maxz = 0.0f;

    m_OpenGLDLL.glViewport( x, y, w, h );
    m_OpenGLDLL.glDepthRange( minz, maxz );
  }

  {
    ZERO( &m_SwapChainFormat, sizeof(m_SwapChainFormat) );

    SWAPCHAINFORMAT& fmt = m_SwapChainFormat;
    fmt.Size.w = m_Window.GetClientSize().w;
    fmt.Size.h = m_Window.GetClientSize().h;

    switch( pfd.cColorBits )
    {
    case 32: { fmt.Format = PIXELFORMAT_X08R08G08B08I; }break;
    default: { MAID_ASSERT( true, "未実装" ); }break;
    }

    fmt.MultiSample.Count = 0;
    fmt.MultiSample.Quality = 1;

    fmt.RefreshRate.Denominator = 1;
    fmt.RefreshRate.Numerator   = 0;

    fmt.ScanlineOrder   = SWAPCHAINFORMAT::SCANLINEORDER_PROGRESSIVE;
    fmt.Scaling   = SWAPCHAINFORMAT::SCALING_STRETCHED;
    fmt.SyncInterval   = 1;
  }
  UpdateDisplayAspect();

  {
    DrawCommandExecuteOpenGL* p = new DrawCommandExecuteOpenGL(m_OpenGLDLL, m_OpenGLExtension);
    p->Initialize();

    m_pDrawCommandExecute.reset(p);
  }
}




void DeviceOpenGL::Finalize()
{
  m_pDrawCommandExecute.reset();

	m_OpenGLDLL.wglMakeCurrent( NULL, NULL );
	m_OpenGLDLL.wglDeleteContext( m_hGLContext );
  ::ReleaseDC( m_Window.GetHWND(), m_hDC );

	IOpenGLObject::SetRuntime(NULL, NULL);

/*
  if( IsFullScreen() ) { m_pSwapChain->SetFullscreenState( FALSE, NULL ); }
  m_pSwapChain.reset();

  m_pDevice.reset();
*/
}


void DeviceOpenGL::UpdateDisplayAspect()
{
  const POINT2DI pos = m_Window.GetPosition();

  POINT p = { pos.x, pos.y };

  const HMONITOR hMon = ::MonitorFromPoint( p, MONITOR_DEFAULTTONEAREST );


  MONITORINFO info;
  info.cbSize = sizeof(info);
  ::GetMonitorInfo(hMon, &info);

  const int w = info.rcMonitor.right - info.rcMonitor.left;
  const int h = info.rcMonitor.bottom - info.rcMonitor.top;

  m_DisplayAspect = SIZE2DI(w,h);
}



void DeviceOpenGL::SetFullScreenState( bool IsFull )
{
/*
  const bool NowFull = IsFullScreen();

  if( NowFull==IsFull ) { return ; }

  if( IsFull )
  {
    //  ウィンドウ -> フルスクリーンの時は、対象先の解像度を覚えておく
    UpdateDisplayAspect();
  }
  {
    const BOOL full = IsFull? TRUE:FALSE;
    const HRESULT ret = m_pSwapChain->SetFullscreenState( full, NULL );
    if( FAILED(ret) ) { MAID_WARNING("IDXGISwapChain::SetFullscreenState() " ); return ; }
  }
*/
}

float DeviceOpenGL::GetTexelMapValue() const
{
  return 0.0f;
}


void DeviceOpenGL::SerchSwapChainSurfaceFormat( std::vector<PIXELFORMAT>& mode )  const
{
/*
  static const DXGI_FORMAT TestFMT[] =
  {
    DXGI_FORMAT_R32G32B32A32_FLOAT,
    DXGI_FORMAT_R16G16B16A16_FLOAT,
    DXGI_FORMAT_R16G16B16A16_UNORM,
    DXGI_FORMAT_R10G10B10A2_UNORM,
    DXGI_FORMAT_R8G8B8A8_UNORM,
  };

  for( int i=0; i<NUMELEMENTS(TestFMT); ++i )
  {
    const DXGI_FORMAT fmt = TestFMT[i];
    UINT flag = 0;

    const HRESULT ret = m_pDevice->CheckFormatSupport( fmt, &flag );
    if( FAILED(ret) ) { continue; }

    const bool IsTextre2D = IsFlag( flag, D3D11_FORMAT_SUPPORT_TEXTURE2D );
    const bool IsRenderTarget = IsFlag( flag, D3D11_FORMAT_SUPPORT_RENDER_TARGET );

    if( IsTextre2D && IsRenderTarget )
    {
      mode.push_back( DXGI_FORMATtoPIXELFORMAT(fmt) );
    }
  }
*/
}

void DeviceOpenGL::SetSwapChainFormat( const SWAPCHAINFORMAT& mode )
{
/*
  DXGI_SWAP_CHAIN_DESC desc;
  const bool NowFull = IsFullScreen();

  {
    const HRESULT ret = m_pSwapChain->GetDesc( &desc );
    if( FAILED(ret) ) { MAID_WARNING("IDXGISwapChain::GetDesc() " << ret ); return ; }

    //  一回ウィンドウモードに戻してから開放しないとダメらしい
    if( NowFull ) { m_pSwapChain->SetFullscreenState( FALSE, NULL ); }
    m_pSwapChain.reset();
  }

  m_Window.SetClientSize( mode.Size );

  desc.BufferDesc.Width  = mode.Size.w;
  desc.BufferDesc.Height = mode.Size.h;
  desc.BufferDesc.RefreshRate.Numerator = 60;
  desc.BufferDesc.RefreshRate.Denominator = 1;
  desc.BufferDesc.Format = PIXELFORMATtoDXGI_FORMAT(mode.Format);
  desc.BufferDesc.ScanlineOrdering = SWAPCHAINFORMATSCANLINEORDERtoDXGI_MODE_SCANLINE_ORDER(mode.ScanlineOrder);
  desc.BufferDesc.Scaling = SWAPCHAINFORMATSCALINGtoDXGI_MODE_SCALING(mode.Scaling);
  desc.BufferCount = 2;

  desc.SampleDesc.Count   = mode.MultiSample.Count;
  desc.SampleDesc.Quality = mode.MultiSample.Quality;


  {
    IDXGISwapChain* p=NULL;
    const HRESULT ret = m_pFactory->CreateSwapChain( m_pDevice.get(), &desc, &p  );
    if( FAILED(ret) ) { MAID_WARNING("IDXGIFactory::CreateSwapChain " << ret ); }
    m_pSwapChain.reset(p);
  }
  
  if( NowFull ) { m_pSwapChain->SetFullscreenState( TRUE, NULL ); }

  m_SyncInterval = mode.SyncInterval;
*/
}


SWAPCHAINFORMAT DeviceOpenGL::GetSwapChainFormat() const
{
  SWAPCHAINFORMAT ret;

/*
  DXGI_SWAP_CHAIN_DESC desc;

  {
    const HRESULT ret = m_pSwapChain->GetDesc( &desc );
    if( FAILED(ret) ) { MAID_WARNING("IDXGISwapChain::GetDesc() " << ret ); return SWAPCHAINFORMAT(); }
  }

  ret.Size   = SIZE2DI( desc.BufferDesc.Width, desc.BufferDesc.Height );
  ret.Format = DXGI_FORMATtoPIXELFORMAT(desc.BufferDesc.Format);
  ret.MultiSample.Quality = desc.SampleDesc.Quality;
  ret.MultiSample.Count   = desc.SampleDesc.Count;
  ret.RefreshRate.Numerator   = desc.BufferDesc.RefreshRate.Numerator;
  ret.RefreshRate.Denominator = desc.BufferDesc.RefreshRate.Denominator;

  ret.Scaling       = DXGI_MODE_SCALINGtoDISPLAYMODESCALING( desc.BufferDesc.Scaling );
  ret.ScanlineOrder = DXGI_MODE_SCANLINE_ORDERtoSWAPCHAINFORMATSCANLINEORDER( desc.BufferDesc.ScanlineOrdering );

  ret.SyncInterval = m_SyncInterval;
*/
  return m_SwapChainFormat;
}

SIZE2DI DeviceOpenGL::GetDisplayAspect() const
{
  return m_DisplayAspect;
}


void DeviceOpenGL::SerchEnableFormat( ENABLEFORMAT& caps ) const
{
  MAID_ASSERT( !caps.Texture.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.DepthStencil.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.RenderTargetTexture.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.RenderTargetSurface.empty(), "引数はemptyにしてください" );
  MAID_ASSERT( !caps.IndexBit.empty(), "引数はemptyにしてください" );

  
  {
    caps.Texture.push_back(PIXELFORMAT_A08B08G08R08I);
    caps.MaxTextureSize = SIZE2DI(4096,4096);
  }
  {
    caps.DepthStencil.push_back(PIXELFORMAT_D16I);
  }
  {
    caps.RenderTargetTexture.push_back(PIXELFORMAT_A08B08G08R08I);
  }
  {
    caps.RenderTargetSurface.push_back(PIXELFORMAT_A08B08G08R08I);
  }
  {
    caps.IndexBit.push_back(16);
  }

  /*
  {
    caps.Texture.push_back(PIXELFORMAT_A08B08G08R08I);
    caps.Texture.push_back(PIXELFORMAT_A02B10G10R10I);
    caps.Texture.push_back(PIXELFORMAT_A16B16G16R16I);
    caps.Texture.push_back(PIXELFORMAT_A16B16G16R16F);
    caps.Texture.push_back(PIXELFORMAT_A32B32G32R32F);

    caps.MaxTextureSize = SIZE2DI(4096,4096);
  }
  {
    caps.DepthStencil.push_back(PIXELFORMAT_D16I);
    caps.DepthStencil.push_back(PIXELFORMAT_D24IS8);
  }
  {
    caps.RenderTargetTexture.push_back(PIXELFORMAT_A08B08G08R08I);
    caps.RenderTargetTexture.push_back(PIXELFORMAT_A02B10G10R10I);
    caps.RenderTargetTexture.push_back(PIXELFORMAT_A16B16G16R16I);
    caps.RenderTargetTexture.push_back(PIXELFORMAT_A16B16G16R16F);
    caps.RenderTargetTexture.push_back(PIXELFORMAT_A32B32G32R32F);
  }
  {
    caps.RenderTargetSurface.push_back(PIXELFORMAT_A08B08G08R08I);
    caps.RenderTargetSurface.push_back(PIXELFORMAT_A02B10G10R10I);
    caps.RenderTargetSurface.push_back(PIXELFORMAT_A16B16G16R16I);
    caps.RenderTargetSurface.push_back(PIXELFORMAT_A16B16G16R16F);
    caps.RenderTargetSurface.push_back(PIXELFORMAT_A32B32G32R32F);
  }
  {
    caps.IndexBit.push_back(16);
  }
*/
}

void DeviceOpenGL::Present()
{
  m_OpenGLDLL.glFlush();
  SwapBuffers( m_hDC );
}

SPDRAWCOMMANDEXECUTE DeviceOpenGL::GetDrawCommandExecute()const
{
  return m_pDrawCommandExecute;
}

SPDRAWCOMMANDCAPTURE DeviceOpenGL::CreateDrawCommandCapture()
{
  return SPDRAWCOMMANDCAPTURE();
}

SPRENDERTARGET DeviceOpenGL::GetCurrentRenderTarget()const
{
  SPTEXTURE2D pTex;
  {
    CREATERETEXTURE2DPARAM p;

    p.Size.w = m_Window.GetClientSize().w;
    p.Size.h = m_Window.GetClientSize().h;
    p.Format = PIXELFORMAT_X08R08G08B08I;
    p.Usage  = RESOURCEUSAGE_GPUWRITE;
    p.BindFlags = RESOURCEBINDFLAG_RENDERTARGET;

    pTex.reset( new Texture2DOpenGL(p) );
  }

  SPRENDERTARGET pTarget;

  {
    CREATERENDERTARGETPARAM p;
    p.Format = pTex->GetParam().Format;
    p.Dimension = CREATERENDERTARGETPARAM::DIMENSION_TEXTURE2D;

    pTarget.reset( new RenderTargetOpenGL(pTex,p) );
  }

  return pTarget;
}




bool DeviceOpenGL::IsFullScreen() const
{
/*
  DXGI_SWAP_CHAIN_DESC desc;
  const HRESULT ret = m_pSwapChain->GetDesc( &desc );
  if( FAILED(ret) ) { MAID_WARNING("IDXGISwapChain::GetDesc() " << ret ); return false; }

  return desc.Windowed == FALSE;
*/
  return false;
}


}}


