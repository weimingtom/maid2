#include"graphicscore.h"

#include"../auxiliary/debug/warning.h"



namespace Maid {



//  2の累乗を返すマクロ
inline int ExtendSize( int src )
{
  int now = 1;
  for( int i=0; i<20; ++i ) //  2^20
  {
    if( src<=now ) { return now; }
    now *= 2;
  }

  return now;
}

  // クオリティは
  //  0:変化なし
  //  1:横だけ 1/2
  //  2:縦だけ 1/2
  //  3:横縦   1/2
  //  4:横1/4, 縦 1/2
  //  5:横1/2, 縦 1/4
  //  6:横1/4, 縦 1/4
  //  という感じに小さくなっていく
inline SIZE2DI CalcQuality( int quality )
{
  static const SIZE2DI  s_QUALITYYABLE[] =
  {
    SIZE2DI( 1, 1),
    SIZE2DI( 2, 1),SIZE2DI( 1, 2),SIZE2DI( 2, 2),
    SIZE2DI( 4, 2),SIZE2DI( 2, 4),SIZE2DI( 4, 4),
    SIZE2DI( 8, 4),SIZE2DI( 4, 8),SIZE2DI( 8, 8),
    SIZE2DI(16, 8),SIZE2DI( 8,16),SIZE2DI(16,16),
    SIZE2DI(32,16),SIZE2DI(16,32),SIZE2DI(32,32),
  };
  MAID_ASSERT( quality<0, "拡大はサポートしていません" );
  MAID_ASSERT( !(quality<NUMELEMENTS(s_QUALITYYABLE)), "値が大きすぎます" );

  return s_QUALITYYABLE[quality];
}


  /*!
	 	@class Core core.h
	 	@brief  グラフィック周りを管理するクラス
	 */


GraphicsCore::GraphicsCore()
:m_IsAspectLock(false)
,m_IsTextureMipMap(true)
{
}

GraphicsCore::~GraphicsCore()
{
	Finalize();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 初期化
/*!
    このオブジェクトそのものの初期化なので、各種設定はここではやらない

    @param	pDriver	[i ]	使うデバイス
    @param	pFont 	[i ]	使うフォントデバイス
 */
void GraphicsCore::Initialize( const Graphics::SPDEVICE& pDevice, const Graphics::SPFONTDEVICE& pFont )
{
  GlobalPointer<GraphicsCore>::Set();

	m_pDevice = pDevice;
	m_pDevice->Initialize();

  {
    std::vector<PIXELFORMAT>  fmt;
  	m_pDevice->SerchSwapChainSurfaceFormat( fmt );

    for( int i=0; i<(int)fmt.size(); ++i ) { m_SwapChainSurfaceFormat.insert(fmt[i]); }
  }

  m_TextureQuality = 0;
  m_pFontDevice = pFont;
  UpdateMember();

  m_VirtualScreenSize = SIZE2DI(0,0);
}


//! 後始末
/*!
 */
void GraphicsCore::Finalize()
{
	if( m_pDevice.get()==NULL ) { return ; }

  m_DepthStencil.Destroy();
  m_Screen.Finalize();
	m_pDevice->Finalize();
	m_pDevice.reset();
  GlobalPointer<GraphicsCore>::Clear();
}

//! デバイスの取得
/*!
 */
const Graphics::SPDEVICE& GraphicsCore::GetDevice()const
{
#pragma  COMPILERMSG( "デバイスは外に触らせないほうがいいかも。要検討" )
  return m_pDevice;
}

//! デバイスの取得
/*!
 */
const Graphics::SPFONTDEVICE& GraphicsCore::GetFontDevice()const
{
#pragma  COMPILERMSG( "デバイスは外に触らせないほうがいいかも。要検討" )
  return m_pFontDevice;
}


//! 現在のスワップチェインの取得
/*!
    @return 現在の設定
 */
const GraphicsCore::SWAPCHAINFORMAT& GraphicsCore::GetSwapChainFormat() const
{
  return m_SwapChainFormat;
}



//! スワップチェインの変更
/*!
    @param  mode  [i ]  新しい設定
 */
void GraphicsCore::SetSwapChainFormat( const SWAPCHAINFORMAT& mode )
{
  Graphics::SWAPCHAINFORMAT NewMode;

  {
    //  ピクセルフォーマットを探す
    PIXELFORMAT fmt = PIXELFORMAT_X08R08G08B08I;  //  <- 安全策
    switch( mode.Type )
    {
    case SWAPCHAINFORMAT::BPP16:  { fmt = PIXELFORMAT_X01R05G05B05I;}break;
    case SWAPCHAINFORMAT::BPP32:  { fmt = PIXELFORMAT_X08R08G08B08I;}break;
    case SWAPCHAINFORMAT::BPP32EX:{ fmt = PIXELFORMAT_A02R10G10B10I;}break;
    default: { MAID_ASSERT( true, "想定外" ); } break;
    }

    #pragma COMPILERMSG("大は小をかねる検索だとまずいかもなー")	
    NewMode.Format = FindFormat( fmt, m_SwapChainSurfaceFormat );
  }

  NewMode.Size = mode.Size;
  NewMode.SyncInterval = mode.IsWaitVSync? 1 : 0;
  NewMode.RefreshRate.Numerator = 0;
  NewMode.RefreshRate.Denominator = 1;

  NewMode.ScanlineOrder = Graphics::SWAPCHAINFORMAT::SCANLINEORDER_PROGRESSIVE;
  NewMode.Scaling = Graphics::SWAPCHAINFORMAT::SCALING_STRETCHED;

  m_Screen.Finalize();
  m_pDevice->SetSwapChainFormat( NewMode );

  UpdateMember();
}

//! 仮想スクリーンモードを開始する
/*!
    RenderTarget等はここで設定した値を元に大きさが変わります

    @param  size  [i ]  仮想サイズ
 */
void GraphicsCore::BeginVirtualScreenMode( const SIZE2DI& size )
{
  m_VirtualScreenSize = size;
}

//! 仮想スクリーンモードを終了する
/*!
 */
void GraphicsCore::EndVirtualScreenMode()
{
  m_VirtualScreenSize = SIZE2DI(0,0);
}



//! スクリーンモードの変更をする
/*!
    @param  IsFullScreen  [i ]  true でフルスクリーン
 */
void GraphicsCore::SetFullScreenState( bool IsFullScreen )
{
  m_Screen.Finalize();
  m_pDevice->SetFullScreenState( IsFullScreen );
  UpdateMember();
  SetAspectLock( m_IsAspectLock );
}


const SIZE2DI& GraphicsCore::GetVirtualScreenSize() const
{
  if( m_VirtualScreenSize.w==0 && m_VirtualScreenSize.h==0 )
  {
    return m_SwapChainFormat.Size;
  }

  return m_VirtualScreenSize;
}


//! 現在フルスクリーン中か？
/*!
    @return フルスクリーン中なら true
 */
bool GraphicsCore::IsFullScreen() const
{
  return m_pDevice->IsFullScreen();
}


//! 作成できるテクスチャの限界サイズを取得する
/*!
    @return  大きさ
 */
const SIZE2DI& GraphicsCore::GetMaxTextureSize() const
{
  return m_EnableFormat.MaxTextureSize;
}


//! 指定した大きさから作成できるサイズを求める
/*!
    このクラス内の設定で動的に変化します。

    @param  size      [i ]  作成したい大きさ

    @return  作るべき大きさ
 */
SIZE2DI GraphicsCore::CalcTextureSize( const SIZE2DI& size ) const
{
  SIZE2DI ret(0,0);

  const SIZE2DI& SRCSIZE = size;
  const SIZE2DI& MAXSIZE = GetMaxTextureSize();
  ret.w = std::min( ExtendSize(SRCSIZE.w), MAXSIZE.w );
  ret.h = std::min( ExtendSize(SRCSIZE.h), MAXSIZE.h );

  return ret;
}

void GraphicsCore::CalcTextureSize( const SIZE2DI& VirtualSize, SIZE2DI& RealSize, SIZE2DI& TextureSize ) const
{
  const SIZE2DI level = CalcQuality( m_TextureQuality );

  const SIZE2DI CalcSize = SIZE2DI( VirtualSize.w/level.w, VirtualSize.h/level.h );
  const SIZE2DI tex = CalcTextureSize(CalcSize);


  {
    SIZE2DI area = CalcSize;

    if( area.w < VirtualSize.w ) { area.w = tex.w; }
    if( area.h < VirtualSize.h ) { area.h = tex.h; }

    RealSize = area;
    TextureSize = tex;
  }



  if( RealSize.w   <1 ) { RealSize.w    = 1; }
  if( RealSize.h   <1 ) { RealSize.h    = 1; }


  if( TextureSize.w<1 ) { TextureSize.w = 1; }
  if( TextureSize.h<1 ) { TextureSize.h = 1; }
}

//! テクスチャ品質の設定
/*!
    @param  level  [i ]  0が変更なし 最大15(汚くなる)まで。
 */
void GraphicsCore::SetTextureQuality( int level )
{
  m_TextureQuality = level;
}


//! アスペクト固定アルゴリズムの選択
/*!
    @param  IsLock  [i ]  trueで固定する
 */
void GraphicsCore::SetAspectLock( bool IsLock )
{
  SIZE2DI aspect(0,0);

  if( IsLock && IsFullScreen() )
  {
    aspect = m_pDevice->GetDisplayAspect();
  }

  m_Screen.SetAspectSize( aspect );
  m_IsAspectLock = IsLock;
}

bool GraphicsCore::IsAspectLock() const
{
  return m_IsAspectLock;
}

void GraphicsCore::SetTextureMipMap( bool use )
{
  m_IsTextureMipMap = use;
}

bool GraphicsCore::IsTextureMipMap() const
{
  return m_IsTextureMipMap;
}


//! スクリーンの取得
/*!
    @return スクリーン
 */
const RenderTargetBase& GraphicsCore::GetBackBuffer() const
{
  return m_Screen;
}

//! スクリーンと対をなすDepthStencilの取得
/*!
    @return DepthStencil
 */
const IDepthStencil&    GraphicsCore::GetDepthStencil() const
{
  return m_DepthStencil;
}

int GraphicsCore::GetTextureQuality() const
{
  return m_TextureQuality;
}

//! 指定したサイズのミップマップレベルを計算する
/*!
    @param size [i ]

    @return DepthStencil
 */
int GraphicsCore::CalcMipLevels( const SIZE2DI& size ) const
{
  int p = std::max( size.w, size.h );

  int ret = 0;
  while( true )
  {
    if( p==0 ) { break; }
    ++ret;
    p >>= 1;
  }

  return ret;
}


//! モニタに表示させる
/*!
 */
void GraphicsCore::Present()
{
  m_Screen.Present();
}


//! 現在の解像度で各種メンバを設定しなおす
/*!
 */
void GraphicsCore::UpdateMember()
{
  MAID_ASSERT( m_pDevice.get()==NULL, "デバイスが作成されていません"  );

  {
    const Graphics::SWAPCHAINFORMAT mode = m_pDevice->GetSwapChainFormat();

    SWAPCHAINFORMAT& fmt = m_SwapChainFormat;

    fmt.Type = ConvertType(mode.Format); 
    fmt.Size = mode.Size;
    fmt.RefreshRate = mode.RefreshRate.Numerator;
    fmt.IsWaitVSync = 0 < mode.SyncInterval;
  }

  m_TextureFormat.clear();
  m_RenderTargetTextureFormat.clear();
  m_RenderTargetFormat.clear();
  m_DepthStencilFormat.clear();
  m_IndexBitFormat.clear();
  m_EnableFormat = Graphics::ENABLEFORMAT();

  m_pDevice->SerchEnableFormat( m_EnableFormat );
  const Graphics::ENABLEFORMAT& fmt = m_EnableFormat;

  for( size_t i=0; i<fmt.Texture.size();             ++i ) { m_TextureFormat.insert(fmt.Texture[i]); }
  for( size_t i=0; i<fmt.RenderTargetTexture.size(); ++i ) { m_RenderTargetTextureFormat.insert(fmt.RenderTargetTexture[i]); }
  for( size_t i=0; i<fmt.RenderTargetSurface.size(); ++i ) { m_RenderTargetFormat.insert(fmt.RenderTargetSurface[i]); }
  for( size_t i=0; i<fmt.DepthStencil.size();        ++i ) { m_DepthStencilFormat.insert(fmt.DepthStencil[i]); }
  for( size_t i=0; i<fmt.IndexBit.size();            ++i ) { m_IndexBitFormat.insert(fmt.IndexBit[i]); }

  m_Screen.Initialize();
#pragma  COMPILERMSG( "ピクセルフォーマットはユーザーが選択できるようにすべき" )
  m_DepthStencil.CreateCompatible( m_Screen, PIXELFORMAT_D24IS8 );
}


GraphicsCore::SWAPCHAINFORMAT::TYPE GraphicsCore::ConvertType( PIXELFORMAT fmt ) const
{
  SWAPCHAINFORMAT::TYPE ret = SWAPCHAINFORMAT::BPP16;
  switch( GetPixelBPP(fmt) )
  {
  case 16: { ret = SWAPCHAINFORMAT::BPP16; }break;
  case 32: 
    {
      if( fmt==PIXELFORMAT_A02R10G10B10I || fmt==PIXELFORMAT_A02B10G10R10I )
      {
        ret = SWAPCHAINFORMAT::BPP32EX; 
      }else
      {
        ret = SWAPCHAINFORMAT::BPP32; 
      }
    }break;
  default:
    {
      MAID_ASSERT( true, "ここにはこないはず" );
    }break;
  }

  return ret;
}


}

