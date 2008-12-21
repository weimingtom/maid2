#include"graphicsrender.h"
#include"customvertextemplate.h"




namespace Maid
{
void GraphicsRender::Initialize( const Graphics::SPDRAWCOMMAND& pDrawCommand )
{
  {
    m_FillVertex.Create( sizeof(CUSTOMVERTEX_COLOR)*4 );

    {
      const char* vs =
        "vs.1.1\n"
        "dcl_position v0\n"
        "dcl_color v1\n"
        "mov oPos, v0\n"
        "mov oD0, v1\n"
        ;

      m_FillVertexShader.Create( String::ConvertSJIStoMAID(vs) );
    }

    {
      const char* ps =
        "1"
        ;

      m_FillPixelShader.Create( String::ConvertSJIStoMAID(ps) );
    }

    {
      Graphics::INPUT_ELEMENT element[] =
      {
        {"POSITION", 0, Graphics::INPUT_ELEMENT::TYPE_FLOAT3, 0, 0, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
        {"COLOR",    0, Graphics::INPUT_ELEMENT::TYPE_FLOAT4, 0, 12, Graphics::INPUT_ELEMENT::METHOD_DEFAULT},
      };

      m_FillLayout.Create( element, NUMELEMENTS(element), NULL, 0 );
    }
  }

  {
    Graphics::RASTERIZERSTATEPARAM param;

    param.Fill = Graphics::RASTERIZERSTATEPARAM::FILL_SOLID;
    param.Culling = Graphics::RASTERIZERSTATEPARAM::CULLING_NONE;
    param.MultiSample = false;

    m_Rasterizer.Create( param );
  }
  {
    Graphics::DEPTHSTENCILSTATEPARAM param;
    param.DepthTest = true;
    m_DepthOn.Create( param );
    param.DepthTest = false;
    m_DepthOff.Create( param );

  }

  {
     Graphics::BLENDSTATEPARAM param;

     param.Target[0].Enable = true;
     param.Target[0].ColorSrc = Graphics::BLENDSTATEPARAM::TARGET::BLEND_SRC_ALPHA;
     param.Target[0].ColorDst = Graphics::BLENDSTATEPARAM::TARGET::BLEND_INV_SRC_ALPHA;
     m_BlendAlpha.Create( param );
  }

  {
    const Graphics::VIEWPORT view = pDrawCommand->GetViewport();
    m_ScreenSize = SIZE2DI( view.Screen.w, view.Screen.h );
  }

  m_pDrawCommand = pDrawCommand;
}

void GraphicsRender::Begin()
{
  m_pDrawCommand->Begin();
}

void GraphicsRender::End()
{
  m_pDrawCommand->End();
}

void GraphicsRender::ClearRenderTarget( const COLOR_R32G32B32A32F& Col )
{
  m_pDrawCommand->ClearRenderTarget( Col );
}


bool GraphicsRender::IsLoading() const
{
  return m_FillVertexShader.IsCompiling()
    || m_FillPixelShader.IsCompiling()
    ;
}

void GraphicsRender::SetDefaultSetting()
{
  MAID_ASSERT( IsLoading(), "初期化中は呼ばないこと" );

  SetDepthTest( false );
  SetBlendState( BLENDSTATE_ALPHA );
  IRasterizerState& raster = m_Rasterizer;
  m_pDrawCommand->SetRasterizerState( raster.Get() );
}



void GraphicsRender::SetDepthTest( bool Enable )
{
  MAID_ASSERT( IsLoading(), "初期化中は呼ばないこと" );
  IDepthStencilState& state = Enable? m_DepthOn : m_DepthOff;
  m_pDrawCommand->SetDepthStencilState( state.Get() );
}

void GraphicsRender::SetBlendState( BLENDSTATE State )
{
  MAID_ASSERT( IsLoading(), "初期化中は呼ばないこと" );
  IBlendState& state = m_BlendAlpha;
  m_pDrawCommand->SetBlendState( state.Get() );

}

}