#ifndef maid2_graphics_core_idrawcommand_h
#define maid2_graphics_core_idrawcommand_h

#include"../../config/define.h"
#include"../../auxiliary/mathematics.h"

#include"../color.h"
#include"hObject.h"

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
    virtual void ClearRenderTarget( const COLOR_A32B32G32R32F& Col )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! インデックスバッファのセット
    /*!
      @param	Buffer	[i ]	設定するインデックス
     */
    virtual void SetIndex( const hIndex& Buffer )=0;

    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点バッファのセット
    /*!
      @param	Slot  	[i ]	設定する場所
      @param	Buffer	[i ]	設定するバッファ
     */
    virtual void SetVertex( int Slot, const hVertex& Buffer )=0;


    /*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
    //! 頂点定義のセット
    /*!
      @param	Layout	[i ]	設定する定義
     */
    virtual void SetInputLayout( const hInputLayout& Layout )=0;


    virtual void VSSetMaterial( int stage, const hMaterial& Material )=0;
    virtual void VSSetShader( const hVertexShader& Shader )=0;
    virtual void VSSetSamplerState( const hSamplerState& Sampler )=0;

    virtual void PSSetMaterial( int stage, const hMaterial& Material )=0;
    virtual void PSSetShader( const hPixelShader& Shader )=0;
    virtual void PSSetSamplerState( const hSamplerState& Sampler )=0;

    virtual void SetViewPort( const RECT2DI& screen, float MinZ, float MaxZ ) = 0;
    virtual void SetRasterizerState( const hRasterizerState& State ) = 0;
    virtual void SetBlendState( const hBlendState& State ) = 0;
    virtual void SetRenderTarget( int no, const hRenderTarget& Color, const hDepthStencil& Depth )=0;
  };


  typedef	boost::shared_ptr<IDrawCommand>	SPDRAWCOMMAND;
}}

#endif