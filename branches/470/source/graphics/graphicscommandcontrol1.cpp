#include"graphicscommandcontrol.h"


namespace Maid
{

  /*!
      @class  GraphicsCommandControl graphicscommandcontrol.h
      @brief  描画処理でなく初期化とかどこでも使いそうな処理を書いておくとこ
  */

GraphicsCommandControl::GraphicsCommandControl()
  :m_TexelMapValue(0)
  ,m_BeginCount(0)
{

}

GraphicsCommandControl::~GraphicsCommandControl()
{

}


//! このクラスを使えるように初期化
/*!
    設定を勝手に調べて、自動的に行います
*/
void GraphicsCommandControl::Initialize()
{
  GraphicsCore* pCore = GlobalPointer<GraphicsCore>::Get();

  Graphics::SPDRAWCOMMAND pCom = pCore->GetDevice()->GetDrawCommandExecute();
  Initialize( pCom );
}

//! このクラスを使えるように初期化
/*!
    @param  pCommand    [i ]  描画デバイス
 */
void GraphicsCommandControl::Initialize( const Graphics::SPDRAWCOMMAND& pCommand )
{
  m_TexelMapValue = GlobalPointer<GraphicsCore>::Get()->GetDevice()->GetTexelMapValue();
  m_pDrawCommand = pCommand;
}

//! 描画開始の合図
/*!
 */
void GraphicsCommandControl::Begin()
{
  if( m_BeginCount==0 )
  {
    m_pDrawCommand->Begin();
  }
  ++m_BeginCount;
}

//! 描画終了の合図
/*!
 */
void GraphicsCommandControl::End()
{
  --m_BeginCount;
  if( m_BeginCount==0 )
  {
    m_pDrawCommand->End();
  }
}

//! 設定されている RenderTarget を塗りつぶす
/*!
    SLI機能(Scalable Link Interface)を使うには、この呼び出しが必須らしいです。
    原理を考えれば当たり前ですね。

    @param  Col [i ]  塗りつぶす色
 */
void GraphicsCommandControl::ClearRenderTarget( const COLOR_R32G32B32A32F& Col )
{
  m_pDrawCommand->ClearRenderTarget( Col );
}

//! 設定されている DepthStencil のうち、Depthを塗りつぶす
/*!
    普通は 1.0f でしょうね。

    @param  Depth [i ]  設定するdepth値
 */
void GraphicsCommandControl::ClearDepth( float Depth )
{
  m_pDrawCommand->ClearDepthStencil( Graphics::IDrawCommand::CLEARFLAG_DEPTH, Depth, 0 );
}

//! 設定されている DepthStencil のうち、Stencilを塗りつぶす
/*!
    普通は 0xFF でしょうね。

    @param  Stencil [i ]  設定する stencil値
 */
void GraphicsCommandControl::ClearStencil( unt08 Stencil )
{
  m_pDrawCommand->ClearDepthStencil( Graphics::IDrawCommand::CLEARFLAG_STENCIL, 0, Stencil );
}


//! 設定されている DepthStencil を塗りつぶす
/*!
    普通は 1.0f、0xFF でしょうね。

    @param  Depth   [i ]  設定する depth値
    @param  Stencil [i ]  設定する stencil値
 */
void GraphicsCommandControl::ClearDepthStencil( float Depth, unt08 Stencil )
{
  m_pDrawCommand->ClearDepthStencil( Graphics::IDrawCommand::CLEARFLAG_DEPTHSTENCIL, Depth, Stencil );
}


void GraphicsCommandControl::ClearRenderTarget()
{
  ClearRenderTarget( COLOR_R32G32B32A32F(0,0,0,1) );
}

void GraphicsCommandControl::ClearDepth()
{
  ClearDepth( 1.0f );
}

void GraphicsCommandControl::ClearDepthStencil()
{
  ClearDepthStencil( 1.0f, 0xFF );
}

//! RenderTargetの設定
/*!
    引数のとおり、DepthStencilはないものとして設定されます

    @param  Target [i ]  設定する Target
 */
void GraphicsCommandControl::SetRenderTarget( const RenderTargetBase& Target )
{
  m_pDrawCommand->SetRenderTarget( 0, Target.Get(), Graphics::SPDEPTHSTENCIL() );

  const RECT2DI rc = Target.CalcDrawArea();

  const Graphics::VIEWPORT view( rc, 0, 1 );
  m_pDrawCommand->SetViewPort( view );
}

//! RenderTargetの設定
/*!
    @param  Target [i ]  設定する Target
    @param  Depth  [i ]  設定する DepthStencil
 */
void GraphicsCommandControl::SetRenderTarget( const RenderTargetBase& Target, const IDepthStencil& Depth )
{
  m_pDrawCommand->SetRenderTarget( 0, Target.Get(), Depth.Get() );

  const RECT2DI rc = Target.CalcDrawArea();
  const Graphics::VIEWPORT view( rc, 0, 1 );

  m_pDrawCommand->SetViewPort( view );
}


Graphics::IDrawCommand& GraphicsCommandControl::GetCommand()
{
  return *m_pDrawCommand;
}


float GraphicsCommandControl::GetTexelMapValue() const
{
  return m_TexelMapValue;
}





void GraphicsCommandControl::ResourceMap( const GraphicsObject<Graphics::SPBUFFER>& res, int SubLevel, MAPTYPE Type, unt32 Flag, MAPPEDRESOURCE& Mapped )
{
  m_pDrawCommand->ResourceMap( res.Get(), SubLevel, (Graphics::IDrawCommand::MAPTYPE)Type, Flag, (Graphics::MAPPEDRESOURCE&)Mapped );
}

void GraphicsCommandControl::ResourceUnmap( const GraphicsObject<Graphics::SPBUFFER>& res, int SubLevel )
{
  m_pDrawCommand->ResourceUnmap( res.Get(), SubLevel );
}

void GraphicsCommandControl::SetIndex( const IIndex& Buffer, size_t Offset )
{
  m_pDrawCommand->SetIndex( Buffer.Get(), Offset );
}

void GraphicsCommandControl::SetVertex( int Slot, const IVertex& Buffer, int Offset, int Stride )
{
  m_pDrawCommand->SetVertex( Slot, Buffer.Get(), Offset, Stride );
}

void GraphicsCommandControl::SetInputLayout( const IInputLayout& Layout )
{
  m_pDrawCommand->SetInputLayout( Layout.Get() );
}

void GraphicsCommandControl::SetPrimitiveTopology( PRIMITIVE_TOPOLOGY Topology )
{
  m_pDrawCommand->SetPrimitiveTopology( (Graphics::IDrawCommand::PRIMITIVE_TOPOLOGY)Topology );
}

void GraphicsCommandControl::VSSetConstant( int slot, const IConstant& buffer )
{
  m_pDrawCommand->VSSetConstant( slot, buffer.Get() );
}

void GraphicsCommandControl::VSSetMaterial( int stage, const IMaterial& mat )
{
  m_pDrawCommand->VSSetMaterial( stage, mat.Get() );
}

void GraphicsCommandControl::VSSetSamplerState( int stage, const ISamplerState& sampler )
{
  m_pDrawCommand->VSSetSamplerState( stage, sampler.Get() );
}

void GraphicsCommandControl::VSSetShader( const IVertexShader& shader )
{
  m_pDrawCommand->VSSetShader( shader.Get() );
}

void GraphicsCommandControl::PSSetConstant( int slot, const IConstant& buffer )
{
  m_pDrawCommand->PSSetConstant( slot, buffer.Get() );
}

void GraphicsCommandControl::PSSetMaterial( int stage, const IMaterial& mat )
{
  m_pDrawCommand->PSSetMaterial( stage, mat.Get() );
}

void GraphicsCommandControl::PSSetSamplerState( int stage, const ISamplerState& sampler )
{
  m_pDrawCommand->PSSetSamplerState( stage, sampler.Get() );
}

void GraphicsCommandControl::PSSetShader( const IPixelShader& shader )
{
  m_pDrawCommand->PSSetShader( shader.Get() );
}

void GraphicsCommandControl::SetRasterizerState( const IRasterizerState& state )
{
  m_pDrawCommand->SetRasterizerState( state.Get() );
}

void GraphicsCommandControl::SetDepthStencilState( const IDepthStencilState& state )
{
  m_pDrawCommand->SetDepthStencilState( state.Get() );
}

void GraphicsCommandControl::SetBlendState( const IBlendState& state )
{
  m_pDrawCommand->SetBlendState( state.Get() );
}

void GraphicsCommandControl::SetViewPort( const VIEWPORT& src )
{
  Graphics::VIEWPORT dst;

  dst.Screen = src.Screen;
  dst.MinZ = src.MinZ;
  dst.MaxZ = src.MaxZ;

  m_pDrawCommand->SetViewPort( dst );
}

GraphicsCommandControl::VIEWPORT GraphicsCommandControl::GetViewport()const
{
  const Graphics::VIEWPORT dst = m_pDrawCommand->GetViewport();

  VIEWPORT ret;
  ret.Screen = dst.Screen;
  ret.MinZ = dst.MinZ;
  ret.MaxZ = dst.MaxZ;

  return ret;
}

void GraphicsCommandControl::Draw( size_t UseVertexCount, size_t StartVertex )
{
  m_pDrawCommand->Draw( UseVertexCount, StartVertex );
}

void GraphicsCommandControl::DrawIndexed( size_t UseIndexCount, size_t StartIndex, size_t OffsetVertex )
{
  m_pDrawCommand->DrawIndexed( UseIndexCount, StartIndex, OffsetVertex );
}



}