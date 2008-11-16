#ifndef maid2_graphic_core_driver_h
#define maid2_graphic_core_driver_h


#include"../../../config/define.h"
#include"../../../config/typedef.h"

#include<boost/smart_ptr.hpp>
#include<vector>

#include"../../../auxiliary/exception.h"
#include"../../../auxiliary/mathematics.h"
#include"../../color.h"
#include"../../pixelformat.h"

#include"surfacebuffer.h"
#include"surfacebuffermemory.h"
#include"vertexbuffer.h"
#include"indexbuffer.h"
#include"texturebuffer.h"
#include"texturebuffermemory.h"
#include"vertexshaderbuffer.h"
#include"pixelshaderbuffer.h"
#include"vertexdeclarationbuffer.h"
#include"vertexdeclarationbuffermemory.h"




namespace Maid
{
  /*!	
      @brief	デバイスがロストしたときに飛んでくる例外
  \n			これを拾ったら VideoCard::Reset() で復帰を試みる
  */
  class DeviceLost : public Exception
  {

  };

  /*!	
      @class	VideoCard videocard.h
      @brief	ビデオドライバ
  \n    			ドライバによってはできること、できないことがありますが
  \n		    	できない場合はエラーを出すのではなく、華麗にスルーするようにしてください
  \n			    ランタイムエラーが起こった場合は Exception を投げること
  */

  class VideoCard
  {
  public:
    struct SCREENMODE
    {
      enum DISPLAY
      {
        DISPLAY_FULL16,
        DISPLAY_FULL32,
        DISPLAY_FULL32EX,  //!<  RGB10bitモード
        DISPLAY_WINDOW,
      };
      SIZE2DI  ScreenSize;   //!<  画面解像度
      DISPLAY  ScreenMode;   //!<  スクリーンモード
      int      RefreshRate;  //!<  リフレッシュレート
      bool     IsWaitVSync;  //!<  VSync同期するか？
    };


    virtual ~VideoCard(){};

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ドライバの初期化
    /*!
        @param	mode	[i ]	初期化して欲しい解像度

        @exception Exception 初期化に失敗した場合
     */
    virtual void Initialize( const SCREENMODE& mode )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! スクリーンモードの変更
    /*!
        デバイスロストからの復帰にも呼んで下さい

        @param	mode	[i ]	あたらしい解像度

        @exception Exception 初期化に失敗した場合
     */
    virtual void Reset( const SCREENMODE& mode )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ロストからの復帰
    /*!
        @exception Exception 復帰に失敗した場合
     */
    virtual void Restore()=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ドライバの開放
    /*!
        @exception Exception 開放に失敗した場合
     */
    virtual void Finalize()=0;

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

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! サーフェスバッファの作成
    /*!
      @param	size	[i ]	テクスチャの大きさ
      @param	fmt		[i ]	ピクセルフォーマット

      @return	作成されたサーフェスバッファ
     */
    virtual SPSURFACEBUFFER CreateSurface( const SIZE2DI& size, PIXELFORMAT fmt )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! テクスチャバッファの作成
    /*!
      @param	buffer	[i ]	そっくりに作るテクスチャバッファ
						      ピクセル内容もコピーされます
						      ミップマップができるかは謎

      @return	作成されたテクスチャバッファ
     */
    virtual SPTEXTUREBUFFER CreateTextureBuffer( const TextureBufferMemory& buffer )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! RenderTargetテクスチャバッファの作成
    /*!
      @param	size	[i ]	テクスチャの大きさ
      @param	fmt	[i ]	ピクセルフォーマット

      @return	作成されたテクスチャバッファ
     */
    virtual SPTEXTUREBUFFER CreateRTTextureBuffer( const SIZE2DI& size, PIXELFORMAT fmt )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! インデックスバッファの作成
    /*!
      @param	data	[i ]	インデックスデータ
      @param	Format	[i ]	１インデックスあたりのビット数

      @return	作成されたインデックスバッファ
     */
    virtual SPINDEXBUFFER CreateIndexBuffer( const std::vector<unt08>& data, int Format )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点バッファの作成
    /*!
      @param	data	[i ]	頂点データ
      @param	Format	[i ]	頂点フォーマット

      @return	作成された頂点バッファ
     */
    virtual SPVERTEXBUFFER CreateVertexBuffer( const std::vector<unt08>& data, unt32 Format )=0;
    
    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! バーテックスシェーダーの作成
    /*!
      @param	Code	[i ]	コード

      @return	作成されたバーテックスシェーダー
     */
    virtual SPVERTEXSHADERBUFFER CreateVertexShader( const std::vector<unt08>& Code )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ゲーム側独自のピクセルシェーダーの作成
    /*!
      @param	Code	[i ]	コード

      @return	作成されたピクセルシェーダー
     */
    virtual SPPIXELSHADERBUFFER CreatePixelShader( const std::vector<unt08>& Code )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ドライバ側で用意されている頂点定義の作成
    /*!
      @param	Name	[i ]	デフォルト名

      @return	作成された頂点定義
     */
    virtual SPVERTEXDECLARATIONBUFFER CreateVertexDeclarationBuffer( const VertexDeclarationBufferMemory& buffer )=0;


    virtual void SetRenderTarget( const SurfaceBuffer* pBuffer )=0;
    virtual void ResetRenderTarget()=0;

    virtual void SetDepthStencil( const SurfaceBuffer* pBuffer )=0;
    virtual void ResetDepthStencil()=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! テクスチャバッファの開放
    /*!
      @param	stage	[i ]	設定するステージ
     */
    virtual void UnsetTextureBuffer( int stage )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! テクスチャバッファのセット
    /*!
      @param	pBuffer	[i ]	設定するバッファ
      @param	stage	[i ]	設定するステージ
     */
    virtual void SetTextureBuffer( const SPTEXTUREBUFFER& pBuffer, int stage )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! インデックスバッファのセット
    /*!
      @param	pBuffer	[i ]	設定するインデックス
     */
    virtual void SetIndexBuffer( const SPINDEXBUFFER& pBuffer )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点バッファのセット
    /*!
      @param	pBuffer	[i ]	設定するバッファ
     */
    virtual void SetVertexBuffer( const SPVERTEXBUFFER& pBuffer, int pos )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! バーテックスシェーダーのセット
    /*!
      @param	pShader	[i ]	設定するシェーダー
     */
    virtual void SetVertexShader( const SPVERTEXSHADERBUFFER& pShader )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ピクセルシェーダーのセット
    /*!
      @param	pShader	[i ]	設定するシェーダー
     */
    virtual void SetPixelShader( const SPPIXELSHADERBUFFER& pShader )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点定義のセット
    /*!
      @param	pDecl	[i ]	設定する定義
     */
    virtual void SetVertexDeclaration( const SPVERTEXDECLARATIONBUFFER& pDecl )=0;

    virtual void SetVertexShaderConstF( int pos, const VECTOR4DF& vec )=0;

    virtual void SetViewPort( const RECT2DI& screen, float MinZ, float MaxZ ) = 0;

    virtual void CopySurface( const SPSURFACEBUFFER& pSrc, SPSURFACEBUFFERMEMORY& pDst )=0;

    virtual String GetVideoInfo()=0;


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
      @param	prim			[i ]	描画の形
      @param	BaseVertexOffset[i ]	VertexBuffer上の MinIndex のオフセット
      @param	MinIndex		[i ]	インデックスの最小の値
      @param	NumVertices		[i ]	呼び出しで使用される頂点の数
      @param	StartIndex		[i ]	何個目のインデックスから描画を開始するか？
      @param	PrimitiveCount	[i ]	描画する数
     */
    virtual void DrawIndexedPrimitive( PRIMITIVE prim, int BaseVertexOffset, unt MinIndex, unt NumVertices, unt StartIndex, unt PrimitiveCount )=0;


    virtual void DrawIndexPrimitiveUP( PRIMITIVE prim, int MinIndex, int NumVertices, const void* pVertex, int VertexSize, int PrimitiveCount, const void* pIndex, int IndexSize  )=0;

  private:

  };

  typedef	boost::shared_ptr<VideoCard>	SPVIDEOCARD;

}


#endif