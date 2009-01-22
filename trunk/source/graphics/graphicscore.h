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

    struct DISPLAYFORMAT
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
    };

    struct SCREENMODE
    {
      DISPLAYFORMAT Format;    //!< 解像度
      bool        IsFullScreen;//!< フルスクリーン？
      bool        IsWaitVSync; //!< VSync同期するか？
    };

    const SCREENMODE& GetScreenMode() const;

    void SetScreenMode( const SCREENMODE& mode );
    void EnumDisplayFormat( std::vector<DISPLAYFORMAT>& dstlist )const;


    PIXELFORMAT FindFormatTexture2D( PIXELFORMAT fmt, bool IsRenderTarget )const;
    PIXELFORMAT FindFormatRenderTarget( PIXELFORMAT fmt )const;
    PIXELFORMAT FindFormatDepthStencil( PIXELFORMAT fmt )const;

    SIZE2DI CalcTextureSize( const SIZE2DI& size, bool IsQuality ) const;

    void SetTextureQuality( int level );
    void SetAspectLock( bool IsLock );

    const RenderTargetBase& GetBackBuffer() const;
    const IDepthStencil&    GetDepthStencil() const;

    int GetTextureQuality() const;

    void Present();

	private:
    const SIZE2DI& GetMaxTextureSize() const;
    typedef std::set<PIXELFORMAT> PIXELFORMATLIST;
    typedef std::set<int>         INDEXBITLIST;

    void UpdateMember();
    bool IsExist( const PIXELFORMATLIST& list, PIXELFORMAT fmt )const;

    DISPLAYFORMAT::TYPE ConvertType( PIXELFORMAT fmt ) const;
    PIXELFORMAT FindFormat( PIXELFORMAT fmt, const PIXELFORMATLIST& List )const;

	private:
    SCREENMODE              m_ScreenMode;
    Graphics::SPDEVICE	    m_pDevice;
    Graphics::SPFONTDEVICE  m_pFontDevice;
    std::vector<Graphics::DISPLAYMODE>  m_EnableDisplayMode;
    Graphics::ENABLEFORMAT  m_EnableFormat;

    RenderTargetScreen  m_Screen;
    DepthStencil  m_DepthStencil;
    int           m_TextureQuality;

    PIXELFORMATLIST m_TextureFormat;
    PIXELFORMATLIST m_RenderTargetTextureFormat;
    PIXELFORMATLIST m_RenderTargetFormat;
    PIXELFORMATLIST m_DepthStencilFormat;
    INDEXBITLIST    m_IndexBitFormat;

  };
}


#endif
