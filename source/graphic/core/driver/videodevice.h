#ifndef maid2_graphic_core_driver_videodevice_h
#define maid2_graphic_core_driver_videodevice_h


#include"../../../config/define.h"
#include"../../../config/typedef.h"

#include<boost/smart_ptr.hpp>
#include<vector>

#include"../../../auxiliary/exception.h"
#include"../../../auxiliary/mathematics.h"
#include"../../../auxiliary/macro.h"
#include"../../color.h"
#include"../../pixelformat.h"

#include"surfacebuffer.h"
#include"vertexbuffer.h"
#include"indexbuffer.h"
#include"texturebuffer.h"
#include"vertexshaderbuffer.h"
#include"pixelshaderbuffer.h"
#include"vertexdeclarationbuffer.h"

#include"surfacebuffermemory.h"
#include"texturebuffermemory.h"
#include"vertexdeclarationbuffermemory.h"



namespace Maid
{
  /*!	
      @brief	デバイスがロストしたときに飛んでくる例外
  \n			これを拾ったら VideoDevice::Reset() で復帰を試みる
  */
  class DeviceLost : public Exception
  {

  };

  /*!	
      @class	VideoDevice videodevice.h
      @brief	ビデオドライバ
  \n    			ドライバによってはできること、できないことがありますが
  \n		    	できない場合はエラーを出すのではなく、華麗にスルーするようにしてください
  \n			    ランタイムエラーが起こった場合は Exception を投げること
  */

  class VideoDevice
  {
  public:

    struct DISPLAYMODE
    {
      SIZE2DI     Size;         //!<  画面解像度
      PIXELFORMAT Format;       //!<  スクリーンモード
      int         RefreshRate;  //!<  リフレッシュレート
      bool        IsFullScreen; //!<  フルスクリーンにするか？
    };

    struct SCREENMODE
    {
      DISPLAYMODE DisplayMode;  //!<  解像度
      bool        IsWaitVSync;  //!<  VSync同期するか？
    };


    virtual ~VideoDevice(){};

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ドライバの初期化
    /*!
        @exception Exception 初期化に失敗した場合
     */
    virtual void Initialize()=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ドライバの開放
    /*!
        @exception Exception 開放に失敗した場合
     */
    virtual void Finalize()=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! スクリーンモードの変更
    /*!
        デバイスロストからの復帰にも呼んで下さい

        @param	mode	[i ]	あたらしい解像度

        @exception Exception 初期化に失敗した場合
     */
    virtual void Reset( const SCREENMODE& mode )=0;

    enum DEVICESTATE
    {
      DEVICESTATE_OK,       //!<  問題なく動いている
      DEVICESTATE_NOTRESET, //!<  復帰できるように準備できた
      DEVICESTATE_LOST,     //!<  ロスト中
    };
    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ドライバの状態を取得する
    /*!
     */
    virtual DEVICESTATE TestCooperativeLevel()=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! フロントバッファに更新する
    /*!
      @exception DeviceLost	デバイスがロストした場合
      @exception Exception	ランタイムエラーにしたい場合
     */
    virtual void Flip()=0;

    enum SAMPLETYPE
    {
      SAMPLETYPE_01SAMPLES,
      SAMPLETYPE_02SAMPLES,
      SAMPLETYPE_03SAMPLES,
      SAMPLETYPE_04SAMPLES,
      SAMPLETYPE_05SAMPLES,
      SAMPLETYPE_06SAMPLES,
      SAMPLETYPE_07SAMPLES,
      SAMPLETYPE_08SAMPLES,
      SAMPLETYPE_09SAMPLES,
      SAMPLETYPE_10SAMPLES,
      SAMPLETYPE_11SAMPLES,
      SAMPLETYPE_12SAMPLES,
      SAMPLETYPE_13SAMPLES,
      SAMPLETYPE_14SAMPLES,
      SAMPLETYPE_15SAMPLES,
      SAMPLETYPE_16SAMPLES,
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! レンダーターゲットバッファの作成
    /*!
      @param	size    [i ]	サーフェスの大きさ
      @param	fmt     [i ]	ピクセルフォーマット
      @param	type    [i ]	サンプル数
      @param	Quality	[i ]	サンプルクオリティ

      @return	作成されたサーフェスバッファ
     */
    virtual SurfaceBuffer* CreateRenderTargetSurface( const SIZE2DI& size, PIXELFORMAT fmt, SAMPLETYPE type, int Quality )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 深度バッファの作成
    /*!
      @param	size	  [i ]	サーフェスの大きさ
      @param	fmt     [i ]	ピクセルフォーマット
      @param	type    [i ]	サンプル数
      @param	Quality	[i ]	サンプルクオリティ

      @return	作成されたサーフェスバッファ
     */
    virtual SurfaceBuffer* CreateDepthStencileSurface( const SIZE2DI& size, PIXELFORMAT fmt, SAMPLETYPE type, int Quality )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! テクスチャバッファの作成
    /*!
      @param	buffer	[i ]	そっくりに作るテクスチャバッファ
						      ピクセル内容もコピーされます
						      ミップマップができるかは謎

      @return	作成されたテクスチャバッファ
     */
    virtual TextureBuffer* CreateTextureBuffer( const TextureBufferMemory& buffer )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! RenderTargetテクスチャバッファの作成
    /*!
      @param	size	[i ]	テクスチャの大きさ
      @param	fmt	[i ]	ピクセルフォーマット

      @return	作成されたテクスチャバッファ
     */
    virtual TextureBuffer* CreateRenderTargetTextureBuffer( const SIZE2DI& size, PIXELFORMAT fmt )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! インデックスバッファの作成
    /*!
      @param	data	[i ]	インデックスデータ
      @param	Format	[i ]	１インデックスあたりのビット数

      @return	作成されたインデックスバッファ
     */
    virtual IndexBuffer* CreateIndexBuffer( const std::vector<unt08>& data, int Format )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点バッファの作成
    /*!
      @param	data	[i ]	頂点データ
      @param	Format	[i ]	頂点フォーマット

      @return	作成された頂点バッファ
     */
    virtual VertexBuffer* CreateVertexBuffer( const std::vector<unt08>& data, unt32 Format )=0;
    
    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! バーテックスシェーダーの作成
    /*!
      @param	Code	[i ]	コード

      @return	作成されたバーテックスシェーダー
     */
    virtual VertexShaderBuffer* CreateVertexShader( const std::vector<unt08>& Code )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ゲーム側独自のピクセルシェーダーの作成
    /*!
      @param	Code	[i ]	コード

      @return	作成されたピクセルシェーダー
     */
    virtual PixelShaderBuffer* CreatePixelShader( const std::vector<unt08>& Code )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ドライバ側で用意されている頂点定義の作成
    /*!
      @param	Name	[i ]	デフォルト名

      @return	作成された頂点定義
     */
    virtual VertexDeclarationBuffer* CreateVertexDeclarationBuffer( const VertexDeclarationBufferMemory& buffer )=0;


    virtual void SetRenderTarget( const SurfaceBuffer* pBuffer )=0;
    virtual void ResetRenderTarget()=0;

    virtual void SetDepthStencil( const SurfaceBuffer* pBuffer )=0;
    virtual void ResetDepthStencil()=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! テクスチャバッファのセット
    /*!
      @param	stage   [i ]	設定するステージ
      @param	pBuffer	[i ]	設定するバッファ
     */
    virtual void SetTextureBuffer( int stage, const TextureBuffer* pBuffer )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! インデックスバッファのセット
    /*!
      @param	pBuffer	[i ]	設定するインデックス
     */
    virtual void SetIndexBuffer( const IndexBuffer* pBuffer )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点バッファのセット
    /*!
      @param	pBuffer	[i ]	設定するバッファ
     */
    virtual void SetVertexBuffer( int pos, const VertexBuffer* pBuffer )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! バーテックスシェーダーのセット
    /*!
      @param	pShader	[i ]	設定するシェーダー
     */
    virtual void SetVertexShader( const VertexShaderBuffer* pShader )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ピクセルシェーダーのセット
    /*!
      @param	pShader	[i ]	設定するシェーダー
     */
    virtual void SetPixelShader( const PixelShaderBuffer* pShader )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点定義のセット
    /*!
      @param	pDecl	[i ]	設定する定義
     */
    virtual void SetVertexDeclaration( const VertexDeclarationBuffer* pDecl )=0;

    virtual void SetVertexShaderConstF( int pos, const VECTOR4DF& vec )=0;

    virtual void SetViewPort( const RECT2DI& screen, float MinZ, float MaxZ ) = 0;

    virtual void CopySurface( const SurfaceBuffer* pSrc, SurfaceBufferMemory& Dst )=0;


    enum RENDERSTATE
    {
      RENDERSTATE_CULLINGMODE,
      RENDERSTATE_ZWRITEENABLE,
      RENDERSTATE_ZWRITEFUNC,
      RENDERSTATE_ZTESTENABLE,
    };

    enum CULLINGMODE
    {
      CULLINGMODE_NONE=0,
      CULLINGMODE_LEFT,	//!<	反時計回りにカリング
      CULLINGMODE_RIGHT	//!<	時計回りにカリング
    };

    enum STATEBOOL
    {
      STATE_TRUE,
      STATE_FALSE,
    };

    enum CMPTYPE
    {
      CMPTYPE_NEVER		= 1,	//!<	常に通る
      CMPTYPE_LESS		= 2,	//!<	o >  n
      CMPTYPE_EQUAL		= 3,	//!<	o == n
      CMPTYPE_LESSEQUAL	= 4,	//!<	o >= n
      CMPTYPE_GREATER		= 5,	//!<	o <  n
      CMPTYPE_NOTEQUAL	= 6,	//!<	o != n
      CMPTYPE_GREATEREQUAL= 7,	//!<	o <= n
      CMPTYPE_ALWAYS		= 8,	//!<	常にダメ
    };

    virtual void SetRenderState( RENDERSTATE state, unt value )=0;


    enum SAMPLERSTATE
    {
      SAMPLERSTATE_MAGFILTER,
      SAMPLERSTATE_MINFILTER,
      SAMPLERSTATE_TADDRESSU,
      SAMPLERSTATE_TADDRESSV,
    };

    enum FILTERTYPE
    {
      FILTERTYPE_POINT,
      FILTERTYPE_LINEAR,
      FILTERTYPE_ANISOTROPIC,
    };

    enum TEXTUREADDRESS
    {
      TEXTUREADDRESS_WRAP,
      TEXTUREADDRESS_MIRROR,
      TEXTUREADDRESS_CLAMP,
    };


    virtual void SetSamplerState( int Stage, SAMPLERSTATE state, unt value )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! レンダリングの開始
    /*!
     */
    virtual void BeginScene()=0;
    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! レンダリングの終了
    /*!
     */
    virtual void EndScene()=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 現在の RenderTarget を塗りつぶす
    /*!
      @param	Color	[i ]	塗りつぶす色
     */
    virtual void ClearColor( const COLOR_A32B32G32R32F& Color )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 現在の Zバッファ を塗りつぶす
    /*!
      @param	val	[i ]	塗りつぶす値
     */
    virtual void ClearZ( float val )=0;

    enum PRIMITIVE
    {
      PRIMITIVE_POINTLIST=0,
      PRIMITIVE_LINELIST,
      PRIMITIVE_LINESTRIP,
      PRIMITIVE_TRIANGLELIST,
      PRIMITIVE_TRIANGLESTRIP,
      PRIMITIVE_TRIANGLEFAN,
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ポリゴンの描画
    /*!
      @param	prim			[i ]	描画の形
      @param	StartVertex		[i ]	描画を開始する頂点番号
      @param	PrimitiveCount	[i ]	描画する数
     */
    virtual void DrawPrimitive( PRIMITIVE prim, unt StartVertex, unt PrimitiveCount )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ポリゴンの描画
    /*!
      @param	prim			[i ]	描画の形
      @param	pVertex			[i ]	描画を開始する頂点
      @param	Stride			[i ]	１頂点あたりの大きさ
      @param	PrimitiveCount	[i ]	描画する数
     */
    virtual void DrawPrimitiveUP( PRIMITIVE prim, const void* pVertex, unt Stride, unt PrimitiveCount )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ポリゴンの描画
    /*!
        @param	prim            [i ]	描画の形
        @param	BaseVertexOffset[i ]	VertexBuffer*上の MinIndex のオフセット
        @param	MinIndex        [i ]	インデックスの最小の値
        @param	NumVertices     [i ]	呼び出しで使用される頂点の数
        @param	StartIndex      [i ]	何個目のインデックスから描画を開始するか？
        @param	PrimitiveCount  [i ]	描画する数
     */
    virtual void DrawIndexedPrimitive( PRIMITIVE prim, int BaseVertexOffset, unt MinIndex, unt NumVertices, unt StartIndex, unt PrimitiveCount )=0;


    virtual void DrawIndexPrimitiveUP( PRIMITIVE prim, int MinIndex, int NumVertices, const void* pVertex, int VertexSize, int PrimitiveCount, const void* pIndex, int IndexSize  )=0;

  protected:

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! デバイスの準備をする
    /*!
     */
    virtual void SetupDevice() =0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 使用可能なディスプレイモードを調べる
    /*!
        @param	mode [ o]	判明した性能の設定先
     */
    virtual void SerchDisplayMode( std::vector<DISPLAYMODE>& mode ) = 0;


    struct ENABLEFORMAT
    {
      std::vector<PIXELFORMAT>  Texture;              //!<  作成可能なテクスチャフォーマット
      std::vector<PIXELFORMAT>  RenderTargetTexture;  //!<  作成可能なレンダーテクスチャフォーマット
      std::vector<PIXELFORMAT>  RenderTargetSurface;  //!<  作成可能なレンダーサーフェスフォーマット
      std::vector<DEPTHSTENCILFORMAT>  DepthStencil;  //!<  作成可能なDepthStencilフォーマット
      std::vector<int>          IndexBit;             //!<  作成可能な１インデックスあたりのビット数
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 現在のディスプレイモードで作成可能な各種フォーマットを調べる
    /*!
        @param	caps [ o]	判明した性能の設定先
     */
    virtual void SerchEnableFormat( ENABLEFORMAT& caps ) = 0;



  private:

    std::vector<DISPLAYMODE>  m_DisplayMode;
    ENABLEFORMAT              m_EnableFormat;

  };

  typedef	boost::shared_ptr<VideoDevice>	SPVIDEODEVICE;

}


#endif