#include"deviced3d09.h"

#include"texture2dd3d09.h"
#include"rendertargetd3d09.h"
#include"depthstencild3d09.h"
#include"drawcommandexecuted3d09.h"
#include"id3d09object.h"
#include"debug.h"

#include<set>


bool std::less<D3DDISPLAYMODE>::operator ()( const D3DDISPLAYMODE& lhs, const D3DDISPLAYMODE& rhs )const
{
	if( lhs.Width < rhs.Width ) { return true;  }
	else if( lhs.Width > rhs.Width ) { return false; }

	if( lhs.Height < rhs.Height ) { return true;  }
	else if( lhs.Height > rhs.Height ) { return false; }

  if( lhs.RefreshRate < rhs.RefreshRate ) { return true;  }
  else if( lhs.RefreshRate > rhs.RefreshRate ) { return false; }

	return false;
}

namespace Maid { namespace Graphics {

DeviceD3D09::DeviceD3D09( const SPD3D09& pD3D, UINT adapter, bool IsSoftwareVertexProcessing, Window& win )
  :m_pDirect3D09(pD3D)
  ,m_Adapter(adapter)
  ,m_IsSoftwareVertexProcessing(IsSoftwareVertexProcessing)
  ,m_Window(win)
  ,m_ShaderCompilerDefault(NULL)
  ,m_ShaderCompilerLast(NULL)
  ,m_GetShaderVersion(NULL)
{
  ZERO( &m_CreateParams, sizeof(m_CreateParams) );
  ZERO( &m_DeviceCaps, sizeof(m_DeviceCaps) );
}



void DeviceD3D09::Initialize()
{
  {
    //  Shader Model2 のコードを流すので、古いバージョンではコンパイルできない。
    //  そのためある程度制限する
    const char* filename[] =
    {
      "d3dx9_40.dll",
      "d3dx9_39.dll",
      "d3dx9_38.dll",
      "d3dx9_37.dll",
      "d3dx9_36.dll",
      "d3dx9_35.dll",
      "d3dx9_34.dll",
      "d3dx9_33.dll",
      "d3dx9_32.dll",
    };

    for( int i=0; i<NUMELEMENTS(filename); ++i )
    {
      if( m_D3DX9_LastDLL.Load( String::ConvertSJIStoMAID(filename[i]) ) == DllWrapper::LOADRETURN_ERROR ) { continue; }

      m_ShaderCompilerDefault = (SHADERCOMPILE)m_D3DX9_LastDLL.GetProcAddress( MAIDTEXT("D3DXCompileShader") );
      break;
    }

    if( m_ShaderCompilerDefault==NULL )
    {
      ::MessageBox( NULL, L"D3DX9.dllが見つかりませんでした。\nDirectXランタイムをインストールしてください。", L"エラー", MB_OK );
      MAID_WARNING( "d3dx9_XX.dllがありません" );
    }

    m_GetShaderVersion = (GETSHADERVERSION)m_D3DX9_LastDLL.GetProcAddress( MAIDTEXT("D3DXGetShaderVersion") );
  }

  {
    m_ShaderCompilerLast = m_ShaderCompilerDefault;
  }

  {
    const UINT adapter = m_Adapter;
    const D3DDEVTYPE type = D3DDEVTYPE_HAL;
    const HWND hWnd = m_Window.GetHWND();
    const DWORD Flags = D3DCREATE_MULTITHREADED | (m_IsSoftwareVertexProcessing? D3DCREATE_SOFTWARE_VERTEXPROCESSING : D3DCREATE_MIXED_VERTEXPROCESSING); 
    D3DPRESENT_PARAMETERS param={0};
    param.BackBufferWidth = m_Window.GetClientSize().w;
    param.BackBufferHeight = m_Window.GetClientSize().h;
    param.BackBufferFormat = D3DFMT_UNKNOWN;
    param.BackBufferCount = 1;
    param.MultiSampleType = D3DMULTISAMPLE_NONE;  //  ウィンドウモードではマルチサンプルできない
    param.MultiSampleQuality = 0;
    param.SwapEffect = D3DSWAPEFFECT_DISCARD;
    param.hDeviceWindow = hWnd;
    param.Windowed = TRUE;
    param.EnableAutoDepthStencil = FALSE;
    param.Flags = 0;
    param.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    param.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

    IDirect3DDevice9* p=NULL;
    HRESULT ret = m_pDirect3D09->CreateDevice(adapter,type,hWnd,Flags, &param, &p );
    if( FAILED(ret) ) 
    {
      //  失敗すると、param に適切な値が入っているので、もう一回作る
      ret = m_pDirect3D09->CreateDevice(adapter,type,hWnd,Flags, &param, &p );

      if( FAILED(ret) ) 
      {
        //  それでもダメな場合は頂点処理をソフトウェアでやってみる
        ret = m_pDirect3D09->CreateDevice(adapter,type,hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING, &param, &p );
        if( FAILED(ret) ) 
        {
          //  これでもダメならアウト
          MAID_WARNING(MAIDTEXT("IDirect3D::CreateDevice()"));
          return ;
        }
      }
    }

    m_pDevice.reset(p);
  }

  {
    const HMONITOR hMon = m_pDirect3D09->GetAdapterMonitor( m_Adapter );
    MONITORINFO info;
    info.cbSize = sizeof(info);
    ::GetMonitorInfo(hMon, &info);

    m_DefaultDisplayRect.x = info.rcMonitor.left;
    m_DefaultDisplayRect.y = info.rcMonitor.top;
    m_DefaultDisplayRect.w = info.rcMonitor.right - info.rcMonitor.left;
    m_DefaultDisplayRect.h = info.rcMonitor.bottom - info.rcMonitor.top;
  }


  m_pDevice->GetDeviceCaps( &m_DeviceCaps );


  D3DDISPLAYMODE CurrentDiplayMode;
  {
    const HRESULT ret = m_pDevice->GetDisplayMode( 0, &CurrentDiplayMode );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetDisplayMode()")); }
  }

  {
    const HRESULT ret = m_pDevice->GetCreationParameters( &m_CreateParams );
   	if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetCreationParameters()")); }
  }


  {
    const D3DPRESENT_PARAMETERS d3dparam = GetD3DPRESENT_PARAMETERS();
    SWAPCHAINFORMAT&  chain = m_SwapChainFormat;

    chain.Size = SIZE2DI( d3dparam.BackBufferWidth, d3dparam.BackBufferHeight );
    chain.Format = D3DFORMATtoPIXELFORMAT(CurrentDiplayMode.Format);
    chain.RefreshRate.Numerator = d3dparam.FullScreen_RefreshRateInHz;
    chain.RefreshRate.Denominator = 1;
    chain.ScanlineOrder = SWAPCHAINFORMAT::SCANLINEORDER_PROGRESSIVE;
    chain.Scaling = SWAPCHAINFORMAT::SCALING_STRETCHED;
  }

  m_WindowModeStyle = m_Window.GetStyle();

  m_pDrawCommandExecute.reset(new DrawCommandExecuteD3D09(*this));
}



void DeviceD3D09::Finalize()
{
  m_pDevice.reset();
}

float DeviceD3D09::GetTexelMapValue() const
{
  return 0.5f;
}

void DeviceD3D09::SerchSwapChainSurfaceFormat( std::vector<PIXELFORMAT>& mode )  const
{
  const SPD3D09& pD3D  = GetD3D();
  const UINT AdapterID = m_Adapter;

  const UINT CountR5G6B5   = pD3D->GetAdapterModeCount( m_Adapter, D3DFMT_R5G6B5 );
  const UINT CountX1R5G5B5 = pD3D->GetAdapterModeCount( m_Adapter, D3DFMT_X1R5G5B5 );
  const UINT CountX8R8G8B8 = pD3D->GetAdapterModeCount( m_Adapter, D3DFMT_X8R8G8B8 );
  const UINT CountA2R10G10B10 = pD3D->GetAdapterModeCount( m_Adapter, D3DFMT_A2R10G10B10 );

  if( 0<CountR5G6B5   ) { mode.push_back(PIXELFORMAT_R05G06B05I); }
  if( 0<CountX1R5G5B5 ) { mode.push_back(PIXELFORMAT_X01R05G05B05I); }
  if( 0<CountX8R8G8B8 ) { mode.push_back(PIXELFORMAT_X08R08G08B08I); }

  #pragma COMPILERMSG("うまく動作する気配がないのでコメントアウトしておく")	
  //  if( 0<CountA2R10G10B10 ) { mode.push_back(PIXELFORMAT_A02R10G10B10I); }
}


D3DPRESENT_PARAMETERS DeviceD3D09::GetD3DPRESENT_PARAMETERS() const
{
  D3DPRESENT_PARAMETERS param ={};

  SPD3D09SWAPCHAIN pSwap;
  {
    IDirect3DSwapChain9* p;
    const HRESULT ret = m_pDevice->GetSwapChain( 0, &p );
  	if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetSwapChain()")); return param; }
    pSwap.reset( p );
  }

  const HRESULT ret = pSwap->GetPresentParameters( &param );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DSwapChain9::GetPresentParameters()")); }

  return param;
}


bool DeviceD3D09::IsFullScreen() const
{
  const D3DPRESENT_PARAMETERS param = GetD3DPRESENT_PARAMETERS();
  return param.Windowed == FALSE;
}

void DeviceD3D09::SetFullScreenState( bool IsFull )
{
  //  D3D9 は 1display / 1device 

  //  win  -> win : 何もしない
  //  full -> full: ディスプレイが１つしかないのが仕様なのでなにもしない
  //  win  -> full: 変更
  //  full -> win : 変更

  const bool NowFull = IsFullScreen();
  const bool NexFull = IsFull;

  if( NowFull==NexFull ) { return ; }

  const SWAPCHAINFORMAT fmt = m_SwapChainFormat;
  SetSwapChainFormat( NexFull, fmt );
}



void DeviceD3D09::SetSwapChainFormat( const SWAPCHAINFORMAT& mode )
{
  D3DPRESENT_PARAMETERS m = GetD3DPRESENT_PARAMETERS();

  SetSwapChainFormat( m.Windowed==FALSE, mode );
}

void DeviceD3D09::SetSwapChainFormat( bool IsFullScreen, const SWAPCHAINFORMAT& mode )
{
  D3DPRESENT_PARAMETERS m = {};

  {
    //  共通設定
    m.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m.hDeviceWindow = m_Window.GetHWND();
    m.EnableAutoDepthStencil = FALSE;
    m.Flags = 0;
    m.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    m.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
  }

  //  指定されたモードに一番近いモードを作成する
  if( IsFullScreen )
  {
    D3DDISPLAYMODE base;
    base.Width  = mode.Size.w;
    base.Height = mode.Size.h;
    base.Format = PIXELFORMATtoD3DFORMAT(mode.Format);
    base.RefreshRate = 0;

    const D3DDISPLAYMODE ret = FindScreenMode( base );

    m.BackBufferWidth  = ret.Width;
    m.BackBufferHeight = ret.Height;
    m.BackBufferFormat = ret.Format;
    m.BackBufferCount = 1;
    m.Windowed = FALSE;
  }else
  {
     //  ウィンドウモードなら何でもＯＫ
    m.BackBufferWidth  = mode.Size.w;
    m.BackBufferHeight = mode.Size.h;
    m.BackBufferFormat = D3DFMT_UNKNOWN;
    m.BackBufferCount = 1;
    m.MultiSampleType = D3DMULTISAMPLE_NONE;
    m.MultiSampleQuality = 0;
    m.Windowed = TRUE;
  }

  //  mode.Scaling と mode.ScanlineOrder は無視
  ScreenReset( m );

  m_SwapChainFormat = mode;
}


void DeviceD3D09::SerchEnableFormat( ENABLEFORMAT& caps ) const
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
  D3DCAPS9 d3dCaps;

  D3DFORMAT SwapFMT = D3DFMT_UNKNOWN;

  {
    const HRESULT ret = m_pDevice->GetDeviceCaps( &d3dCaps );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetDeviceCaps()")); }
  }

  {
    D3DDISPLAYMODE CurrentDiplayMode;
    const HRESULT ret = m_pDevice->GetDisplayMode( 0, &CurrentDiplayMode );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::GetDisplayMode()")); }

    SwapFMT = CurrentDiplayMode.Format;
  }

  {
    //  作成できるテクスチャのフォーマットを調べる
    for( int i=0; i<NUMELEMENTS(s_TEXTURE_FORMAT); ++i )
    {
      const D3DFORMAT fmt = s_TEXTURE_FORMAT[i];
      const HRESULT ret = pD3D->CheckDeviceFormat(
                                      AdapterID, DeviceType, SwapFMT,
                                      0, D3DRTYPE_TEXTURE,
                                      fmt                                      
                                      );

      if( SUCCEEDED(ret) ) 
      {
        PIXELFORMAT f = D3DFORMATtoPIXELFORMAT(fmt);
        if( fmt!=PIXELFORMAT_NONE ) { caps.Texture.push_back(f);  }
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
                                      AdapterID, DeviceType, SwapFMT,
                                      D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE,
                                      fmt                                      
                                      );
      if( SUCCEEDED(ret) ) 
      {
        PIXELFORMAT f = D3DFORMATtoPIXELFORMAT(fmt);
        if( fmt!=PIXELFORMAT_NONE ) { caps.RenderTargetTexture.push_back(f);  }
      }
    }
  }
  {
    //  作成できるレンダーサーフェスのフォーマットを調べる
    for( int i=0; i<NUMELEMENTS(s_TEXTURE_FORMAT); ++i )
    {
      const D3DFORMAT fmt = s_TEXTURE_FORMAT[i];
      const HRESULT ret = pD3D->CheckDeviceFormat(
                                      AdapterID, DeviceType, SwapFMT,
                                      D3DUSAGE_RENDERTARGET, D3DRTYPE_SURFACE,
                                      fmt                                      
                                      );

      if( SUCCEEDED(ret) ) 
      {
        PIXELFORMAT f = D3DFORMATtoPIXELFORMAT(fmt);
        if( fmt!=PIXELFORMAT_NONE ) { caps.RenderTargetSurface.push_back(f);  }
      }
    }
  }

  {
    //  作成できるDepthStencilフォーマットを調べる
    for( int i=0; i<NUMELEMENTS(s_DEPTH_STENCIL_FORMAT); ++i )
    {
      const D3DFORMAT fmt = s_DEPTH_STENCIL_FORMAT[i];
      const HRESULT ret = pD3D->CheckDeviceFormat(
                                      AdapterID, DeviceType, SwapFMT,
                                      D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE,
                                      fmt                                      
                                      );

      if( SUCCEEDED(ret) ) 
      {
        PIXELFORMAT f = D3DFORMATtoPIXELFORMAT(fmt);
        if( fmt!=PIXELFORMAT_NONE ) { caps.DepthStencil.push_back(f);  }
      }
    }
  }

  {
    //  作成できるindexフォーマットを調べる
    if( d3dCaps.MaxVertexIndex <= 0x0000FFFF )
    {
      caps.IndexBit.push_back(16); 
    }else
    {
      caps.IndexBit.push_back(16); 
      caps.IndexBit.push_back(32); 
    }
  }
}


const SPD3D09& DeviceD3D09::GetD3D()const
{
  return m_pDirect3D09;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! GraphicsDeviceD3D09::Flip() を参照
void DeviceD3D09::Present()
{
	const HRESULT ret = m_pDevice->Present( NULL, NULL, NULL, NULL );

  switch( ret )
  {
  case D3DERR_DRIVERINTERNALERROR:
  case D3DERR_DEVICELOST:
    {
      switch( m_pDevice->TestCooperativeLevel() )
      {
      case D3DERR_DEVICELOST:     { }break;
      case D3DERR_DEVICENOTRESET: 
        {
          ScreenReset( GetD3DPRESENT_PARAMETERS() );
        }break;
        /* そのほかは正常 */
      }

    }break;

  case D3DERR_INVALIDCALL:
    {
      MAID_WARNING(MAIDTEXT("IDirect3DDevice9::Present()"));
    }break;

  }
}

SWAPCHAINFORMAT DeviceD3D09::GetSwapChainFormat() const
{
  return m_SwapChainFormat;
}

SIZE2DI DeviceD3D09::GetDisplayAspect() const
{
  return m_DefaultDisplayRect.GetSize();
}


void	DeviceD3D09::ScreenReset( const D3DPRESENT_PARAMETERS& NewParam )
{
  const bool NowFull = IsFullScreen();
  const bool NexFull = NewParam.Windowed == FALSE;

	for( int i=0; i<8; ++i ) { m_pDevice->SetTexture( i, NULL ); }
	for( int i=0; i<8; ++i ) { m_pDevice->SetStreamSource( i, NULL, 0, 0 ); }

	m_pDevice->SetIndices( NULL );
	m_pDevice->SetPixelShader( NULL );
	m_pDevice->SetVertexShader( NULL );
	m_pDevice->SetVertexDeclaration( NULL );

  {
    //  vramにしかないやつを全部退避する
    ID3D09Object::EscapeAll();
  }


  D3DPRESENT_PARAMETERS p = NewParam; //  切り替え後に値が変わることもあるのでコピーしておく
  { //  これでやっと解像度変更

    const D3DPRESENT_PARAMETERS old = GetD3DPRESENT_PARAMETERS();

    const HRESULT ret = m_pDevice->Reset( &p );
    if( ret==D3DERR_DRIVERINTERNALERROR ) { MAID_WARNING("ドライバエラーです。復帰できません"); return; }
    else if( FAILED(ret) )
    {
      MAID_WARNING("IDirect3DDevice9::Reset() " << ret << " " << DebugStringD3D09(p));
      p = old;
      m_pDevice->Reset( &p );
      ID3D09Object::RestoreAll( *this );
      return ;
    }
  }
  if( !NowFull && NexFull )
  {
    //  ウィンドウ -> フルスクリーンのときは
    //  ウィンドウの変更

/*
    m_Window.SetSize( SIZE2DI(NewParam.BackBufferWidth, NewParam.BackBufferHeight) );
    m_WindowModeStyle = m_Window.GetStyle();
    m_Window.SetStyle( WS_POPUP|WS_EX_TOPMOST );
    m_Window.SetPosition( m_DefaultDisplayRect.GetPoint() );
    m_Window.SetZOrder( HWND_TOPMOST );
*/
    //  ↑と同じことをやってるはずなんだけど、こうしないと動かない。はて？
    const POINT2DI pos = m_DefaultDisplayRect.GetPoint();
    m_WindowModeStyle = m_Window.GetStyle();
		::SetWindowLong( m_Window.GetHWND(), GWL_STYLE, WS_POPUP|WS_EX_TOPMOST );
		::SetWindowPos( m_Window.GetHWND(), HWND_TOPMOST,
				pos.x, pos.y,
				NewParam.BackBufferWidth, NewParam.BackBufferHeight,
				SWP_DRAWFRAME|SWP_SHOWWINDOW );
  }
  else if( NowFull && !NexFull )
  {
    //  フルスクリーン -> ウィンドウのときは
    //  自分のスタイルの変更
    m_Window.SetZOrder( HWND_NOTOPMOST );
    m_Window.SetStyle( m_WindowModeStyle );
    m_Window.SetClientSize( SIZE2DI(p.BackBufferWidth, p.BackBufferHeight) );

    const SIZE2DI window = m_Window.GetSize();
    const SIZE2DI screen = m_DefaultDisplayRect.GetSize();
    const int x = m_DefaultDisplayRect.x + (screen.w - window.w)/2;
    const int y = m_DefaultDisplayRect.y + (screen.h - window.h)/2;
    m_Window.SetPosition( POINT2DI(x,y) );
  }
  else if( NowFull==NexFull )
  {
    //  状態の変更がない場合はウィンドウサイズを変更する
    m_Window.SetClientSize( SIZE2DI(p.BackBufferWidth, p.BackBufferHeight) );
  }

  {
    //  で、戻ってきてもらうー
    ID3D09Object::RestoreAll( *this );
  }
}


SPDRAWCOMMANDEXECUTE DeviceD3D09::GetDrawCommandExecute()const
{
  return m_pDrawCommandExecute;
}

SPDRAWCOMMANDCAPTURE DeviceD3D09::CreateDrawCommandCapture()
{
  MAID_ASSERT( true, "未実装" );
  return SPDRAWCOMMANDCAPTURE();
}

SPRENDERTARGET DeviceD3D09::GetCurrentRenderTarget()const
{
  SPD3D09SURFACE pTarget;
  {
    SPD3D09SWAPCHAIN pSwap;
    {
      IDirect3DSwapChain9* p=NULL;
      const HRESULT ret = m_pDevice->GetSwapChain( 0, &p );
    	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::GetSwapChain()")); goto MAID_ERROR; }
      pSwap.reset( p );
    }

    IDirect3DSurface9* p=NULL;
    const HRESULT ret = pSwap->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &p );
   	if( FAILED(ret) ) { MAID_THROWEXCEPTION(MAIDTEXT("IDirect3DDevice9::GetBackBuffer()")); goto MAID_ERROR; }
    pTarget.reset( p );
  }
  {
    D3DSURFACE_DESC desc;
    pTarget->GetDesc( &desc );

    SPTEXTURE2D pTex;
    {
      CREATERETEXTURE2DPARAM param;
      {
        param.Size = SIZE2DI(desc.Width,desc.Height);
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

      pTex.reset( new Texture2DD3D09(param,pTarget,true) );
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
  return SPRENDERTARGET();
}

D3DDISPLAYMODE DeviceD3D09::FindScreenMode( const D3DDISPLAYMODE& base )
{
  //  フルスクリーンモードはそれに一番近いモードにする
  //  いったん使えるディスプレイモードを全部出して、ソートして
  //  使えるのを見る

  D3DDISPLAYMODE ret;

  SPD3D09 pD3D = GetD3D();
  const UINT AdapterID = m_CreateParams.AdapterOrdinal;
  const D3DFORMAT fmt  = GetD3DPRESENT_PARAMETERS().BackBufferFormat;

  const UINT ModeMAX = pD3D->GetAdapterModeCount( AdapterID, fmt );

  typedef std::set<D3DDISPLAYMODE,std::less<D3DDISPLAYMODE> > SORTLIST;
  SORTLIST  SortList;

  for( UINT i=0; i<ModeMAX; ++i )
  {
    D3DDISPLAYMODE mode;
    const HRESULT ret = pD3D->EnumAdapterModes( AdapterID, fmt, i, &mode );

    if( ret==D3D_OK && base.Format==fmt )
    {
      SortList.insert( mode );
    }
  }

  for( SORTLIST::iterator ite=SortList.begin(); ite!=SortList.end(); ++ite )
  {
    const D3DDISPLAYMODE& m = *ite;
    if( m.Width  < base.Width ) { continue; }
    if( m.Height < base.Height ) { continue; }

    #pragma COMPILERMSG("リフレッシュレートはどうするか？　現状はデフォルト動作に頼る")	
    ret = m;
    ret.RefreshRate = 0;
    break;
  }
  return ret;
}


}}


