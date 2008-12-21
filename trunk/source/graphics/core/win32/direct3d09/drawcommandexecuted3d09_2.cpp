#include"drawcommandexecuted3d09.h"
#include"deviced3d09.h"

//  ここは各シェーダーステージのset,getを書く

#include"vertexd3d09.h"
#include"indexd3d09.h"
#include"constantd3d09.h"
#include"texture2dd3d09.h"
#include"rendertargetd3d09.h"
#include"depthstencild3d09.h"
#include"materiald3d09.h"
#include"inputlayoutd3d09.h"
#include"rasterizerstated3d09.h"
#include"samplerstated3d09.h"
#include"blendstated3d09.h"
#include"vertexshaderd3d09.h"
#include"pixelshaderd3d09.h"
#include"pixelshaderd3d09fixed.h"

namespace Maid { namespace Graphics {

void DrawCommandExecuteD3D09::SetIndex( const SPBUFFER& pBuffer )
{

}

void DrawCommandExecuteD3D09::SetVertex( int Slot, const SPBUFFER& pVertex, int Offset, int Stride )
{
  VertexD3D09* p = static_cast<VertexD3D09*>(pVertex.get());

  const HRESULT ret = m_Device.GetDevice()->SetStreamSource( Slot, p->pBuffer.get(), Offset, Stride );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetVertex()")); return; }
}

void DrawCommandExecuteD3D09::SetInputLayout( const SPINPUTLAYOUT& pLayout )
{
  InputLayoutD3D09* p = static_cast<InputLayoutD3D09*>(pLayout.get());

  const HRESULT ret = m_Device.GetDevice()->SetVertexDeclaration( p->pDecl.get() );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::SetVertexDeclaration()")); return; }
}

void DrawCommandExecuteD3D09::SetPrimitiveTopology( PRIMITIVE_TOPOLOGY Topology )
{
  D3DPRIMITIVETYPE t = D3DPT_POINTLIST;
  switch( Topology )
  {
  case PRIMITIVE_TOPOLOGY_POINTLIST:    { t = D3DPT_POINTLIST; }break;
  case PRIMITIVE_TOPOLOGY_LINELIST:     { t = D3DPT_LINELIST ; }break;
  case PRIMITIVE_TOPOLOGY_LINESTRIP:    { t = D3DPT_LINESTRIP; }break;
  case PRIMITIVE_TOPOLOGY_TRIANGLELIST: { t = D3DPT_TRIANGLELIST; }break;
  case PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:{ t = D3DPT_TRIANGLESTRIP; }break;
  }


  m_PrimitiveType = t;
}



void DrawCommandExecuteD3D09::VSSetMaterial( int stage, const SPMATERIAL& pBuffer )
{
}

void DrawCommandExecuteD3D09::VSSetShader( const SPVERTEXSHADER& Shader )
{
  VertexShaderD3D09* p = static_cast<VertexShaderD3D09*>(Shader.get());

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

void DrawCommandExecuteD3D09::VSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler )
{
}


void DrawCommandExecuteD3D09::PSSetMaterial( int stage, const SPMATERIAL& Buffer )
{

}

void DrawCommandExecuteD3D09::PSSetShader( const SPPIXELSHADER& Shader )
{

}

void DrawCommandExecuteD3D09::PSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler )
{
}

void DrawCommandExecuteD3D09::SetViewPort( const VIEWPORT& port )
{

}

void DrawCommandExecuteD3D09::SetRasterizerState( const SPRASTERIZERSTATE& State )
{
  RasterizerStateD3D09* p = static_cast<RasterizerStateD3D09*>(State.get());

  const SPD3D09DEVICE& pDev = m_Device.GetDevice();
  for( int i=0; i<(int)p->RenderState.size(); ++i )
  {
    const RENDERSTATE& s = p->RenderState[i];

    pDev->SetRenderState( s.Type, s.Value );
  }
}

void DrawCommandExecuteD3D09::SetBlendState( const SPBLENDSTATE& State )
{
  BlendStateD3D09* p = static_cast<BlendStateD3D09*>(State.get());

  const SPD3D09DEVICE& pDev = m_Device.GetDevice();
  for( int i=0; i<(int)p->RenderState.size(); ++i )
  {
    const RENDERSTATE& s = p->RenderState[i];

    pDev->SetRenderState( s.Type, s.Value );
  }
}

void DrawCommandExecuteD3D09::SetRenderTarget( int no, const SPRENDERTARGET& pColor, const SPDEPTHSTENCIL& pDepth )
{
  MAID_ASSERT( (no<8), "値が大きすぎます "<< no );

  m_NowRenderTarget[no] = pColor;
  m_NowDepthStencil[no] = pDepth;

  IDirect3DSurface9* c = static_cast<Texture2DD3D09*>(pColor->GetResource().get())->pSurface.get();
  IDirect3DSurface9* d = static_cast<Texture2DD3D09*>(pDepth->GetResource().get())->pSurface.get();

#pragma  COMPILERMSG( "Draw()時に切り替えるようにしたほうがいいかもなー" )

  GetDevice()->SetRenderTarget( no, c );
  GetDevice()->SetDepthStencilSurface( d );
}


void DrawCommandExecuteD3D09::GetRenderTarget( int no, SPRENDERTARGET& pColor, SPDEPTHSTENCIL& pDepth )
{
  MAID_ASSERT( (no<8), "値が大きすぎます "<< no );

  pColor = m_NowRenderTarget[no];
  pDepth = m_NowDepthStencil[no];
}


}}

