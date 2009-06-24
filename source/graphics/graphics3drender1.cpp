﻿#include"graphics3drender.h"

#include"graphics3drenderlocal.h"

namespace Maid
{

Graphics3DRender::Graphics3DRender( GraphicsCommandControl& com )
  :m_ViewMatrix(MATRIX4DF().SetIdentity())
  ,m_ProjectionMatrix(MATRIX4DF().SetIdentity())
  ,m_State(STATE_EMPTY)
  ,m_SpriteBltState(BLENDSTATE_ALPHA)
  ,m_SpriteFilter(TEXTUREFILTER_LINEAR)
  ,m_IsDepthWrite(true)
  ,m_CommandCtl(com)
{

}

Graphics3DRender::~Graphics3DRender()
{

}


//  MQO に座標だけが入っているシェーダ
static const char* pCode0100 = 
"\n cbuffer cbPerObject"
"\n {"
"\n   matrix mWVP	  : packoffset( c0 );"
"\n   float4 Color	: packoffset( c4 );"
"\n };"
"\n"
"\n struct VS_INPUT"
"\n {"
"\n   float4 Position    : POSITION;"    //頂点座標
"\n };"
"\n"
"\n struct VS_OUTPUT"
"\n {"
"\n   float4 Position    : SV_Position; "   //頂点座標
"\n   float4 Diffuse     : COLOR0;  "    //デフューズ色
"\n };"
"\n"
"\n VS_OUTPUT main(VS_INPUT Input)"
"\n {"
"\n   VS_OUTPUT Out = (VS_OUTPUT)0;"
"\n   Out.Position = mul( Input.Position, mWVP );"
"\n   Out.Diffuse = Color;"
"\n"
"\n   return Out;"
"\n }"
;

//  MQO に座標と色が入っている場合のシェーダ
static const char* pCode0200 = 
"\n cbuffer cbPerObject"
"\n {"
"\n   matrix mWVP	: packoffset( c0 );"
"\n };"
"\n"
"\n struct VS_INPUT"
"\n {"
"\n   float4 Position    : POSITION;"    //頂点座標
"\n   float4 Diffuse     : COLOR0; "     //デフューズ色
"\n };"
"\n"
"\n struct VS_OUTPUT"
"\n {"
"\n   float4 Position    : SV_Position; "   //頂点座標
"\n   float4 Diffuse     : COLOR0;  "    //デフューズ色
"\n };"
"\n"
"\n VS_OUTPUT main(VS_INPUT Input)"
"\n {"
"\n   VS_OUTPUT Out = (VS_OUTPUT)0;"
"\n   Out.Position = mul( Input.Position, mWVP );"
"\n   Out.Diffuse = Input.Diffuse;"
"\n"
"\n   return Out;"
"\n }"
;




void Graphics3DRender::Initialize()
{
  m_State = STATE_LOADING;


  {
    m_MQOVertexShader.Create( MAIDTEXT(pCode0100) );
  }

  {
    Graphics::INPUT_ELEMENT element[] =
    {
      {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 1, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
    };

    m_MQOLayout.Create( element, NUMELEMENTS(element), MAIDTEXT(pCode0100) );
  }

  {
    m_MQOPixelShader.Create( MAIDTEXT("0200") );
  }

  {
    m_ShaderConstant.Create( sizeof(CONSTANT0100) );
  }

  {
    Graphics::RASTERIZERSTATEPARAM param;

    param.Fill = Graphics::RASTERIZERSTATEPARAM::FILL_SOLID;
    param.Culling = Graphics::RASTERIZERSTATEPARAM::CULLING_LEFT;
    param.MultiSample = false;

    m_Rasterizer.Create( param );
  }
  {
    Graphics::RASTERIZERSTATEPARAM param;

    param.Fill = Graphics::RASTERIZERSTATEPARAM::FILL_SOLID;
    param.Culling = Graphics::RASTERIZERSTATEPARAM::CULLING_NONE;
    param.MultiSample = false;

    m_SpriteRasterizer.Create( param );
  }
  {
    Graphics::DEPTHSTENCILSTATEPARAM param;
    param.DepthTest = true;
    m_DepthOn.Create( param );

    param.DepthTest = false;
    m_DepthOff.Create( param );
  }

  {
    m_BlendState.Create( BlendState::TEMPLATE_NORMAL );
  }

  {
    m_SpriteBlendState.resize( 9 );

    m_SpriteBlendState[0].Create( BlendState::TEMPLATE_NORMAL );
    m_SpriteBlendState[1].Create( BlendState::TEMPLATE_ALPHA );
    m_SpriteBlendState[2].Create( BlendState::TEMPLATE_ADD );
    m_SpriteBlendState[3].Create( BlendState::TEMPLATE_SUB );
    m_SpriteBlendState[4].Create( BlendState::TEMPLATE_MUL );
    m_SpriteBlendState[5].Create( BlendState::TEMPLATE_SCREEN );
    m_SpriteBlendState[6].Create( BlendState::TEMPLATE_DARKEN );
    m_SpriteBlendState[7].Create( BlendState::TEMPLATE_LIGHTTEN );
    m_SpriteBlendState[8].Create( BlendState::TEMPLATE_ALPHA );
  }

  {
    Graphics::SAMPLERSTATEPARAM state;
    state.MinFilter = Graphics::SAMPLERSTATEPARAM::FILTER_LINEAR;
    state.MagFilter = Graphics::SAMPLERSTATEPARAM::FILTER_LINEAR;
    state.MipFilter = Graphics::SAMPLERSTATEPARAM::FILTER_LINEAR;

    m_SamplerLinar.Create( state );

    state.MinFilter = Graphics::SAMPLERSTATEPARAM::FILTER_POINT;
    state.MagFilter = Graphics::SAMPLERSTATEPARAM::FILTER_POINT;
    state.MipFilter = Graphics::SAMPLERSTATEPARAM::FILTER_POINT;

    m_SamplerPoint.Create( state );
  }

  {
    m_ShaderConstant.Create( sizeof(CONSTANTSPRITE) );
  }
  {
    //  1頂点あたり128バイトも使わないでしょう
    m_SpriteVertex.Create( 128*4 );
  }
  {
    const String vs = MAIDTEXT("0110");
    m_SpriteFillVertexShader.Create( vs );
    m_SpriteFillPixelShader.Create( MAIDTEXT("0200") );
    Graphics::INPUT_ELEMENT element[] =
    {
      {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 0, 12, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
    };

    m_SpriteFillLayout.Create( element, NUMELEMENTS(element), vs );
  }


  {
    {
      const String vs = MAIDTEXT("0111");
      m_SpriteBltVertexShader.Create( vs );
      m_SpriteBltPixelShader210.Create( MAIDTEXT("0210") );
      m_SpriteBltPixelShader211.Create( MAIDTEXT("0211") );
      m_SpriteBltPixelShader212.Create( MAIDTEXT("0212") );

      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 0, 12, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"TEXCOORD", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT2, 0, 28, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };

      m_SpriteBltLayout.Create( element, NUMELEMENTS(element), vs );
    }
  }

  {
    const String vs = MAIDTEXT("0112");

    Graphics::INPUT_ELEMENT element[] =
    {
      {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 0, 12, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      {"TEXCOORD", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT2, 0, 28, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      {"TEXCOORD", 1, Graphics::INPUT_ELEMENT::TYPE_FLOAT2, 0, 36, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
    };

    m_SpriteMaskBltVertexShader.Create( vs );
    m_SpriteMaskBltLayout.Create( element, NUMELEMENTS(element), vs );
    m_SpriteMaskBltPixelShader220.Create( MAIDTEXT("0220") );
    m_SpriteMaskBltPixelShader221.Create( MAIDTEXT("0221") );
    m_SpriteMaskBltPixelShader222.Create( MAIDTEXT("0222") );
    m_SpriteMaskBltPixelShader223.Create( MAIDTEXT("0223") );
    m_SpriteMaskBltPixelShader224.Create( MAIDTEXT("0224") );
    m_SpriteMaskBltPixelShader225.Create( MAIDTEXT("0225") );
  }

  {
    {
      const String vs = MAIDTEXT("0112");

      m_SpriteMixBltVertexShader.Create( vs );

      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0,  0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 0, 12, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"TEXCOORD", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT2, 0, 28, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"TEXCOORD", 1, Graphics::INPUT_ELEMENT::TYPE_FLOAT2, 0, 36, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };

      m_SpriteMixBltLayout.Create( element, NUMELEMENTS(element), vs );
    }

    {
      m_SpriteMixBltPixelShader230.Create( MAIDTEXT("0230") );
      m_SpriteMixBltPixelShader231.Create( MAIDTEXT("0231") );
      m_SpriteMixBltPixelShader232.Create( MAIDTEXT("0232") );
    }
  }

}


void Graphics3DRender::SetSpriteBlendState( SPRITEBLENDSTATE State )
{
  m_SpriteBltState = State;
}
void Graphics3DRender::SetSpriteFilterState( SPRITETEXTUREFILTER filter )
{
  m_SpriteFilter = filter;
}

void Graphics3DRender::SetDepthWrite( bool IsOn )
{
  m_IsDepthWrite = IsOn;
}


void Graphics3DRender::SetCamera( const Camera& cam )
{
  m_ViewMatrix = cam.GetViewMatrix();
  m_ProjectionMatrix = cam.GetProjectionMatrix();
}


//! 初期化中か？
/*!
    @return   初期化中なら true
              まだなら false
 */
bool Graphics3DRender::IsInitializing() const
{
  if( m_State!=STATE_LOADING ) { return false; }


  //  メンバが読み込み中なら当然 true
  if( IsMemberLoading() ) { return true; }

  //  読み込み終わったので、残りの処理を行う

  const_cast<Graphics3DRender*>(this)->m_State = STATE_WORKING;
  return false;

}


bool Graphics3DRender::IsMemberLoading() const
{
  return  m_MQOLayout.IsCompiling()
    ||    m_MQOVertexShader.IsCompiling()
    ||    m_MQOPixelShader.IsCompiling()

    ||    m_SpriteFillLayout.IsCompiling()
    ||    m_SpriteFillVertexShader.IsCompiling()
    ||    m_SpriteFillPixelShader.IsCompiling()

    ||    m_SpriteBltLayout.IsCompiling()
    ||    m_SpriteBltVertexShader.IsCompiling()
    ||    m_SpriteBltPixelShader210.IsCompiling()
    ||    m_SpriteBltPixelShader211.IsCompiling()
    ||    m_SpriteBltPixelShader212.IsCompiling()

    ||    m_SpriteMaskBltLayout.IsCompiling()
    ||    m_SpriteMaskBltVertexShader.IsCompiling()
    ||    m_SpriteMaskBltPixelShader220.IsCompiling()
    ||    m_SpriteMaskBltPixelShader221.IsCompiling()
    ||    m_SpriteMaskBltPixelShader222.IsCompiling()
    ||    m_SpriteMaskBltPixelShader223.IsCompiling()
    ||    m_SpriteMaskBltPixelShader224.IsCompiling()
    ||    m_SpriteMaskBltPixelShader225.IsCompiling()

    ||    m_SpriteMixBltLayout.IsCompiling()
    ||    m_SpriteMixBltVertexShader.IsCompiling()
    ||    m_SpriteMixBltPixelShader230.IsCompiling()
    ||    m_SpriteMixBltPixelShader231.IsCompiling()
    ||    m_SpriteMixBltPixelShader232.IsCompiling()

    ;
}

MATRIX4DF	Graphics3DRender::GetCameraMatrix() const
{
  return m_ViewMatrix * m_ProjectionMatrix;
}


Graphics::IDrawCommand& Graphics3DRender::GetCommand()
{
  return m_CommandCtl.GetCommand();
}


//! スプライト表示に使う設定をセットアップする
/*!
    影響がでる範囲は SpriteXXX() のやつだけです
 */
void Graphics3DRender::SetupSpriteState()
{
  Graphics::IDrawCommand& Command = GetCommand();

  {
    ISamplerState* state = NULL;
    
    switch( m_SpriteFilter )
    {
    case TEXTUREFILTER_POINT:  { state = &m_SamplerPoint; }break;
    case TEXTUREFILTER_LINEAR: { state = &m_SamplerLinar; }break;
    }

    Command.PSSetSamplerState( 0, state->Get() );
    Command.PSSetSamplerState( 1, state->Get() );
  }

  {
    IBlendState& state = m_SpriteBlendState[m_SpriteBltState];
    Command.SetBlendState( state.Get() );
  }


  {
    IDepthStencilState* pState = NULL;
    if( m_IsDepthWrite ) { pState = &m_DepthOn;  }
    else                 { pState = &m_DepthOff; }

    Command.SetDepthStencilState( pState->Get() );
  }

  {
    IRasterizerState& raster = m_SpriteRasterizer;
    Command.SetRasterizerState( raster.Get() );
  }

}

}

