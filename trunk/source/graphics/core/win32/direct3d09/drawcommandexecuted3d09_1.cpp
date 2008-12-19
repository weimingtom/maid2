#include"drawcommandexecuted3d09.h"

namespace Maid { namespace Graphics {

DrawCommandExecuteD3D09::DrawCommandExecuteD3D09( const SPD3D09DEVICE& pDev, const OBJECTDESCMAP& ObjectMap )
  :m_pDevice(pDev), m_ObjectDescMap(ObjectMap)
{

}

ObjectDesc::TYPE DrawCommandExecuteD3D09::GetType( const hObject& obj )
{
  OBJECTDESCMAP::const_iterator ite = m_ObjectDescMap.find( obj.ID );

  if( ite==m_ObjectDescMap.end() ) { return ObjectDesc::TYPE_UNKWON; }

  return ite->second->Type;
}


void DrawCommandExecuteD3D09::Begin()
{

}

void DrawCommandExecuteD3D09::End()
{

}

void DrawCommandExecuteD3D09::ClearDepthStencil( CLEARFLAG flag, float Depth, unt08 Stencil )
{

}

void DrawCommandExecuteD3D09::ClearRenderTarget( const COLOR_A32B32G32R32F& Col )
{
  const unt08 r = BITCONVERT32Fto08I(Col.GetR());
  const unt08 g = BITCONVERT32Fto08I(Col.GetG());
  const unt08 b = BITCONVERT32Fto08I(Col.GetB());

  m_pDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(r,g,b), 0, 0 );

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

