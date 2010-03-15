#include"graphics3drender.h"

#include"graphics3drenderlocal.h"

namespace Maid
{

Graphics3DRender::Graphics3DRender( GraphicsCommandControl& com )
  :m_State(STATE_EMPTY)
  ,m_SpriteBltState(BLENDSTATE_ALPHA)
  ,m_SpriteFilter(TEXTUREFILTER_LINEAR)
  ,m_IsDepthWrite(true)
  ,m_CommandCtl(com)
  ,m_Ambient(0,0,0,1)
{

}

Graphics3DRender::~Graphics3DRender()
{

}




void Graphics3DRender::Initialize()
{
  m_State = STATE_LOADING;

  MQOShaderCreate();


  {
    //  256 byteもあれば十分でしょう
    m_ShaderConstantVS.Create( 4*64 );
    m_ShaderConstantPS.Create( 4*64 );
  }

  {
    Graphics::RASTERIZERSTATEPARAM param;

    param.Fill = Graphics::RASTERIZERSTATEPARAM::FILL_SOLID;
    param.Culling = Graphics::RASTERIZERSTATEPARAM::CULLING_BACK;
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

//! アンビエントライトの設定
/*!
    @param	amb     [i ]	設定する色
 */
void Graphics3DRender::SetAmbient( const COLOR_R32G32B32A32F& amb )
{
  m_Ambient = amb;
}

//! 指定したテクスチャを描画する
/*!
    @param	light     [i ]	描画開始座標
 */
void Graphics3DRender::SetLight( const std::vector<LIGHT>& light )
{
  m_Light = light;
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
  m_Camera = cam;
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
  return  MQOShaderIsLoading()

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
  return m_Camera.GetViewMatrix() * m_Camera.GetProjectionMatrix();
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

