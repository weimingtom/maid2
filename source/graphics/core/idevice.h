#ifndef maid2_graphics_core_idevice_h
#define maid2_graphics_core_idevice_h


#include"../../config/define.h"
#include"../../config/typedef.h"

#include<boost/smart_ptr.hpp>
#include<vector>

#include"../../auxiliary/exception.h"
#include"../../auxiliary/mathematics.h"
#include"../../auxiliary/macro.h"

#include"../../auxiliary/string.h"
#include"../color.h"
#include"../pixelformat.h"

#include"createparam.h"


#include"ibuffer.h"
#include"itexture2d.h"
#include"iconstant.h"
#include"irendertarget.h"
#include"idepthstencil.h"
#include"imaterial.h"
#include"iinputlayout.h"
#include"irasterizerstate.h"
#include"isamplerstate.h"
#include"iblendstate.h"
#include"idepthstencilstate.h"
#include"ivertexshader.h"
#include"ipixelshader.h"
#include"idrawcommandexecute.h"
#include"idrawcommandcapture.h"


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
    virtual void SerchDisplayMode( std::vector<DISPLAYMODE>& mode ) const= 0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! このアダプタが使用可能なディスプレイモニタを調べる
    /*!
        @param	info [ o] モニタの列挙
     */
    virtual void SerchDisplayInfo( std::vector<DISPLAYINFO>& info ) const=0;



    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 現在のディスプレイモードで作成可能な各種フォーマットを調べる
    /*!
        @param	caps [ o]	判明した性能の設定先
     */
    virtual void SerchEnableFormat( ENABLEFORMAT& caps )  const= 0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ディスプレイのアスペクト比を取得する
    /*!
        @return アスペクト比（といっても縦と横の大きさがあるだけです)
     */
    virtual SIZE2DI GetAspectRatio() const=0;

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
    virtual SCREENMODE GetScreenMode() const=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点定義の作成
    /*!
        @param	Element	  [i ]	セットアップ情報
        @param	Count	    [i ]	Element の個数
        @param	pShaderBytecodeWithInputSignature	[i ]	Elementを対応させるためのシェーダー
        @param	BytecodeLength	[i ]	pShaderBytecodeWithInputSignature の長さ

        @return	作成された頂点定義
     */
    virtual SPINPUTLAYOUT CreateInputLayout( const INPUT_ELEMENT* Element, int Count, const void* pShaderBytecodeWithInputSignature, size_t BytecodeLength )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点バッファの作成
    /*!
        @param	param   [i ]	作成したいリソース

        @return	作成されたリソース
     */
    virtual SPBUFFER CreateBuffer( const CREATEBUFFERPARAM& param, const SUBRESOURCE* data )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ２Ｄテクスチャの作成
    /*!
        @param	param   [i ]	作成したいリソース

        @return	作成されたリソース
     */
    virtual SPTEXTURE2D CreateTexture2D( const CREATERETEXTURE2DPARAM& param, const SUBRESOURCE* data )=0;

    virtual SPRENDERTARGET CreateRenderTarget( const SPRESOURCE& resource, const CREATERENDERTARGETPARAM* param )=0;
    virtual SPDEPTHSTENCIL CreateDepthStencil( const SPRESOURCE& resource, const CREATEDEPTHSTENCILPARAM* param )=0;
    virtual SPMATERIAL     CreateMaterial( const SPRESOURCE& resource, const CREATEMATERIALPARAM* param )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! バーテックスシェーダーの作成
    /*!
      @param	pData   [i ]	コード
      @param	Length  [i ]	データの長さ

      @return	作成されたバーテックスシェーダー
     */
    virtual SPVERTEXSHADER CreateVertexShader( const void* pData, size_t Length )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! ゲーム側独自のピクセルシェーダーの作成
    /*!
      @param	pData   [i ]	コード
      @param	Length  [i ]	データの長さ

      @return	作成されたピクセルシェーダー
     */
    virtual SPPIXELSHADER CreatePixelShader( const void* pData, size_t Length )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 各種シェーダー語をコンパイルする
    /*!
        @param	Source  [i ]	コード
        @param	Binary  [ o]	コンパイルされたデータ
        @param	ErrorMessage  [ o]	コンパイルが失敗したときのエラーメッセージ

        @return	コンパイルに成功したら true
    \n          失敗したら false
     */

    enum CODETYPE
    {
      CODETYPE_VERTEXSHADER,
      CODETYPE_PIXELSHADER,
    };

    virtual bool CompileShaderLanguage( const String& Source, CODETYPE type, std::vector<unt08>& Binary, String& ErrorMessage )=0;


    virtual SPSAMPLERSTATE      CreateSamplerState( const SAMPLERSTATEPARAM& Option )=0;
    virtual SPRASTERIZERSTATE   CreateRasterizerState( const RASTERIZERSTATEPARAM& Option )=0;
    virtual SPDEPTHSTENCILSTATE CreateDepthStencilState( const DEPTHSTENCILSTATEPARAM& Option )=0;
    virtual SPBLENDSTATE        CreateBlendState( const BLENDSTATEPARAM& Option )=0;

    virtual SPDRAWCOMMANDEXECUTE GetDrawCommandExecute()const=0;
    virtual SPDRAWCOMMANDCAPTURE CreateDrawCommandCapture()=0;

    //! 次のPresent()時に表に出てくるRenderTargetを取得する
    /*!
        @return	RenderTarget
     */
    virtual SPRENDERTARGET GetCurrentRenderTarget()const=0;

  };

  typedef	boost::shared_ptr<IDevice>	SPDEVICE;

}}


#endif