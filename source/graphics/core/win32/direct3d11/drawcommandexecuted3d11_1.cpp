#include"drawcommandexecuted3d11.h"


#include"bufferd3d11.h"
#include"texture2dd3d11.h"
#include"rendertargetd3d11.h"
#include"depthstencild3d11.h"
#include"materiald3d11.h"
#include"inputlayoutd3d11.h"
#include"rasterizerstated3d11.h"
#include"samplerstated3d11.h"
#include"blendstated3d11.h"
#include"vertexshaderd3d11.h"
#include"pixelshaderd3d11.h"
#include"debug.h"

namespace Maid { namespace Graphics {

DrawCommandExecuteD3D11::DrawCommandExecuteD3D11( const SPD3D11DEVICECONTEXT& pDevice, const SPRENDERTARGET& pDefaultRT )
  :m_pDevice(pDevice)
{
  m_NowRenderTarget[0] = pDefaultRT;
}

void DrawCommandExecuteD3D11::Begin()
{
//  m_pDevice->Begin(NULL);
}

void DrawCommandExecuteD3D11::End()
{
//  m_pDevice->End(NULL);
}

void DrawCommandExecuteD3D11::ClearDepthStencil( CLEARFLAG flag, float Depth, unt08 Stencil )
{
  UINT f;

  switch( flag )
  {
  case CLEARFLAG_DEPTH:         { f = D3D11_CLEAR_DEPTH; }break;
  case CLEARFLAG_STENCIL:       { f = D3D11_CLEAR_STENCIL; }break;
  case CLEARFLAG_DEPTHSTENCIL:  { f = D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL; }break;
  }

  for( int i=0; i<NUMELEMENTS(m_NowDepthStencil); ++i )
  {
    DepthStencilD3D11* pDepth = static_cast<DepthStencilD3D11*>(m_NowDepthStencil[i].get());
    if( pDepth==NULL ) { continue; }

    m_pDevice->ClearDepthStencilView( pDepth->pView.get(), f, Depth, Stencil );
  }

}

void DrawCommandExecuteD3D11::ClearRenderTarget( const COLOR_R32G32B32A32F& Col )
{
  const float c[4] =
  {
    Col.GetR(),
    Col.GetG(),
    Col.GetB(),
    Col.GetA(),
  };

  for( int i=0; i<NUMELEMENTS(m_NowRenderTarget); ++i )
  {
    RenderTargetD3D11* pColor = static_cast<RenderTargetD3D11*>(m_NowRenderTarget[i].get());
    if( pColor==NULL ) { continue; }

    m_pDevice->ClearRenderTargetView( pColor->pView.get(), c );
  }
}


inline D3D11_MAP MAPTYPEtoD3D11_MAP( IDrawCommand::MAPTYPE type )
{
  D3D11_MAP ret;
  switch( type )
  {
  case IDrawCommand::MAPTYPE_READ:      { ret = D3D11_MAP_READ; }break;
  case IDrawCommand::MAPTYPE_WRITE:     { ret = D3D11_MAP_WRITE; }break;
  case IDrawCommand::MAPTYPE_READ_WRITE:{ ret = D3D11_MAP_READ_WRITE; }break;
  case IDrawCommand::MAPTYPE_WRITE_DISCARD: { ret = D3D11_MAP_WRITE_DISCARD; }break;
  case IDrawCommand::MAPTYPE_WRITE_NO_OVERWRITE: { ret = D3D11_MAP_WRITE_NO_OVERWRITE; }break;
  }

  return ret;
}

void DrawCommandExecuteD3D11::ResourceMap( const SPRESOURCE& pResource, int SubLevel, MAPTYPE Type, unt32 Flag, MAPPEDRESOURCE& Mapped )
{
  Mapped.pData = NULL;
  Mapped.Pitch = 0;
  Mapped.Slice = 0;

  switch( pResource->GetType() )
  {
  case IResource::TYPE_BUFFER:
    {
      BufferD3D11* pBuffer = static_cast<BufferD3D11*>( pResource.get() );

      const D3D11_MAP t = MAPTYPEtoD3D11_MAP(Type);
      const UINT f = Flag;
      D3D11_MAPPED_SUBRESOURCE  Data;

      {
        const HRESULT ret = m_pDevice->Map( pBuffer->pBuffer.get(), SubLevel, t, f, &Data );
        if( FAILED(ret) ) 
        {
          MAID_WARNING( "ID3D11Buffer::Map() " << ret 
            << " D3D11_MAP:" << DebugStringD3D11(t)
            << " Flag:" << f
            );
          return; 
        }
      }
      Mapped.pData = Data.pData;
      Mapped.Pitch = pBuffer->GetParam().Length;
    }break;

  case IResource::TYPE_TEXTURE2D:
    {
      Texture2DD3D11* pBuffer = static_cast<Texture2DD3D11*>( pResource.get() );

      const UINT s = SubLevel;
      const D3D11_MAP t = MAPTYPEtoD3D11_MAP(Type);
      const UINT f = Flag;
      D3D11_MAPPED_SUBRESOURCE  Data;

      {
        const HRESULT ret = m_pDevice->Map( pBuffer->pTexture.get(), SubLevel, t, f, &Data );
        if( FAILED(ret) )
        {
          MAID_WARNING( "ID3D11Texture2D::Map() " << ret 
            << " D3D11_MAP:" << DebugStringD3D11(t)
            << " SubLevel:" << s
            << " Flag:" << f
            ); 
          return; 
        }
      }

      Mapped.pData = Data.pData;
      Mapped.Pitch = Data.RowPitch;
    }break;
  }

}

void DrawCommandExecuteD3D11::ResourceUnmap( const SPRESOURCE& pResource, int SubLevel )
{
  switch( pResource->GetType() )
  {
  case IResource::TYPE_BUFFER:
    {
      BufferD3D11* pBuffer = static_cast<BufferD3D11*>( pResource.get() );
      m_pDevice->Unmap( pBuffer->pBuffer.get(), SubLevel );
    }break;

  case IResource::TYPE_TEXTURE2D:
    {
      Texture2DD3D11* pBuffer = static_cast<Texture2DD3D11*>( pResource.get() );
      m_pDevice->Unmap( pBuffer->pTexture.get(), SubLevel );
    }break;
  }
}

}}

