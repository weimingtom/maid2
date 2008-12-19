#include"drawcommandexecuted3d09.h"

//  ここは各シェーダーステージのset,getを書く

namespace Maid { namespace Graphics {

void DrawCommandExecuteD3D09::SetIndex( const hIndex& Buffer )
{

}

void DrawCommandExecuteD3D09::SetVertex( int Slot, const hVertex& Buffer, int Offset, int Stride )
{
  IDirect3DVertexBuffer9* p = (IDirect3DVertexBuffer9*)Buffer.ID;

  m_pDevice->SetStreamSource( Slot, p, Offset, Stride );
}

void DrawCommandExecuteD3D09::SetInputLayout( const hInputLayout& Layout )
{

}

void DrawCommandExecuteD3D09::VSSetMaterial( int stage, const hMaterial& Buffer )
{
}

void DrawCommandExecuteD3D09::VSSetShader( const hVertexShader& Shader )
{

}

void DrawCommandExecuteD3D09::VSSetSamplerState( int stage, const hSamplerState& Sampler )
{
}


void DrawCommandExecuteD3D09::PSSetMaterial( int stage, const hMaterial& Buffer )
{

}

void DrawCommandExecuteD3D09::PSSetShader( const hPixelShader& Shader )
{

}

void DrawCommandExecuteD3D09::PSSetSamplerState( int stage, const hSamplerState& Sampler )
{
}

void DrawCommandExecuteD3D09::SetViewPort( const RECT2DI& screen, float MinZ, float MaxZ )
{

}

void DrawCommandExecuteD3D09::SetRasterizerState( const hRasterizerState& State )
{

}

void DrawCommandExecuteD3D09::SetBlendState( const hBlendState& State )
{

}

void DrawCommandExecuteD3D09::SetRenderTarget( int no, const hRenderTarget& Color, const hDepthStencil& Depth )
{

}


}}

