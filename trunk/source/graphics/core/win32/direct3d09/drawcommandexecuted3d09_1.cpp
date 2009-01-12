#include"drawcommandexecuted3d09.h"
#include"deviced3d09.h"

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

DrawCommandExecuteD3D09::DrawCommandExecuteD3D09( const DeviceD3D09& Device  )
  :m_Device(Device)
{
  m_PrimitiveType = D3DPT_POINTLIST;

  m_NowRenderTarget[0] = Device.GetDefaultRenderTarget();
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 指定した頂点数、現在の変換ルールを元に、プリミティブの数を求める
/*!
    @param	VertexCount	[i ]	頂点数

    @return プリミティブ個数
 */
int DrawCommandExecuteD3D09::CalcPrimitiveCount( int VertexCount )
{
  int ret = 0;

  switch( m_PrimitiveType )
  {
  case D3DPT_POINTLIST:     { ret = VertexCount; }break;
  case D3DPT_LINELIST:      { ret = VertexCount/2; }break;
  case D3DPT_LINESTRIP:     { ret = VertexCount-1; }break;
  case D3DPT_TRIANGLELIST:  { ret = VertexCount/3; }break;
  case D3DPT_TRIANGLESTRIP: { ret = VertexCount-2; }break;
  }

  return ret;
}

const SPD3D09DEVICE& DrawCommandExecuteD3D09::GetDevice()
{
  return m_Device.GetDevice();
}


void DrawCommandExecuteD3D09::Begin()
{
  const HRESULT ret = m_Device.GetDevice()->BeginScene();
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::BeginScene()")); return; }
}

void DrawCommandExecuteD3D09::End()
{
  const HRESULT ret = m_Device.GetDevice()->EndScene();
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::EndScene()")); return; }

}

void DrawCommandExecuteD3D09::ClearDepthStencil( CLEARFLAG flag, float Depth, unt08 Stencil )
{
  DWORD f;

  switch( flag )
  {
  case CLEARFLAG_DEPTH:         { f = D3DCLEAR_ZBUFFER; }break;
  case CLEARFLAG_STENCIL:       { f = D3DCLEAR_STENCIL; }break;
  case CLEARFLAG_DEPTHSTENCIL:  { f = D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL; }break;
  }

  const HRESULT ret = GetDevice()->Clear( 0, NULL, flag, 0, Depth, Stencil );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::Clear()")); return; }
}

void DrawCommandExecuteD3D09::ClearRenderTarget( const COLOR_R32G32B32A32F& Col )
{
  const unt08 r = BITCONVERT32Fto08I(Col.GetR());
  const unt08 g = BITCONVERT32Fto08I(Col.GetG());
  const unt08 b = BITCONVERT32Fto08I(Col.GetB());

  const HRESULT ret = GetDevice()->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(r,g,b), 0, 0 );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::Clear()")); return; }
}


void DrawCommandExecuteD3D09::ResourceMap( const SPRESOURCE& pResource, int SubLevel, MAPTYPE Type, unt32 Flag, MAPPEDRESOURCE& Mapped )
{
  Mapped.pData = NULL;
  Mapped.Pitch = 0;
  Mapped.Slice = 0;


  switch( pResource->GetType() )
  {
  case IResource::TYPE_BUFFER:
    {
      const CREATEBUFFERPARAM& param = static_cast<IBuffer*>(pResource.get())->GetParam();

      if( IsFlag(param.BindFlags,RESOURCEBINDFLAG_VERTEX) )
      {
        VertexD3D09* p = static_cast<VertexD3D09*>(pResource.get());
       
        const HRESULT ret = p->pBuffer->Lock( 0, 0, &(Mapped.pData), D3DLOCK_DISCARD );
        if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DVertexBuffer9::Lock()")); return; }

        Mapped.Pitch = param.Length;
      }
    }break;
  }
}

void DrawCommandExecuteD3D09::ResourceUnmap( const SPRESOURCE& pResource, int SubLevel )
{
  switch( pResource->GetType() )
  {
  case IResource::TYPE_BUFFER:
    {
      const CREATEBUFFERPARAM& param = static_cast<IBuffer*>(pResource.get())->GetParam();

      if( IsFlag(param.BindFlags,RESOURCEBINDFLAG_VERTEX) )
      {
        VertexD3D09* p = static_cast<VertexD3D09*>(pResource.get());
       
      const HRESULT ret = p->pBuffer->Unlock();
      if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DVertexBuffer9::Unlock()")); return; }
      }

    }break;
  }
}


}}

