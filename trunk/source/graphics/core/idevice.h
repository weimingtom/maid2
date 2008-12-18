#ifndef maid2_graphics_core_idevice_h
#define maid2_graphics_core_idevice_h


#include"../../config/define.h"
#include"../../config/typedef.h"

#include<boost/smart_ptr.hpp>
#include<vector>

#include"../../auxiliary/exception.h"
#include"../../auxiliary/mathematics.h"
#include"../../auxiliary/macro.h"
#include"../color.h"
#include"../pixelformat.h"

#include"hObject.h"
#include"iDrawCommandexecute.h"
#include"iDrawCommandcapture.h"


namespace Maid { namespace Graphics {

  /*!	
      @class	IDevice idevice.h
      @brief	ビデオドライバ
  \n    			ドライバによってはできること、できないことがありますが
  \n		    	できない場合はエラーを出すのではなく、華麗にスルーするようにしてください
  \n			    ランタイムエラーが起こった場合は Exception を投げること
  */

  class IDevice
  {
  public:

    struct DISPLAYMODE
    {
      SIZE2DI     Size;         //!<  画面解像度
      PIXELFORMAT Format;       //!<  スクリーンモード
      int         RefreshRate;  //!<  リフレッシュレート
    };

    struct SCREENMODE
    {
      DISPLAYMODE DisplayMode;  //!<  解像度
      bool        IsFullScreen; //!<  フルスクリーンにするか？
      bool        IsWaitVSync;  //!<  VSync同期するか？
    };

    virtual ~IDevice(){}

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ドライバの初期化
    /*!
        @exception Exception 初期化に失敗した場合
     */
    virtual void Initialize() = 0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ドライバの開放
    /*!
        @exception Exception 開放に失敗した場合
     */
    virtual void Finalize() = 0;

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

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! スクリーンモードの変更
    /*!
        デバイスロストからの復帰にも呼んで下さい

        @param	mode	[i ]	あたらしい解像度

        @exception Exception 初期化に失敗した場合
     */
    virtual void SetScreenMode( const SCREENMODE& mode )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! フロントバッファに更新する
    /*!
     */
    virtual void Present()=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 現在のスクリーンモードの取得
    /*!
        @return 現在のスクリーンモード
     */
    virtual SCREENMODE GetScreenMode()=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 指定したオブジェクトを削除する
    /*!
        @param Object [i ] 削除するオブジェクト
     */
    virtual void DeleteObject( const hObject& Object )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 指定したオブジェクトの情報を取得する
    /*!
        @param Object [i ] 取得するオブジェクト
        @param Desc   [ o] 設定する情報先
     */
    virtual void GetObjectDesc( const hObject& Object, ObjectDesc& desc )=0;


    struct CREATEVERTEXPARAM
    {
      const void* pData;  //  頂点データ
      size_t Length;      //  データの長さ(byte単位)
      unt32 Format;       //  頂点フォーマット
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点バッファの作成
    /*!
        @param	param   [i ]	作成したいリソース

        @return	作成されたリソース
     */
    virtual hVertex CreateVertex( const CREATEVERTEXPARAM& param )=0;


    struct CREATEREINDEXPARAM
    {
      const void* pData;  //  インデックスデータ
      size_t  Length;     //  データの長さ(byte単位)
      unt32   Format;     //  １インデックスあたりのビット数
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! インデックスバッファの作成
    /*!
        @param	param   [i ]	作成したいリソース

        @return	作成されたリソース
     */
    virtual hIndex CreateIndex( const CREATEREINDEXPARAM& param )=0;


    struct CREATERETEXTURE2DPARAM
    {
      const void* pData;  //  ピクセルデータ
      PIXELFORMAT Format; //  ピクセルフォーマット
      int   ArraySize;    //  何枚あるか？
      int   Pitch;        //  １ラインあたりの大きさ(byte単位)
      int   Slice;        //  １平面あたりの大きさ(byte単位)
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ２Ｄテクスチャの作成
    /*!
        @param	param   [i ]	作成したいリソース

        @return	作成されたリソース
     */
    virtual hTexture2D CreateTexture2D( const CREATERETEXTURE2DPARAM& param )=0;


    struct CREATERECONSTANTPARAM
    {
      const void* pData;  //  バッファデータ
      size_t  Length;     //  データの長さ(byte単位)
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 定数バッファの作成
    /*!
        @param	param   [i ]	作成したいリソース

        @return	作成されたリソース
     */
    virtual hConstant CreateConstant( const CREATERECONSTANTPARAM& param )=0;


    struct CREATERENDERTARGETPARAM
    {
      enum DIMENSION
      {
        DIMENSION_BUFFER,
        DIMENSION_TEXTURE1D,
        DIMENSION_TEXTURE1DARRAY,
        DIMENSION_TEXTURE2D,
        DIMENSION_TEXTURE2DARRAY,
        DIMENSION_TEXTURE2DMS,
        DIMENSION_TEXTURE2DMSARRAY,
        DIMENSION_TEXTURE3D,
      };

      PIXELFORMAT Format; //  想定させるフォーマット
      DIMENSION   Dimension;
      unt32       Param[4];
    };

    virtual hRenderTarget CreateRenderTarget( const hResource& resource, const CREATERENDERTARGETPARAM& param )=0;


    struct CREATEDEPTHSTENCILPARAM
    {
      enum DIMENSION
      {
        DIMENSION_TEXTURE1D,
        DIMENSION_TEXTURE1DARRAY,
        DIMENSION_TEXTURE2D,
        DIMENSION_TEXTURE2DARRAY,
        DIMENSION_TEXTURE2DMS,
        DIMENSION_TEXTURE2DMSARRAY,
      };
      enum FLAG
      {
        FLAG_READ_ONLY_DEPTH   = 0x01,
        FLAG_READ_ONLY_STENCIL = 0x02,
      };

      DEPTHSTENCILFORMAT Format; //  想定させるフォーマット
      DIMENSION          Dimension;
      unt32       ReadOnlyUsage;
      unt32       Param[4];
    };

    virtual hDepthStencil CreateDepthStencil( const hResource& resource, const CREATEDEPTHSTENCILPARAM& param )=0;

   struct CREATESHADERMATERIALPARAM
    {
      enum DIMENSION
      {
        DIMENSION_BUFFER,
        DIMENSION_TEXTURE1D,
        DIMENSION_TEXTURE1DARRAY,
        DIMENSION_TEXTURE2D,
        DIMENSION_TEXTURE2DARRAY,
        DIMENSION_TEXTURE2DMS,
        DIMENSION_TEXTURE2DMSARRAY,
        DIMENSION_TEXTURE3D,
        DIMENSION_TEXTURECUBE,
        DIMENSION_TEXTURECUBEARRAY,
        DIMENSION_BUFFEREX,
      };
      enum FLAG
      {
        FLAG_READ_ONLY_DEPTH   = 0x01,
        FLAG_READ_ONLY_STENCIL = 0x02,
      };

      DEPTHSTENCILFORMAT Format; //  想定させるフォーマット
      DIMENSION          Dimension;
      unt32       Param[4];
    };
    virtual hMaterial CreateShaderMaterial( const hResource& resource, const CREATESHADERMATERIALPARAM& param )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! バーテックスシェーダーの作成
    /*!
      @param	pData   [i ]	コード
      @param	Length  [i ]	データの長さ

      @return	作成されたバーテックスシェーダー
     */
    virtual hVertexShader CreateVertexShader( const void* pData, size_t Length )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ゲーム側独自のピクセルシェーダーの作成
    /*!
      @param	pData   [i ]	コード
      @param	Length  [i ]	データの長さ

      @return	作成されたピクセルシェーダー
     */
    virtual hPixelShader CreatePixelShader( const void* pData, size_t Length )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 各種シェーダー語をコンパイルする
    /*!
      @param	Source  [i ]	コード
      @param	Binary  [ o]	コンパイルされたデータ
      @param	ErrorMessage  [ o]	コンパイルが失敗したときのエラーメッセージ

      @return	コンパイルに成功したら true
    \n        失敗したら false
     */
    virtual bool CompileShaderLanguage( const String& Source, std::vector<unt08>& Binary, String& ErrorMessage )=0;

    enum COMPARISON
    {
      COMPARISON_NEVER        = 1,	//!<	常に通る
      COMPARISON_LESS         = 2,	//!<	o >  n
      COMPARISON_EQUAL		    = 3,	//!<	o == n
      COMPARISON_LESSEQUAL	  = 4,	//!<	o >= n
      COMPARISON_GREATER		  = 5,	//!<	o <  n
      COMPARISON_NOTEQUAL     = 6,	//!<	o != n
      COMPARISON_GREATEREQUAL = 7,	//!<	o <= n
      COMPARISON_ALWAYS       = 8,	//!<	常にダメ
    };

    struct SAMPLERSTATEPARAM
    {
      enum FILTER
      {
        FILTER_POINT,
        FILTER_LINEAR,
        FILTER_ANISOTROPIC,
      };

      enum ADDRESS
      {
        ADDRESS_WRAP,
        ADDRESS_MIRROR,
        ADDRESS_CLAMP,
        ADDRESS_BORDER,
        ADDRESS_MIRROR_ONCE,
      };



      FILTER  MinFilter;
      FILTER  MagFilter;

      int   MaxAnisotropy;

      ADDRESS AddressU;
      ADDRESS AddressV;
      ADDRESS AddressW;

      float MipLODBias;
      COMPARISON   ComparisonFunc;
      float BorderColor[4];
      float MinLOD;
      float MaxLOD;
    };
    virtual hSamplerState CreateSamplerState( const SAMPLERSTATEPARAM& Option )=0;

    struct RASTERIZERSTATEPARAM
    {
      enum CULLING
      {
        CULLING_NONE=0,
        CULLING_LEFT,	//!<	反時計回りにカリング
        CULLING_RIGHT	//!<	時計回りにカリング
      };

      CULLING Culling;
      bool MultiSample;
    };
    virtual hRasterizerState CreateRasterizerState( const RASTERIZERSTATEPARAM& Option )=0;

    struct BLENDSTATEPARAM
    {
      enum BLEND
      {
        BLEND_ZERO,
        BLEND_ONE,
        BLEND_SRC_COLOR,
        BLEND_INV_SRC_COLOR,
        BLEND_SRC_ALPHA,
        BLEND_INV_SRC_ALPHA,
        BLEND_DEST_ALPHA,
        BLEND_INV_DEST_ALPHA,
        BLEND_DEST_COLOR,
        BLEND_INV_DEST_COLOR,
        BLEND_SRC_ALPHA_SAT,
        BLEND_BLEND_FACTOR,
        BLEND_INV_BLEND_FACTOR,
        BLEND_SRC1_COLOR,
        BLEND_INV_SRC1_COLOR,
        BLEND_SRC1_ALPHA,
        BLEND_INV_SRC1_ALPHA,
      };

      enum OPERATION
      {
        OPERATION_ADD,
        OPERATION_SUBTRACT,
        OPERATION_REV_SUBTRACT,
        OPERATION_MIN,
        OPERATION_MAX,
      };

      enum MASK
      {
        MASK_RED  =0x01,
        MASK_GREEN=0x02,
        MASK_BLUE =0x04,
        MASK_ALPHA=0x08,
        MASK_ALL = MASK_RED|MASK_GREEN|MASK_BLUE|MASK_ALPHA
      };

      bool  AlphaToCoverageEnable;
      bool  BlendEnable[8];

      BLEND SrcBlend;
      BLEND DestBlend;
      OPERATION BlendOp;
      BLEND SrcBlendAlpha;
      BLEND DestBlendAlpha;
      OPERATION BlendOpAlpha;
      unt08 RenderTargetWriteMask[8];
    };

    virtual hBlendState CreateBlendState( const BLENDSTATEPARAM& Option )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ドライバ側で用意されている頂点定義の作成
    /*!
      @param	Name	[i ]	デフォルト名

      @return	作成された頂点定義
     */
    struct INPUT_ELEMENT
    {
      enum TYPE
      {
        TYPE_FLOAT1,
        TYPE_FLOAT2,
        TYPE_FLOAT3,
        TYPE_FLOAT4,
        TYPE_COLOR,
      };
      enum METHOD {
          METHOD_DEFAULT = 0,
          METHOD_PARTIALU = 1,
          METHOD_PARTIALV = 2,
          METHOD_CROSSUV = 3,
          METHOD_UV = 4,
          METHOD_LOOKUP,
          METHOD_LOOKUPPRESAMPLED
      };

      char*  SemanticName;
      int    SemanticIndex;
      TYPE   Type;
      int    SlotNo;
      int    Offset;
      METHOD Method;
    };
    virtual hInputLayout CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength )=0;


    virtual IDrawCommandExecute* CreateDrawCommandExecute()=0;
    virtual IDrawCommandCapture* CreateDrawCommandCapture()=0;

    virtual hRenderTarget GetDefaultRenderTarget()=0;
    virtual hDepthStencil GetDefaultDepthStencil()=0;

  };

  typedef	boost::shared_ptr<IDevice>	SPDEVICE;

}}


#endif