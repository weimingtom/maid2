#include"graphicscore.h"


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

namespace Maid {

GraphicsCore*  GlobalPointer<GraphicsCore>::s_pPointer;

  /*!
	 	@class Core core.h
	 	@brief  グラフィック周りを管理するクラス
	 */


GraphicsCore::GraphicsCore()
{
}

GraphicsCore::~GraphicsCore()
{
	Finalize();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 初期化
/*!
	@param	pDriver	[i ]	使うドライバ
 */
void GraphicsCore::Initialize( const Graphics::SPDEVICE& pDevice, const Graphics::SPFONTDEVICE& pFont )
{
  GlobalPointer<GraphicsCore>::Set();

	m_pDevice = pDevice;
	m_pDevice->Initialize();
	m_pDevice->SerchDisplayMode( m_EnableDisplayMode );

  m_AspectLockType = ASPECTLOCK_NONE;
  m_pFontDevice = pFont;
  UpdateMember();


  m_Screen.Initialize();
#pragma  COMPILERMSG( "ピクセルフォーマットはユーザーが選択できるようにすべき" )
  m_DepthStencil.CreateCompatible( m_Screen, PIXELFORMAT_D24IS8 );
}

void GraphicsCore::Finalize()
{
	if( m_pDevice.get()==NULL ) { return ; }

  m_DepthStencil.Destroy();
  m_Screen.Destroy();
  m_AspectScreen.Destroy();
  m_AspectLockType = ASPECTLOCK_NONE;
	m_pDevice->Finalize();
	m_pDevice.reset();
  GlobalPointer<GraphicsCore>::Clear();
}

const Graphics::SPDEVICE& GraphicsCore::GetDevice()const
{
  return m_pDevice;
}

const Graphics::SPFONTDEVICE& GraphicsCore::GetFontDevice()const
{
  return m_pFontDevice;
}

const GraphicsCore::SCREENMODE& GraphicsCore::GetScreenMode() const
{
  return m_ScreenMode;
}

void GraphicsCore::SetScreenMode( const SCREENMODE& mode )
{
  const std::vector<Graphics::DISPLAYMODE>& ModeList = m_EnableDisplayMode;

  //  条件が一致するフォーマットで行う
  for( size_t i=0; i<ModeList.size(); ++i )
  {
    const Graphics::DISPLAYMODE& src = ModeList[i];
    const DISPLAYFORMAT& dst = mode.Format;

    if( dst.Type!=ConvertType(src.Format) ) { continue; }
    if( dst.Size!=src.Size ) { continue; }

    //  リフレッシュレートを０指定した場合は、勝手に行う
    if( dst.RefreshRate!=0 )
    {
      if( dst.RefreshRate!=src.RefreshRate ) { continue; }
    }
    
    Graphics::SCREENMODE new_mode;
    new_mode.DisplayMode  = src;
    new_mode.IsFullScreen = mode.IsFullScreen;
    new_mode.IsWaitVSync  = mode.IsWaitVSync;

	  m_pDevice->SetScreenMode(new_mode);
    break;
  }

  m_Screen.Initialize();
  m_DepthStencil.CreateCompatible( m_Screen, PIXELFORMAT_D24IS8 );
}

void GraphicsCore::EnumDisplayFormat( std::vector<DISPLAYFORMAT>& dstlist ) const
{
  dstlist.clear();

  const std::vector<Graphics::DISPLAYMODE>& ModeList = m_EnableDisplayMode;

  for( size_t i=0; i<ModeList.size(); ++i )
  {
    const Graphics::DISPLAYMODE& src = ModeList[i];
    DISPLAYFORMAT dst;

    dst.Type = ConvertType(src.Format);
    dst.Size = src.Size;
    dst.RefreshRate = src.RefreshRate;

    dstlist.push_back( dst );
  }
}


const SIZE2DI& GraphicsCore::GetMaxTextureSize() const
{
  return m_EnableFormat.MaxTextureSize;
}

SIZE2DI GraphicsCore::CalcTextureSize( const SIZE2DI& size ) const
{
  SIZE2DI ret(0,0);

  const SIZE2DI& SRCSIZE = size;
  const SIZE2DI  MAXSIZE = GetMaxTextureSize();
  ret.w = std::min( ExtendSize(SRCSIZE.w), MAXSIZE.w );
  ret.h = std::min( ExtendSize(SRCSIZE.h), MAXSIZE.h );

  return ret;
}

void GraphicsCore::SetAspectLock( ASPECTLOCK type )
{
  if( m_AspectLockType==type ) { return ; }

  if( type==ASPECTLOCK_NONE )
  {
    m_AspectScreen.Destroy();
  }else
  {
    const SIZE2DI size = m_Screen.GetSize();
    const PIXELFORMAT fmt = m_Screen.GetFormat();

    m_AspectScreen.Create( size, fmt );
  }

  m_AspectLockType = type;
}

const RenderTargetBase& GraphicsCore::GetRenderTarget() const
{
  if( m_AspectLockType==ASPECTLOCK_NONE ) { return m_Screen; }
  
  return m_AspectScreen;
}

const IDepthStencil&    GraphicsCore::GetDepthStencil() const
{
  return m_DepthStencil;
}



void GraphicsCore::Present()
{
  if( m_AspectLockType!=ASPECTLOCK_NONE ) 
  {
    //  アスペクト比を固定するなら ごにょごにょする
    MAID_ASSERT( true, "未実装" );
  }

  m_pDevice->Present();
}

void GraphicsCore::UpdateMember()
{
  MAID_ASSERT( m_pDevice.get()==NULL, "デバイスが作成されていません"  );

  {
    const Graphics::SCREENMODE& mode = m_pDevice->GetScreenMode();
    m_ScreenMode.Format.Type = ConvertType(mode.DisplayMode.Format); 
    m_ScreenMode.Format.Size = mode.DisplayMode.Size;
    m_ScreenMode.Format.RefreshRate = mode.DisplayMode.RefreshRate;
    m_ScreenMode.IsWaitVSync = mode.IsWaitVSync;
    m_ScreenMode.IsFullScreen= mode.IsFullScreen;

  }

  m_TextureFormat.clear();

  m_pDevice->SerchEnableFormat( m_EnableFormat );
  const Graphics::ENABLEFORMAT& fmt = m_EnableFormat;

  for( size_t i=0; i<fmt.Texture.size();             ++i ) { m_TextureFormat.insert(fmt.Texture[i]); }
  for( size_t i=0; i<fmt.RenderTargetTexture.size(); ++i ) { m_RenderTargetTextureFormat.insert(fmt.RenderTargetTexture[i]); }
  for( size_t i=0; i<fmt.RenderTargetSurface.size(); ++i ) { m_RenderTargetFormat.insert(fmt.RenderTargetSurface[i]); }
  for( size_t i=0; i<fmt.DepthStencil.size();        ++i ) { m_DepthStencilFormat.insert(fmt.DepthStencil[i]); }
  for( size_t i=0; i<fmt.IndexBit.size();            ++i ) { m_IndexBitFormat.insert(fmt.IndexBit[i]); }
}

GraphicsCore::DISPLAYFORMAT::TYPE GraphicsCore::ConvertType( PIXELFORMAT fmt ) const
{
  DISPLAYFORMAT::TYPE ret = DISPLAYFORMAT::BPP16;
  switch( GetPixelBPP(fmt) )
  {
  case 16: { ret = DISPLAYFORMAT::BPP16; }break;
  case 32: 
    {
      if( fmt==PIXELFORMAT_A02R10G10B10I || fmt==PIXELFORMAT_A02B10G10R10I )
      {
        ret = DISPLAYFORMAT::BPP32EX; 
      }else
      {
        ret = DISPLAYFORMAT::BPP32; 
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

