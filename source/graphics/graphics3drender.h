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

#include"graphicscommandcontrol.h"
#include"light.h"

namespace Maid
{
  class Graphics3DRender
    : public GlobalPointer<GraphicsCore>
  {
  public:
    Graphics3DRender( GraphicsCommandControl& com );
    virtual ~Graphics3DRender();

    void Initialize();
    bool IsInitializing() const;

    void SetCamera( const Camera& cam );

    void SetDepthWrite( bool IsOn );

    void Blt( const POINT3DF& Pos, const ModelMQO& model, float alpha );
    void BltS ( const POINT3DF& Pos, const ModelMQO& model, float alpha, const SIZE3DF& Scale );
    void BltR ( const POINT3DF& Pos, const ModelMQO& model, float alpha, float Rotate, const VECTOR3DF& vec );
    void BltSR( const POINT3DF& Pos, const ModelMQO& model, float alpha, const SIZE3DF& Scale, float Rotate, const VECTOR3DF& vec );
    void Blt( const MATRIX4DF& mat, const ModelMQO& model, float alpha );
 
    void SetAmbient( const COLOR_R32G32B32A32F& amb );
    void SetLight( const std::vector<LIGHT>& light );

    MATRIX4DF	GetCameraMatrix() const;


    enum SPRITEBLENDSTATE
    {
      BLENDSTATE_NONE=0,	//  何もしない
      BLENDSTATE_ALPHA,		//  α合成のみ
      BLENDSTATE_ADD,		//  加算
      BLENDSTATE_SUB,		//  減算
      BLENDSTATE_MUL,		//  乗算
      BLENDSTATE_SCREEN,	//  スクリーン
      BLENDSTATE_DARKEN,   //  比較（暗）
      BLENDSTATE_LIGHTTEN, //  比較（明）
      BLENDSTATE_REVERSE,   //  反転
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

    void SpriteMaskBlt  ( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const Texture2DBase& Mask );
    void SpriteMaskBltS ( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, const Texture2DBase& Mask );
    void SpriteMaskBltR ( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, float Rotate, const VECTOR3DF& vec, const Texture2DBase& Mask );
    void SpriteMaskBltSR( const POINT3DF& Pos, const Texture2DBase& Texture, const RECT2DI& rc, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec, const Texture2DBase& Mask );
    void SpriteMaskBlt  ( const MATRIX4DF& mat, const Texture2DBase& Texture, const COLOR_R32G32B32A32F& Color, const RECT2DI& rc, const POINT3DF& Center, const Texture2DBase& Mask );

    void SpriteMixBlt  ( const POINT3DF& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, float alpha, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow );
    void SpriteMixBltS ( const POINT3DF& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow );
    void SpriteMixBltR ( const POINT3DF& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, float alpha, float Rotate, const VECTOR3DF& vec, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow );
    void SpriteMixBltSR( const POINT3DF& Pos, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT2DI& Center, float alpha, const SIZE2DF& Scale, float Rotate, const VECTOR3DF& vec, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow );
    void SpriteMixBlt  ( const MATRIX4DF& mat, const Texture2DBase& TexBegin, const RECT2DI& BeginRect, const POINT3DF& Center, float alpha, const Texture2DBase& TexEnd, const RECT2DI& EndRect, float pow );


  protected:
    bool IsMemberLoading() const;
    Graphics::IDrawCommand& GetCommand();

	  void SetupSpriteState();

    void MQOShaderCreate();
    bool MQOShaderIsLoading()const;
    void MQOShaderSetup( const MATRIX4DF& world, const MATRIX4DF& wvp, const MQOMATERIAL& mat, float alpha );




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
    GraphicsCommandControl&  m_CommandCtl;

    Vertex  m_TestVertex;
    InputLayout  m_TestInputLayout;
    Index  m_TestIndex;

    std::map<int,InputLayout>   m_MQOLayout;
    std::map<int,VertexShader>  m_MQOVertexShader;
    std::map<int,PixelShader>   m_MQOPixelShader;
    BlendState      m_MQOBlendState;
    RasterizerState m_MQORasterizer;


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
    PixelShader  m_SpriteBltPixelShader212;

    InputLayout  m_SpriteMaskBltLayout;
    VertexShader m_SpriteMaskBltVertexShader;
    PixelShader  m_SpriteMaskBltPixelShader220;
    PixelShader  m_SpriteMaskBltPixelShader221;
    PixelShader  m_SpriteMaskBltPixelShader222;
    PixelShader  m_SpriteMaskBltPixelShader223;
    PixelShader  m_SpriteMaskBltPixelShader224;
    PixelShader  m_SpriteMaskBltPixelShader225;

    InputLayout  m_SpriteMixBltLayout;
    VertexShader m_SpriteMixBltVertexShader;
    PixelShader  m_SpriteMixBltPixelShader230;
    PixelShader  m_SpriteMixBltPixelShader231;
    PixelShader  m_SpriteMixBltPixelShader232;


    bool  m_IsDepthWrite;

    ShaderConstant  m_ShaderConstant;
    RasterizerState m_Rasterizer;

    SamplerState  m_Sampler;

    DepthStencilState m_DepthOn;
    DepthStencilState m_DepthOff;

    BlendState m_BlendState;

    SamplerState  m_SamplerLinar;
    SamplerState  m_SamplerPoint;


    Camera    m_Camera;

    std::vector<LIGHT>  m_Light;
    COLOR_R32G32B32A32F m_Ambient;
  };	
	
}

#endif