#include"drawcommandexecuted3d10.h"
#include"deviced3d10_0.h"

#include"bufferd3d10.h"
#include"texture2dd3d10.h"
#include"rendertargetd3d10.h"
#include"depthstencild3d10.h"
#include"materiald3d10.h"
#include"inputlayoutd3d10.h"
#include"rasterizerstated3d10.h"
#include"samplerstated3d10.h"
#include"blendstated3d10.h"
#include"vertexshaderd3d10.h"
#include"pixelshaderd3d10.h"
#include"debug.h"

namespace Maid { namespace Graphics {

DrawCommandExecuteD3D10::DrawCommandExecuteD3D10( const DeviceD3D10_0& Device  )
  :m_Device(Device)
{
  m_NowRenderTarget[0] = Device.GetCurrentRenderTarget();
}



const SPD3D10DEVICE& DrawCommandExecuteD3D10::GetDevice()
{
  return m_Device.GetDevice();
}


void DrawCommandExecuteD3D10::Begin()
{
}

void DrawCommandExecuteD3D10::End()
{
}

void DrawCommandExecuteD3D10::ClearDepthStencil( CLEARFLAG flag, float Depth, unt08 Stencil )
{
  UINT f;

  switch( flag )
  {
  case CLEARFLAG_DEPTH:         { f = D3D10_CLEAR_DEPTH; }break;
  case CLEARFLAG_STENCIL:       { f = D3D10_CLEAR_STENCIL; }break;
  case CLEARFLAG_DEPTHSTENCIL:  { f = D3D10_CLEAR_DEPTH|D3D10_CLEAR_STENCIL; }break;
  }

  for( int i=0; i<NUMELEMENTS(m_NowDepthStencil); ++i )
  {
    DepthStencilD3D10* pDepth = static_cast<DepthStencilD3D10*>(m_NowDepthStencil[i].get());
    if( pDepth==NULL ) { continue; }

    GetDevice()->ClearDepthStencilView( pDepth->pView.get(), flag, Depth, Stencil );
  }

}

void DrawCommandExecuteD3D10::ClearRenderTarget( const COLOR_R32G32B32A32F& Col )
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
    RenderTargetD3D10* pColor = static_cast<RenderTargetD3D10*>(m_NowRenderTarget[i].get());
    if( pColor==NULL ) { continue; }

    GetDevice()->ClearRenderTargetView( pColor->pView.get(), c );
  }
}


inline D3D10_MAP MAPTYPEtoD3D10_MAP( IDrawCommand::MAPTYPE type )
{
  D3D10_MAP ret;
  switch( type )
  {
  case IDrawCommand::MAPTYPE_READ:      { ret = D3D10_MAP_READ; }break;
  case IDrawCommand::MAPTYPE_WRITE:     { ret = D3D10_MAP_WRITE; }break;
  case IDrawCommand::MAPTYPE_READ_WRITE:{ ret = D3D10_MAP_READ_WRITE; }break;
  case IDrawCommand::MAPTYPE_WRITE_DISCARD: { ret = D3D10_MAP_WRITE_DISCARD; }break;
  case IDrawCommand::MAPTYPE_WRITE_NO_OVERWRITE: { ret = D3D10_MAP_WRITE_NO_OVERWRITE; }break;
  }

  return ret;
}

void DrawCommandExecuteD3D10::ResourceMap( const SPRESOURCE& pResource, int SubLevel, MAPTYPE Type, unt32 Flag, MAPPEDRESOURCE& Mapped )
{
  Mapped.pData = NULL;
  Mapped.Pitch = 0;
  Mapped.Slice = 0;

  switch( pResource->GetType() )
  {
  case IResource::TYPE_BUFFER:
    {
      BufferD3D10* pBuffer = static_cast<BufferD3D10*>( pResource.get() );

      const D3D10_MAP t = MAPTYPEtoD3D10_MAP(Type);
      const UINT f = Flag;
      void* pData = NULL;

      {
        const HRESULT ret = pBuffer->pBuffer->Map( t, f, &pData );
        if( FAILED(ret) ) 
        {
          MAID_WARNING( "ID3D10Buffer::Map() " << ret 
            << " D3D10_MAP:" << DebugStringD3D10(t)
            << " Flag:" << f
            );
          return; 
        }
      }
      Mapped.pData = pData;
      Mapped.Pitch = pBuffer->GetParam().Length;
    }break;

  case IResource::TYPE_TEXTURE2D:
    {
      Texture2DD3D10* pBuffer = static_cast<Texture2DD3D10*>( pResource.get() );

      const UINT s = SubLevel;
      const D3D10_MAP t = MAPTYPEtoD3D10_MAP(Type);
      const UINT f = Flag;
      D3D10_MAPPED_TEXTURE2D dat;

      {
        const HRESULT ret = pBuffer->pTexture->Map( s, t, f, &dat );
        if( FAILED(ret) )
        {
          MAID_WARNING( "ID3D10Texture2D::Map() " << ret 
            << " D3D10_MAP:" << DebugStringD3D10(t)
            << " SubLevel:" << s
            << " Flag:" << f
            ); 
          return; 
        }
      }

      Mapped.pData = dat.pData;
      Mapped.Pitch = dat.RowPitch;
    }break;
  }
}

void DrawCommandExecuteD3D10::ResourceUnmap( const SPRESOURCE& pResource, int SubLevel )
{
  switch( pResource->GetType() )
  {
  case IResource::TYPE_BUFFER:
    {
      BufferD3D10* pBuffer = static_cast<BufferD3D10*>( pResource.get() );
      pBuffer->pBuffer->Unmap();
    }break;

  case IResource::TYPE_TEXTURE2D:
    {
      Texture2DD3D10* pBuffer = static_cast<Texture2DD3D10*>( pResource.get() );
      pBuffer->pTexture->Unmap(SubLevel);
    }break;
  }
}

}}

