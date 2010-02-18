#include"drawcommandexecuted3d10.h"
#include"deviced3d10_0.h"

//  ここは各シェーダーステージのset,getを書く

#include"bufferd3d10.h"
#include"texture2dd3d10.h"
#include"rendertargetd3d10.h"
#include"depthstencild3d10.h"
#include"materiald3d10.h"
#include"inputlayoutd3d10.h"
#include"rasterizerstated3d10.h"
#include"samplerstated3d10.h"
#include"blendstated3d10.h"
#include"depthstencilstated3d10.h"
#include"vertexshaderd3d10.h"
#include"pixelshaderd3d10.h"

namespace Maid { namespace Graphics {

void DrawCommandExecuteD3D10::SetIndex( const SPBUFFER& pBuffer, size_t Offset )
{
  BufferD3D10* p = static_cast<BufferD3D10*>(pBuffer.get());

  const DXGI_FORMAT fmt = DXGI_FORMAT_R16_UINT;

  GetDevice()->IASetIndexBuffer( p->pBuffer.get(), fmt, Offset );
}

void DrawCommandExecuteD3D10::SetVertex( int Slot, const SPBUFFER& pVertex, int Offset, int Stride )
{
  BufferD3D10* p = static_cast<BufferD3D10*>(pVertex.get());

  const UINT StartSlot = Slot;
  const UINT NumBuffers = 1;
  ID3D10Buffer* pBuff[] = { p->pBuffer.get() };
  const UINT Strides[] = {Stride};
  const UINT Offsets[] = {Offset};

  GetDevice()->IASetVertexBuffers( StartSlot, NumBuffers, pBuff, Strides, Offsets );
}

void DrawCommandExecuteD3D10::SetInputLayout( const SPINPUTLAYOUT& pLayout )
{
  InputLayoutD3D10* p = static_cast<InputLayoutD3D10*>(pLayout.get());

  GetDevice()->IASetInputLayout( p->pDecl.get() );
}

void DrawCommandExecuteD3D10::SetPrimitiveTopology( PRIMITIVE_TOPOLOGY Topology )
{
  D3D10_PRIMITIVE_TOPOLOGY t = D3D10_PRIMITIVE_TOPOLOGY_POINTLIST;
  switch( Topology )
  {
  case PRIMITIVE_TOPOLOGY_POINTLIST:    { t = D3D10_PRIMITIVE_TOPOLOGY_POINTLIST; }break;
  case PRIMITIVE_TOPOLOGY_LINELIST:     { t = D3D10_PRIMITIVE_TOPOLOGY_LINELIST ; }break;
  case PRIMITIVE_TOPOLOGY_LINESTRIP:    { t = D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP; }break;
  case PRIMITIVE_TOPOLOGY_TRIANGLELIST: { t = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST; }break;
  case PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:{ t = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; }break;
  }

  GetDevice()->IASetPrimitiveTopology( t );
}






void DrawCommandExecuteD3D10::VSSetMaterial( int stage, const SPMATERIAL& pBuffer )
{
  MaterialD3D10* p = static_cast<MaterialD3D10*>(pBuffer.get());

  const UINT NumBuffers = 1;
  ID3D10ShaderResourceView* pBuff[] = { p->pView.get() };

  GetDevice()->VSSetShaderResources( stage, NumBuffers, pBuff );

}
void DrawCommandExecuteD3D10::VSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler )
{
  SamplerStateD3D10* p = static_cast<SamplerStateD3D10*>(Sampler.get());

  const UINT NumBuffers = 1;
  ID3D10SamplerState* pBuff[] = { p->pState.get() };

  GetDevice()->VSSetSamplers( stage, NumBuffers, pBuff );
}


void DrawCommandExecuteD3D10::VSSetShader( const SPVERTEXSHADER& Shader )
{
  VertexShaderD3D10* p = static_cast<VertexShaderD3D10*>(Shader.get());

  GetDevice()->VSSetShader( p->pShader.get() );
}


void DrawCommandExecuteD3D10::VSSetConstant( int slot, const SPBUFFER& pBuffer )
{
  BufferD3D10* p = static_cast<BufferD3D10*>(pBuffer.get());

  const UINT NumBuffers = 1;
  ID3D10Buffer * pBuff[] = { p->pBuffer.get() };

  GetDevice()->VSSetConstantBuffers( slot, NumBuffers, pBuff );
}









void DrawCommandExecuteD3D10::PSSetMaterial( int stage, const SPMATERIAL& pMaterial )
{
  MaterialD3D10* p = static_cast<MaterialD3D10*>(pMaterial.get());

  const UINT NumBuffers = 1;
  ID3D10ShaderResourceView* pBuff[] = { p->pView.get() };

  GetDevice()->PSSetShaderResources( stage, NumBuffers, pBuff );

}

void DrawCommandExecuteD3D10::PSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler )
{
  SamplerStateD3D10* p = static_cast<SamplerStateD3D10*>(Sampler.get());

  const UINT NumBuffers = 1;
  ID3D10SamplerState* pBuff[] = { p->pState.get() };

  GetDevice()->PSSetSamplers( stage, NumBuffers, pBuff );

}

void DrawCommandExecuteD3D10::PSSetShader( const SPPIXELSHADER& Shader )
{
  PixelShaderD3D10* p = static_cast<PixelShaderD3D10*>(Shader.get());

  GetDevice()->PSSetShader( p->pShader.get() );

}

void DrawCommandExecuteD3D10::PSSetConstant( int slot, const SPBUFFER& pBuffer )
{
  BufferD3D10* p = static_cast<BufferD3D10*>(pBuffer.get());

  const UINT NumBuffers = 1;
  ID3D10Buffer * pBuff[] = { p->pBuffer.get() };

  GetDevice()->PSSetConstantBuffers( slot, NumBuffers, pBuff );
}





void DrawCommandExecuteD3D10::SetViewPort( const VIEWPORT& port )
{
  D3D10_VIEWPORT vp;
  vp.TopLeftX = port.Screen.x;
  vp.TopLeftY = port.Screen.y;
  vp.Width    = port.Screen.w;
  vp.Height   = port.Screen.h;
  vp.MinDepth = port.MinZ;
  vp.MaxDepth = port.MaxZ;

  GetDevice()->RSSetViewports( 1, &vp );
}

void DrawCommandExecuteD3D10::SetRasterizerState( const SPRASTERIZERSTATE& State )
{
  RasterizerStateD3D10* p = static_cast<RasterizerStateD3D10*>(State.get());

  GetDevice()->RSSetState( p->pState.get() );
}

void DrawCommandExecuteD3D10::SetDepthStencilState( const SPDEPTHSTENCILSTATE& State )
{
  DepthStencilStateD3D10* p = static_cast<DepthStencilStateD3D10*>(State.get());

  GetDevice()->OMSetDepthStencilState( p->pState.get(), 0x00 );
}

void DrawCommandExecuteD3D10::SetBlendState( const SPBLENDSTATE& State )
{
  BlendStateD3D10* p = static_cast<BlendStateD3D10*>(State.get());

  const FLOAT c[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

  GetDevice()->OMSetBlendState( p->pState.get(), c, 0xFFFFFFFF );
}

void DrawCommandExecuteD3D10::SetRenderTarget( int no, const SPRENDERTARGET& pColor, const SPDEPTHSTENCIL& pDepth )
{
  MAID_ASSERT( !(no<8), "値が大きすぎます "<< no );

  m_NowRenderTarget[no] = pColor;
  m_NowDepthStencil[no] = pDepth;

  ID3D10DepthStencilView* d = NULL;
  if( pDepth.get()!=NULL ){ d = static_cast<DepthStencilD3D10*>(pDepth.get())->pView.get(); }

  ID3D10RenderTargetView* c[8] = { NULL };

  for( int i=0; i<NUMELEMENTS(c); ++i )
  {
    if( m_NowRenderTarget[i].get()!=NULL ){ c[i] = static_cast<RenderTargetD3D10*>(m_NowRenderTarget[i].get())->pView.get(); }
  }

  GetDevice()->OMSetRenderTargets( 8, c, d );
}

VIEWPORT DrawCommandExecuteD3D10::GetViewport()
{
  D3D10_VIEWPORT vp[1];
  UINT num = NUMELEMENTS(vp);

  GetDevice()->RSGetViewports( &num, vp );
  
  const D3D10_VIEWPORT& v = vp[0];

  return VIEWPORT( RECT2DI(v.TopLeftX,v.TopLeftY,v.Width,v.Height), v.MinDepth, v.MaxDepth );
}

void DrawCommandExecuteD3D10::GetRenderTarget( int no, SPRENDERTARGET& pColor, SPDEPTHSTENCIL& pDepth )
{
  MAID_ASSERT( (no<8), "値が大きすぎます "<< no );

  pColor = m_NowRenderTarget[no];
  pDepth = m_NowDepthStencil[no];
}


}}

