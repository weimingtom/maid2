#include"drawcommandexecuted3d09.h"
#include"deviced3d09.h"

namespace Maid { namespace Graphics {

DrawCommandExecuteD3D09::DrawCommandExecuteD3D09( const DeviceD3D09& Device  )
  :m_Device(Device)
{

}

ObjectDesc::TYPE DrawCommandExecuteD3D09::GetType( const hObject& obj )
{
  const OBJECTDESCMAP& map = m_Device.GetOBJECTDESCMAP();
  OBJECTDESCMAP::const_iterator ite = map.find( obj.ID );

  if( ite==map.end() ) { return ObjectDesc::TYPE_UNKWON; }

  return ite->second->Type;
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

}

void DrawCommandExecuteD3D09::ClearRenderTarget( const COLOR_A32B32G32R32F& Col )
{
  const unt08 r = BITCONVERT32Fto08I(Col.GetR());
  const unt08 g = BITCONVERT32Fto08I(Col.GetG());
  const unt08 b = BITCONVERT32Fto08I(Col.GetB());

  const HRESULT ret = m_Device.GetDevice()->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(r,g,b), 0, 0 );
  if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DDevice9::Clear()")); return; }
}


void DrawCommandExecuteD3D09::ResourceMap( const hResource& Resource, int SubLevel, MAPTYPE Type, unt32 Flag, MAPRESOURCE& Mapped )
{
  Mapped.pData = NULL;
  Mapped.RowPitch = 0;
  Mapped.DepthPitch = 0;

  switch( GetType(Resource) )
  {
  case ObjectDesc::TYPE_RESOURCEVERTEX:
    {
      IDirect3DVertexBuffer9* p = (IDirect3DVertexBuffer9*)Resource.ID;
     
      const HRESULT ret = p->Lock( 0, 0, &(Mapped.pData), D3DLOCK_DISCARD );
      if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DVertexBuffer9::Lock()")); return; }
    }break;
  }
}

void DrawCommandExecuteD3D09::ResourceUnmap( const hResource& Resource, int SubLevel )
{
  switch( GetType(Resource) )
  {
  case ObjectDesc::TYPE_RESOURCEVERTEX:
    {
      IDirect3DVertexBuffer9* p = (IDirect3DVertexBuffer9*)Resource.ID;
     
      const HRESULT ret = p->Unlock();
      if( FAILED(ret) ) { MAID_WARNING(MAIDTEXT("IDirect3DVertexBuffer9::Unlock()")); return; }
    }break;
  }
}


}}

