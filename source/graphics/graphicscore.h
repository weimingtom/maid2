/*!	
    @file
    @brief	ゲームフレームワークが持つ描画コア
*/

#ifndef maid2_graphics_graphicscore_h
#define maid2_graphics_graphicscore_h

#include"../config/define.h"
#include"../auxiliary/globalpointer.h"
#include"../auxiliary/mathematics/size.h"

#include"rendertarget.h"
#include"rendertargetscreen.h"
#include"depthstencil.h"

#include"pixelformat.h"


#include"core/idevice.h"
#include"core/ifontdevice.h"

#include<set>
#include<vector>

namespace Maid {

  class GraphicsCore : public GlobalPointer<GraphicsCore>
	{
	public:
		GraphicsCore();
		virtual ~GraphicsCore();

		void Initialize( const Graphics::SPDEVICE& pDevice, const Graphics::SPFONTDEVICE& pFont );
		void Finalize();

    const Graphics::SPDEVICE& GetDevice()const;
    const Graphics::SPFONTDEVICE& GetFontDevice()const;

    struct SWAPCHAINFORMAT
    {
      enum TYPE
      {
        BPP16,   //  16bitモード
        BPP32,   //  32bitモード
        BPP32EX, //  RGB10bitモード
      };

      TYPE    Type;
      SIZE2DI Size;
      int     RefreshRate;  //!<  リフレッシュレート
      bool    IsWaitVSync;  //!< VSync同期するか？
    };


    void SetSwapChainFormat( const SWAPCHAINFORMAT& mode );
    void BeginVirtualScreenMode( const SIZE2DI& size );
    void EndVirtualScreenMode();
    void SetFullScreenState( bool IsFullScreen );

    const SWAPCHAINFORMAT& GetSwapChainFormat() const;
    const SIZE2DI& GetVirtualScreenSize() const;
    bool IsFullScreen() const;

    PIXELFORMAT FindFormatTexture2D( PIXELFORMAT fmt, bool IsRenderTarget )const;
    PIXELFORMAT FindFormatRenderTarget( PIXELFORMAT fmt )const;
    PIXELFORMAT FindFormatDepthStencil( PIXELFORMAT fmt )const;

    SIZE2DI CalcTextureSize( const SIZE2DI& size ) const;
    void CalcTextureSize( const SIZE2DI& VirtualSize, SIZE2DI& RealSize, SIZE2DI& TextureSize ) const;

    void SetTextureQuality( int level );
    void SetAspectLock( bool IsLock );
    bool IsAspectLock() const;

    void SetTextureMipMap( bool use );
    bool IsTextureMipMap() const;


    const RenderTargetBase& GetBackBuffer() const;
    const IDepthStencil&    GetDepthStencil() const;

    int GetTextureQuality() const;
    int CalcMipLevels( const SIZE2DI& size ) const;

    void Present();

	private:
    const SIZE2DI& GetMaxTextureSize() const;
    typedef std::set<PIXELFORMAT> PIXELFORMATLIST;
    typedef std::set<int>         INDEXBITLIST;

    void UpdateMember();
    bool IsExist( const PIXELFORMATLIST& list, PIXELFORMAT fmt )const;

    SWAPCHAINFORMAT::TYPE ConvertType( PIXELFORMAT fmt ) const;
    PIXELFORMAT FindFormat( PIXELFORMAT fmt, const PIXELFORMATLIST& List )const;

	private:
    SWAPCHAINFORMAT         m_SwapChainFormat;
    Graphics::SPDEVICE	    m_pDevice;
    Graphics::SPFONTDEVICE  m_pFontDevice;

    Graphics::ENABLEFORMAT    m_EnableFormat;

    String  m_FullScreenDisplayName;  //  フルスクリーン化されているディスプレイ名

    SIZE2DI m_VirtualScreenSize;
    RenderTargetScreen  m_Screen;
    DepthStencil  m_DepthStencil;
    int           m_TextureQuality;
    bool          m_IsAspectLock;
    bool          m_IsTextureMipMap;

    PIXELFORMATLIST m_SwapChainSurfaceFormat;
    PIXELFORMATLIST m_TextureFormat;
    PIXELFORMATLIST m_RenderTargetTextureFormat;
    PIXELFORMATLIST m_RenderTargetFormat;
    PIXELFORMATLIST m_DepthStencilFormat;
    INDEXBITLIST    m_IndexBitFormat;
  };
}


#endif
