#include"drawcommandexecuted3d09.h"
#include"object.h"
#include"deviced3d09.h"

//  ここは各シェーダーステージのset,getを書く

namespace Maid { namespace Graphics {

void DrawCommandExecuteD3D09::SetIndex( const hIndex& Buffer )
{

}

void DrawCommandExecuteD3D09::SetVertex( int Slot, const hVertex& Buffer, int Offset, int Stride )
{
  IDirect3DVertexBuffer9* p = (IDirect3DVertexBuffer9*)Buffer.ID;

  const HRESULT ret = m_Device.GetDevice()->SetStreamSource( Slot, p, Offset, Stride );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetVertex()")); return; }
}

void DrawCommandExecuteD3D09::SetInputLayout( const hInputLayout& Layout )
{
  IDirect3DVertexDeclaration9* p = (IDirect3DVertexDeclaration9*)Layout.ID;

  const HRESULT ret = m_Device.GetDevice()->SetVertexDeclaration( p );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetVertexDeclaration()")); return; }
}

void DrawCommandExecuteD3D09::VSSetMaterial( int stage, const hMaterial& Buffer )
{
}

void DrawCommandExecuteD3D09::VSSetShader( const hVertexShader& Shader )
{
  VertexShaderD3D09* p = (VertexShaderD3D09*)Shader.ID;

  const D3DDEVICE_CREATION_PARAMETERS& creat = m_Device.GetD3DDEVICE_CREATION_PARAMETERS();
  const D3DCAPS9& caps = m_Device.GetD3DCAPS9();

  //  VertexShader がハードで実行できるか調べる
  if( IsFlag(creat.BehaviorFlags,D3DCREATE_MIXED_VERTEXPROCESSING) )
  {
    BOOL IsSoft = TRUE;
    if( caps.VertexShaderVersion < p->Version ) { IsSoft = TRUE; }
    else { IsSoft = FALSE; }

    const HRESULT ret = m_Device.GetDevice()->SetSoftwareVertexProcessing( IsSoft );
    if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetSoftwareVertexProcessing()")); return; }
  }

  const HRESULT ret = m_Device.GetDevice()->SetVertexShader( p->pShader.get() );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetVertexShader()")); return; }
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
  RasterizerStateD3D09* p = (RasterizerStateD3D09*)State.ID;

  const SPD3D09DEVICE& pDev = m_Device.GetDevice();
  for( int i=0; i<(int)p->RenderState.size(); ++i )
  {
    const RENDERSTATE& s = p->RenderState[i];

    pDev->SetRenderState( s.Type, s.Value );
  }
}

void DrawCommandExecuteD3D09::SetBlendState( const hBlendState& State )
{

}

void DrawCommandExecuteD3D09::SetRenderTarget( int no, const hRenderTarget& Color, const hDepthStencil& Depth )
{

}


}}

