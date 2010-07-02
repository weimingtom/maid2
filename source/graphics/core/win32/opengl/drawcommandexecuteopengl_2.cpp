#include"drawcommandexecuteopengl.h"
#include"deviceopengl.h"


//  ここは各シェーダーステージのset,getを書く

#include"bufferopengl.h"
#include"texture2dopengl.h"

/*
#include"rendertargetd3d11.h"
#include"depthstencild3d11.h"
*/
#include"materialopengl.h"
/*
#include"inputlayoutd3d11.h"
#include"rasterizerstated3d11.h"
#include"samplerstated3d11.h"
#include"blendstated3d11.h"
#include"depthstencilstated3d11.h"
#include"vertexshaderd3d11.h"
#include"pixelshaderd3d11.h"
*/


namespace Maid { namespace Graphics {

void DrawCommandExecuteOpenGL::SetIndex( const SPBUFFER& pBuffer, size_t Offset )
{
/*
  BufferD3D11* p = static_cast<BufferD3D11*>(pBuffer.get());

  const DXGI_FORMAT fmt = DXGI_FORMAT_R16_UINT;

  m_pDevice->IASetIndexBuffer( p->pBuffer.get(), fmt, Offset );
*/
}

void DrawCommandExecuteOpenGL::SetVertex( int Slot, const SPBUFFER& pVertex, int Offset, int Stride )
{
  MAID_ASSERT( !(Slot<(int)m_VertexBuffer.size()), "指定したスロットが大きすぎます " << Slot );

  m_VertexBuffer[Slot].pBuffer = boost::shared_static_cast<BufferOpenGL>(pVertex);
  m_VertexBuffer[Slot].Offset  = Offset;
  m_VertexBuffer[Slot].Stride  = Stride;
}

void DrawCommandExecuteOpenGL::SetInputLayout( const SPINPUTLAYOUT& pLayout )
{
  m_pInputLayout = pLayout;
/*
  InputLayoutD3D11* p = static_cast<InputLayoutD3D11*>(pLayout.get());

  m_pDevice->IASetInputLayout( p->pDecl.get() );
*/
}

void DrawCommandExecuteOpenGL::SetPrimitiveTopology( PRIMITIVE_TOPOLOGY Topology )
{
  GLenum t = GL_TRIANGLE_STRIP;

  switch( Topology )
  {
  case PRIMITIVE_TOPOLOGY_POINTLIST:    { t = GL_POINTS; }break;
  case PRIMITIVE_TOPOLOGY_LINELIST:     { t = GL_LINES ; }break;
  case PRIMITIVE_TOPOLOGY_LINESTRIP:    { t = GL_LINE_STRIP; }break;
  case PRIMITIVE_TOPOLOGY_TRIANGLELIST: { t = GL_TRIANGLES; }break;
  case PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:{ t = GL_TRIANGLE_STRIP; }break;
  }

  m_PrimitiveTopology = t;
}






void DrawCommandExecuteOpenGL::VSSetMaterial( int stage, const SPMATERIAL& pBuffer )
{
/*
  MaterialD3D11* p = static_cast<MaterialD3D11*>(pBuffer.get());

  const UINT NumBuffers = 1;
  ID3D11ShaderResourceView* pBuff[] = { p->pView.get() };

  m_pDevice->VSSetShaderResources( stage, NumBuffers, pBuff );
*/
}
void DrawCommandExecuteOpenGL::VSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler )
{
/*
  SamplerStateD3D11* p = static_cast<SamplerStateD3D11*>(Sampler.get());

  const UINT NumBuffers = 1;
  ID3D11SamplerState* pBuff[] = { p->pState.get() };

  m_pDevice->VSSetSamplers( stage, NumBuffers, pBuff );
*/
}


void DrawCommandExecuteOpenGL::VSSetShader( const SPVERTEXSHADER& Shader )
{
  const GLuint prog = m_ShaderProgramID;
  SPVERTEXSHADEROPENGL p = boost::shared_static_cast<VertexShaderOpenGL>(Shader);

  if( m_pVertexShader.get()!=NULL ) { m_Ext.glDetachShader( prog, m_pVertexShader->GetID() ); }
  if( p.get()!=NULL ) { m_Ext.glAttachShader( prog, p->GetID() ); }

  m_pVertexShader = p;
}


void DrawCommandExecuteOpenGL::VSSetConstant( int slot, const SPBUFFER& pBuffer )
{
/*
  BufferD3D11* p = static_cast<BufferD3D11*>(pBuffer.get());

  const UINT NumBuffers = 1;
  ID3D11Buffer * pBuff[] = { p->pBuffer.get() };

  m_pDevice->VSSetConstantBuffers( slot, NumBuffers, pBuff );
*/
}









void DrawCommandExecuteOpenGL::PSSetMaterial( int stage, const SPMATERIAL& pMaterial )
{
  MAID_ASSERT( NUMELEMENTS(m_pMaterialPS)<=stage, "stageは８まで " << stage );

  m_pMaterialPS[stage] = pMaterial;


  MaterialOpenGL* pMat = static_cast<MaterialOpenGL*>(pMaterial.get());

  m_Ext.glActiveTexture( GL_TEXTURE0+stage );
  if( pMat==NULL )
  {
    m_Dll.glDisable( GL_TEXTURE_2D );
  }else
  {
    Texture2DOpenGL* pTex = static_cast<Texture2DOpenGL*>(pMat->GetResource().get());
    m_Dll.glBindTexture( GL_TEXTURE_2D, pTex->GetID() );
    m_Dll.glEnable( GL_TEXTURE_2D );
  }

}

void DrawCommandExecuteOpenGL::PSSetSamplerState( int stage, const SPSAMPLERSTATE& Sampler )
{
/*
  SamplerStateD3D11* p = static_cast<SamplerStateD3D11*>(Sampler.get());

  const UINT NumBuffers = 1;
  ID3D11SamplerState* pBuff[] = { p->pState.get() };

  m_pDevice->PSSetSamplers( stage, NumBuffers, pBuff );
*/
}

void DrawCommandExecuteOpenGL::PSSetShader( const SPPIXELSHADER& Shader )
{
  const GLuint prog = m_ShaderProgramID;
  SPPIXELSHADEROPENGL p = boost::shared_static_cast<PixelShaderOpenGL>(Shader);

  if( m_pPixelShader.get()!=NULL ) { m_Ext.glDetachShader( prog, m_pPixelShader->GetID() ); }
  if( p.get()!=NULL )              { m_Ext.glAttachShader( prog, p->GetID() ); }

  m_pPixelShader = p;
}

void DrawCommandExecuteOpenGL::PSSetConstant( int slot, const SPBUFFER& pBuffer )
{
/*
  BufferD3D11* p = static_cast<BufferD3D11*>(pBuffer.get());

  const UINT NumBuffers = 1;
  ID3D11Buffer * pBuff[] = { p->pBuffer.get() };

  m_pDevice->PSSetConstantBuffers( slot, NumBuffers, pBuff );
*/
}





void DrawCommandExecuteOpenGL::SetViewPort( const VIEWPORT& port )
{
  const int x = port.Screen.x;
  const int y = port.Screen.y;
  const int w = port.Screen.w;
  const int h = port.Screen.h;
  const float minz = port.MinZ;
  const float maxz = port.MaxZ;

  m_Dll.glViewport( x, y, w, h );
  m_Dll.glDepthRange( minz, maxz );
}

void DrawCommandExecuteOpenGL::SetRasterizerState( const SPRASTERIZERSTATE& State )
{
/*
  RasterizerStateD3D11* p = static_cast<RasterizerStateD3D11*>(State.get());

  m_pDevice->RSSetState( p->pState.get() );
*/
}

void DrawCommandExecuteOpenGL::SetDepthStencilState( const SPDEPTHSTENCILSTATE& State )
{
/*
  DepthStencilStateD3D11* p = static_cast<DepthStencilStateD3D11*>(State.get());

  m_pDevice->OMSetDepthStencilState( p->pState.get(), 0x00 );
*/
}

void DrawCommandExecuteOpenGL::SetBlendState( const SPBLENDSTATE& State )
{
  BlendStateOpenGL* p = static_cast<BlendStateOpenGL*>(State.get());

  const BLENDSTATEPARAM::TARGET& target =  p->GetParam().Target[0];

  if( target.Enable )
  {
    m_Dll.glEnable(GL_BLEND);

    {
      const GLenum c_src = TARGETBLENDtoGLenum( target.ColorSrc );
      const GLenum c_dst = TARGETBLENDtoGLenum( target.ColorDst );
      const GLenum a_src = TARGETBLENDtoGLenum( target.AlphaSrc );
      const GLenum a_dst = TARGETBLENDtoGLenum( target.AlphaDst );

      m_Ext.glBlendFuncSeparate( c_src, c_dst, a_src, a_dst );
    }
    {
      // カラーとアルファで別々に設定できない？
      const GLenum op = TARGETOPERATIONtoGLenum(target.ColorOp);
      m_Ext.glBlendEquation(op);
    }

  }else
  {
    m_Dll.glDisable(GL_BLEND);
  }

}

void DrawCommandExecuteOpenGL::SetRenderTarget( int no, const SPRENDERTARGET& pColor, const SPDEPTHSTENCIL& pDepth )
{
/*
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
*/
}

VIEWPORT DrawCommandExecuteOpenGL::GetViewport()
{
  return VIEWPORT( RECT2DI(0,0,800,600), 0,1 );
}

void DrawCommandExecuteOpenGL::GetRenderTarget( int no, SPRENDERTARGET& pColor, SPDEPTHSTENCIL& pDepth )
{
/*
  MAID_ASSERT( (no<8), "値が大きすぎます "<< no );

  pColor = m_NowRenderTarget[no];
  pDepth = m_NowDepthStencil[no];
*/
}


}}

