#ifndef maid2_graphics_core_idrawcommand_h
#define maid2_graphics_core_idrawcommand_h

#include"../../config/define.h"
#include"../../auxiliary/mathematics.h"

#include"../color.h"
#include"createparam.h"
#include"ibuffer.h"
#include"itexture2d.h"
#include"iconstant.h"
#include"irendertarget.h"
#include"idepthstencil.h"
#include"imaterial.h"
#include"iinputlayout.h"
#include"isamplerstate.h"
#include"irasterizerstate.h"
#include"idepthstencilstate.h"
#include"iblendstate.h"
#include"ivertexshader.h"
#include"ipixelshader.h"

namespace Maid { namespace Graphics {

  /*!	
      @class	IDrawCommand idrawcommand.h
      @brief	描画コンテキスト
  */

  class IDrawCommand
  {
  public:
    virtual ~IDrawCommand(){}

    virtual void Begin()=0;
    virtual void End()=0;

    enum CLEARFLAG
    {
      CLEARFLAG_DEPTH,
      CLEARFLAG_STENCIL,
      CLEARFLAG_DEPTHSTENCIL,
    };

    virtual void ClearDepthStencil( CLEARFLAG flag, float Depth, unt08 Stencil )=0;
    virtual void ClearRenderTarget( const COLOR_R32G32B32A32F& Col )=0;


    enum MAPTYPE
    {
      MAPTYPE_READ,
      MAPTYPE_WRITE,
      MAPTYPE_READ_WRITE,
      MAPTYPE_WRITE_DISCARD,
      MAPTYPE_WRITE_NO_OVERWRITE,
    };

    enum MAPFLAG
    {
      MAPFLAG_DO_NOT_WAIT = 0x100000L,
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! GPUメモリをCPUメモリにマップする
    /*!
      @param	Resource	[i ]	展開するリソース
      @param	SubLevel	[i ]	展開するサブレベル
      @param	Type	    [i ]	アクセスタイプ
      @param	Flag	    [i ]	オプション
      @param	Mapped	  [ o]	CPUメモリ情報
     */
    virtual void ResourceMap( const SPRESOURCE& Resource, int SubLevel, MAPTYPE Type, unt32 Flag, MAPPEDRESOURCE& Mapped )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! マップしてたCPUメモリを閉じる
    /*!
      @param	Resource	[i ]	閉じるリソース
      @param	SubLevel	[i ]	閉じるサブレベル
     */
    virtual void ResourceUnmap( const SPRESOURCE& Resource, int SubLevel )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! インデックスバッファのセット
    /*!
        @param	Buffer	[i ]	設定するインデックス
        @param	Offset	[i ]	最初に使う頂点のオフセット
     */
    virtual void SetIndex( const SPBUFFER& Buffer, size_t Offset )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点バッファのセット
    /*!
      @param	Slot  	[i ]	設定する場所
      @param	Buffer	[i ]	設定するバッファ
      @param	Offset	[i ]	ストリーム先頭から頂点データ先頭までのオフセット (バイト単位)。
      @param	Stride	[i ]	１頂点あたりのサイズ (バイト単位)。
     */
    virtual void SetVertex( int Slot, const SPBUFFER& Buffer, int Offset, int Stride )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点定義のセット
    /*!
      @param	Layout	[i ]	設定する定義
     */
    virtual void SetInputLayout( const SPINPUTLAYOUT& Layout )=0;


    enum PRIMITIVE_TOPOLOGY
    {
      PRIMITIVE_TOPOLOGY_POINTLIST,
      PRIMITIVE_TOPOLOGY_LINELIST,
      PRIMITIVE_TOPOLOGY_LINESTRIP,
      PRIMITIVE_TOPOLOGY_TRIANGLELIST,
      PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
  //  D3D10 には D3DPT_TRIANGLEFAN  がないっぽいから用意しない？(・ω・)
    };

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点をプリミティブに変換するときのルール
    /*!
        @param	Topology	[i ]	設定するルール
     */ 
    virtual void SetPrimitiveTopology( PRIMITIVE_TOPOLOGY Topology )=0;


    virtual void VSSetConstant( int slot, const SPBUFFER& pBuffer )=0;
    virtual void VSSetMaterial( int stage, const SPMATERIAL& Material )=0;
    virtual void VSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler )=0;
    virtual void VSSetShader( const SPVERTEXSHADER& Shader )=0;

    virtual void PSSetConstant( int slot, const SPBUFFER& pBuffer )=0;
    virtual void PSSetMaterial( int stage, const SPMATERIAL& Material )=0;
    virtual void PSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler )=0;
    virtual void PSSetShader( const SPPIXELSHADER& Shader )=0;

    virtual void SetRasterizerState( const SPRASTERIZERSTATE& State ) = 0;
    virtual void SetDepthStencilState( const SPDEPTHSTENCILSTATE& State ) = 0;
    virtual void SetBlendState( const SPBLENDSTATE& State ) = 0;
    virtual void SetViewPort( const VIEWPORT& port ) = 0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 描画先の設定
    /*!
      @param	no	  [i ]	設定先
      @param	Color [i ]	描画されるColor（NULLではずす)
      @param	Depth [i ]	描画されるDepthStencil（NULLではずす)
     */
    virtual void SetRenderTarget( int no, const SPRENDERTARGET& Color, const SPDEPTHSTENCIL& Depth )=0;


    virtual VIEWPORT GetViewport()=0;
    virtual void GetRenderTarget( int no, SPRENDERTARGET& Color, SPDEPTHSTENCIL& Depth )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! プリミティブの描画
    /*!
        @param	UseVertexCount	[i ]	描画に使う頂点数
        @param	StartVertex   	[i ]	すでに設定されている頂点から何個目を最初にするか？
     */
    virtual void Draw( size_t UseVertexCount, size_t StartVertex )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! プリミティブの描画
    /*!
        @param	UseIndexCount [i ]  描画に使うインデックス数
        @param	StartIndex   	[i ]  すでに設定されているインデックスから何個目を最初にするか？
        @param	OffsetVertex  [i ]  使われる頂点の先頭位置・・・？
     */
    virtual void DrawIndexed( size_t UseIndexCount, size_t StartIndex, size_t OffsetVertex )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! リソースのコピー
    /*!
        転送先と元は同一パラメータで作られている必要があります

        @param	pDst [i ]  転送先
        @param	pSrc [i ]  転送元
     */
    virtual void CopyResource( const SPRESOURCE& pDst, const SPRESOURCE& pSrc ) = 0;
  };


  typedef	boost::shared_ptr<IDrawCommand>	SPDRAWCOMMAND;
}}

#endif