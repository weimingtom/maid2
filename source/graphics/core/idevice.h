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

#include"hobject.h"
#include"createparam.h"
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



    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 現在のディスプレイモードで作成可能な各種フォーマットを調べる
    /*!
        @param	caps [ o]	判明した性能の設定先
     */
    virtual void SerchEnableFormat( ENABLEFORMAT& caps ) = 0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! スクリーンモードの変更
    /*!
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
        @param Object [i ]  削除するオブジェクト
    \n                      Object.ID==ID_NONE の場合はなにもしない
     */
    virtual void DeleteObject( const hObject& Object )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 指定したオブジェクトの情報を取得する
    /*!
        @param Object [i ] 取得するオブジェクト
        @param Desc   [ o] 設定する情報先
     */
    virtual void GetObjectDesc( const hObject& Object, ObjectDesc& desc )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点定義の作成
    /*!
        @param	Element	  [i ]	セットアップ情報
        @param	Count	    [i ]	Element の個数
        @param	pShaderBytecodeWithInputSignature	[i ]	Elementを対応させるためのシェーダー
        @param	BytecodeLength	[i ]	pShaderBytecodeWithInputSignature の長さ

        @return	作成された頂点定義
     */
    virtual hInputLayout CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点バッファの作成
    /*!
        @param	param   [i ]	作成したいリソース

        @return	作成されたリソース
     */
    virtual hVertex CreateVertex( const CREATEVERTEXPARAM& param )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! インデックスバッファの作成
    /*!
        @param	param   [i ]	作成したいリソース

        @return	作成されたリソース
     */
    virtual hIndex CreateIndex( const CREATEREINDEXPARAM& param )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ２Ｄテクスチャの作成
    /*!
        @param	param   [i ]	作成したいリソース

        @return	作成されたリソース
     */
    virtual hTexture2D CreateTexture2D( const CREATERETEXTURE2DPARAM& param )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 定数バッファの作成
    /*!
        @param	param   [i ]	作成したいリソース

        @return	作成されたリソース
     */
    virtual hConstant CreateConstant( const CREATERECONSTANTPARAM& param )=0;

    virtual hRenderTarget CreateRenderTarget( const hResource& resource, const CREATERENDERTARGETPARAM& param )=0;
    virtual hDepthStencil CreateDepthStencil( const hResource& resource, const CREATEDEPTHSTENCILPARAM& param )=0;
    virtual hMaterial     CreateMaterial( const hResource& resource, const CREATESHADERMATERIALPARAM& param )=0;


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
    \n          失敗したら false
     */
    virtual bool CompileShaderLanguage( const String& Source, std::vector<unt08>& Binary, String& ErrorMessage )=0;


    virtual hSamplerState     CreateSamplerState( const SAMPLERSTATEPARAM& Option )=0;
    virtual hRasterizerState  CreateRasterizerState( const RASTERIZERSTATEPARAM& Option )=0;
    virtual hBlendState       CreateBlendState( const BLENDSTATEPARAM& Option )=0;



    virtual IDrawCommandExecute* CreateDrawCommandExecute()=0;
    virtual IDrawCommandCapture* CreateDrawCommandCapture()=0;

    virtual hRenderTarget GetDefaultRenderTarget()=0;
    virtual hDepthStencil GetDefaultDepthStencil()=0;

  };

  typedef	boost::shared_ptr<IDevice>	SPDEVICE;

}}


#endif