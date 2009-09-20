#include"drawcommandexecuted3d11.h"
#include"deviced3d11.h"

//  ここは各シェーダーステージのset,getを書く

#include"bufferd3d11.h"
#include"texture2dd3d11.h"
#include"rendertargetd3d11.h"
#include"depthstencild3d11.h"
#include"materiald3d11.h"
#include"inputlayoutd3d11.h"
#include"rasterizerstated3d11.h"
#include"samplerstated3d11.h"
#include"blendstated3d11.h"
#include"depthstencilstated3d11.h"
#include"vertexshaderd3d11.h"
#include"pixelshaderd3d11.h"

namespace Maid { namespace Graphics {

void DrawCommandExecuteD3D11::SetIndex( const SPBUFFER& pBuffer, size_t Offset )
{
  BufferD3D11* p = static_cast<BufferD3D11*>(pBuffer.get());

  const DXGI_FORMAT fmt = DXGI_FORMAT_R16_UINT;

  m_pDevice->IASetIndexBuffer( p->pBuffer.get(), fmt, Offset );
}

void DrawCommandExecuteD3D11::SetVertex( int Slot, const SPBUFFER& pVertex, int Offset, int Stride )
{
  BufferD3D11* p = static_cast<BufferD3D11*>(pVertex.get());

  const UINT StartSlot = Slot;
  const UINT NumBuffers = 1;
  ID3D11Buffer* pBuff[] = { p->pBuffer.get() };
  const UINT Strides[] = {Stride};
  const UINT Offsets[] = {Offset};

  m_pDevice->IASetVertexBuffers( StartSlot, NumBuffers, pBuff, Strides, Offsets );
}

void DrawCommandExecuteD3D11::SetInputLayout( const SPINPUTLAYOUT& pLayout )
{
  InputLayoutD3D11* p = static_cast<InputLayoutD3D11*>(pLayout.get());

  m_pDevice->IASetInputLayout( p->pDecl.get() );
}

void DrawCommandExecuteD3D11::SetPrimitiveTopology( PRIMITIVE_TOPOLOGY Topology )
{
  D3D11_PRIMITIVE_TOPOLOGY t = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
  switch( Topology )
  {
  case PRIMITIVE_TOPOLOGY_POINTLIST:    { t = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST; }break;
  case PRIMITIVE_TOPOLOGY_LINELIST:     { t = D3D11_PRIMITIVE_TOPOLOGY_LINELIST ; }break;
  case PRIMITIVE_TOPOLOGY_LINESTRIP:    { t = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP; }break;
  case PRIMITIVE_TOPOLOGY_TRIANGLELIST: { t = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; }break;
  case PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:{ t = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; }break;
  }

  m_pDevice->IASetPrimitiveTopology( t );
}






void DrawCommandExecuteD3D11::VSSetMaterial( int stage, const SPMATERIAL& pBuffer )
{
  MaterialD3D11* p = static_cast<MaterialD3D11*>(pBuffer.get());

  const UINT NumBuffers = 1;
  ID3D11ShaderResourceView* pBuff[] = { p->pView.get() };

  m_pDevice->VSSetShaderResources( stage, NumBuffers, pBuff );

}
void DrawCommandExecuteD3D11::VSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler )
{
  SamplerStateD3D11* p = static_cast<SamplerStateD3D11*>(Sampler.get());

  const UINT NumBuffers = 1;
  ID3D11SamplerState* pBuff[] = { p->pState.get() };

  m_pDevice->VSSetSamplers( stage, NumBuffers, pBuff );
}


void DrawCommandExecuteD3D11::VSSetShader( const SPVERTEXSHADER& Shader )
{
  VertexShaderD3D11* p = static_cast<VertexShaderD3D11*>(Shader.get());

  m_pDevice->VSSetShader( p->pShader.get(), NULL, 0 );
}


void DrawCommandExecuteD3D11::VSSetConstant( int slot, const SPBUFFER& pBuffer )
{
  BufferD3D11* p = static_cast<BufferD3D11*>(pBuffer.get());

  const UINT NumBuffers = 1;
  ID3D11Buffer * pBuff[] = { p->pBuffer.get() };

  m_pDevice->VSSetConstantBuffers( slot, NumBuffers, pBuff );
}









void DrawCommandExecuteD3D11::PSSetMaterial( int stage, const SPMATERIAL& pMaterial )
{
  MaterialD3D11* p = static_cast<MaterialD3D11*>(pMaterial.get());

  const UINT NumBuffers = 1;
  ID3D11ShaderResourceView* pBuff[] = { p->pView.get() };

  m_pDevice->PSSetShaderResources( stage, NumBuffers, pBuff );

}

void DrawCommandExecuteD3D11::PSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler )
{
  SamplerStateD3D11* p = static_cast<SamplerStateD3D11*>(Sampler.get());

  const UINT NumBuffers = 1;
  ID3D11SamplerState* pBuff[] = { p->pState.get() };

  m_pDevice->PSSetSamplers( stage, NumBuffers, pBuff );

}

void DrawCommandExecuteD3D11::PSSetShader( const SPPIXELSHADER& Shader )
{
  PixelShaderD3D11* p = static_cast<PixelShaderD3D11*>(Shader.get());

  m_pDevice->PSSetShader( p->pShader.get(), NULL, 0 );

}

void DrawCommandExecuteD3D11::PSSetConstant( int slot, const SPBUFFER& pBuffer )
{
  BufferD3D11* p = static_cast<BufferD3D11*>(pBuffer.get());

  const UINT NumBuffers = 1;
  ID3D11Buffer * pBuff[] = { p->pBuffer.get() };

  m_pDevice->PSSetConstantBuffers( slot, NumBuffers, pBuff );
}





void DrawCommandExecuteD3D11::SetViewPort( const VIEWPORT& port )
{
  D3D11_VIEWPORT vp;
  vp.TopLeftX = (FLOAT)port.Screen.x;
  vp.TopLeftY = (FLOAT)port.Screen.y;
  vp.Width    = (FLOAT)port.Screen.w;
  vp.Height   = (FLOAT)port.Screen.h;
  vp.MinDepth = (FLOAT)port.MinZ;
  vp.MaxDepth = (FLOAT)port.MaxZ;

  m_pDevice->RSSetViewports( 1, &vp );
}

void DrawCommandExecuteD3D11::SetRasterizerState( const SPRASTERIZERSTATE& State )
{
  RasterizerStateD3D11* p = static_cast<RasterizerStateD3D11*>(State.get());

  m_pDevice->RSSetState( p->pState.get() );
}

void DrawCommandExecuteD3D11::SetDepthStencilState( const SPDEPTHSTENCILSTATE& State )
{
  DepthStencilStateD3D11* p = static_cast<DepthStencilStateD3D11*>(State.get());

  m_pDevice->OMSetDepthStencilState( p->pState.get(), 0x00 );
}

void DrawCommandExecuteD3D11::SetBlendState( const SPBLENDSTATE& State )
{
  BlendStateD3D11* p = static_cast<BlendStateD3D11*>(State.get());

  const FLOAT c[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

  m_pDevice->OMSetBlendState( p->pState.get(), c, 0xFFFFFFFF );
}

void DrawCommandExecuteD3D11::SetRenderTarget( int no, const SPRENDERTARGET& pColor, const SPDEPTHSTENCIL& pDepth )
{
  MAID_ASSERT( !(no<8), "値が大きすぎます "<< no );

  m_NowRenderTarget[no] = pColor;
  m_NowDepthStencil[no] = pDepth;

  ID3D11DepthStencilView* d = NULL;
  if( pDepth.get()!=NULL ){ d = static_cast<DepthStencilD3D11*>(pDepth.get())->pView.get(); }

  ID3D11RenderTargetView* c[8] = { NULL };

  for( int i=0; i<NUMELEMENTS(c); ++i )
  {
    if( m_NowRenderTarget[i].get()!=NULL ){ c[i] = static_cast<RenderTargetD3D11*>(m_NowRenderTarget[i].get())->pView.get(); }
  }

  m_pDevice->OMSetRenderTargets( 8, c, d );
}

VIEWPORT DrawCommandExecuteD3D11::GetViewport()
{
  D3D11_VIEWPORT vp[1];
  UINT num = NUMELEMENTS(vp);

  m_pDevice->RSGetViewports( &num, vp );
  
  const D3D11_VIEWPORT& v = vp[0];

  return VIEWPORT( RECT2DI(v.TopLeftX,v.TopLeftY,v.Width,v.Height), v.MinDepth, v.MaxDepth );
}

void DrawCommandExecuteD3D11::GetRenderTarget( int no, SPRENDERTARGET& pColor, SPDEPTHSTENCIL& pDepth )
{
  MAID_ASSERT( (no<8), "値が大きすぎます "<< no );

  pColor = m_NowRenderTarget[no];
  pDepth = m_NowDepthStencil[no];
}


}}

