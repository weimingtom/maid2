#ifndef maid2_graphics_graphics3drender_h
#define maid2_graphics_graphics3drender_h

/*!
    @file
    @brief  描画を受け持つクラス 3D ver.
 */


#include"../config/define.h"
#include"../auxiliary/mathematics/size.h"
#include"../auxiliary/globalpointer.h"

#include"graphicscore.h"
#include"vertex.h"
#include"texture2dbase.h"
#include"vertexshader.h"
#include"pixelshader.h"
#include"inputlayout.h"
#include"rasterizerstate.h"
#include"depthstencilstate.h"
#include"blendstate.h"
#include"rendertargetbase.h"
#include"samplerstate.h"
#include"modelmqo.h"
#include"camera.h"
#include"shaderconstant.h"

namespace Maid
{
  class Graphics3DRender
    : public GlobalPointer<GraphicsCore>
  {
  public:
    Graphics3DRender();
    virtual ~Graphics3DRender();

    void Initialize();
    void Initialize( const Graphics::SPDRAWCOMMAND& pCommand );
    bool IsInitializing() const;

    void ClearRenderTarget( const COLOR_R32G32B32A32F& Col );
    void ClearDepth( float Depth );
    void ClearStencil( unt08 Stencil );
    void ClearDepthStencil( float Depth, unt08 Stencil );

    void Begin();
    void End();

    void SetCamera( const Camera& cam );
    void SetRenderTarget( const RenderTargetBase& Target, const IDepthStencil& Depth );

    void SetDepthWrite( bool IsOn );

    void Fill ();
    void _Fill ();

    void Blt  ( const POINT3DF& Pos, const ModelMQO& model );
 
    enum SPRITEBLENDSTATE
    {
      BLENDSTATE_NONE=0,	//  何もしない
      BLENDSTATE_ALPHA,		//  α合成のみ
      BLENDSTATE_ADD,		//  加算
      BLENDSTATE_SUB,		//  減算
      BLENDSTATE_MUL,		//  乗算
      BLENDSTATE_SCREEN,	//  スクリーン
      BLENDSTATE_DARKEN,	//  比較(暗)
      BLENDSTATE_LIGHTTEN,	//  比較(明)
    };

    enum SPRITETEXTUREFILTER
    {
      TEXTUREFILTER_POINT,
      TEXTUREFILTER_LINEAR,
    };

    void SetSpriteFilterState( SPRITETEXTUREFILTER filter );
    void SetSpriteBlendState( SPRITEBLENDSTATE State );

    void SpriteFill  ( const POINT3DF& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center );
  	void SpriteFillS ( const POINT3DF& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, const SIZE2DF& Scale );
    void SpriteFillR ( const POINT3DF& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, float Rotate, const VECTOR3DF& vec );
    void SpriteFillSR( const POINT3DF& Pos, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT2DI& Center, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec );
    void SpriteFill  ( const MATRIX4DF& mat, const COLOR_R32G32B32A32F& Color, const SIZE2DI& Size, const POINT3DF& Center );

    void SpriteBlt  ( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha );
    void SpriteBltS ( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale );
    void SpriteBltR ( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, float Rotate, const VECTOR3DF& vec );
    void SpriteBltSR( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec );
    void SpriteBlt  ( const MATRIX4DF& mat, const Texture2DBase& Texture, const COLOR_R32G32B32A32F& Color, const RECT2DI& rc, const POINT3DF& Center );

  protected:
    bool IsMemberLoading() const;
    Graphics::IDrawCommand& GetCommand();

	  void SetupSpriteState();

  protected:
    enum STATE
    {
      STATE_EMPTY,    //  まだ初期化されてない
      STATE_LOADING,  //  初期設定中
      STATE_WORKING,  //  稼動できる
    };

    STATE m_State;


    SPRITEBLENDSTATE    m_SpriteBltState;
    SPRITETEXTUREFILTER	m_SpriteFilter;

  protected:
    Graphics::SPDRAWCOMMAND m_pDrawCommand;

    Vertex  m_TestVertex;
    InputLayout  m_TestInputLayout;
    Index  m_TestIndex;


    InputLayout  m_MQOLayout;
    VertexShader m_MQOVertexShader;
    PixelShader  m_MQOPixelShader;

    Vertex          m_SpriteVertex;  //  スプライトで使うバッファ
    ShaderConstant  m_SpriteConstant;
    std::vector<BlendState> m_SpriteBlendState;
    RasterizerState m_SpriteRasterizer;

    InputLayout  m_SpriteFillLayout;
    VertexShader m_SpriteFillVertexShader;
    PixelShader  m_SpriteFillPixelShader;

    InputLayout  m_SpriteBltLayout;
    VertexShader m_SpriteBltVertexShader;
    PixelShader  m_SpriteBltPixelShader210;
    PixelShader  m_SpriteBltPixelShader211;

    bool  m_IsDepthWrite;

    float   m_TexelMapValue;

    ShaderConstant  m_ShaderConstant;
    RasterizerState m_Rasterizer;

    SamplerState  m_Sampler;

    DepthStencilState m_DepthOn;
    DepthStencilState m_DepthOff;

    BlendState m_BlendState;

    SamplerState  m_SamplerLinar;
    SamplerState  m_SamplerPoint;


		MATRIX4DF	m_ViewMatrix;
		MATRIX4DF	m_ProjectionMatrix;
  };	
	
}

#endif