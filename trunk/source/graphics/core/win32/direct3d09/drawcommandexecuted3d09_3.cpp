#include"drawcommandexecuted3d09.h"
#include"deviced3d09.h"

//  ここは各シェーダーステージのset,getを書く

namespace Maid { namespace Graphics {

void DrawCommandExecuteD3D09::Draw( int UseVertexCount, int StartVertex )
{
  D3DPRIMITIVETYPE type = D3DPT_TRIANGLELIST;
  const int count = UseVertexCount / 3;

//  m_Device.GetDevice()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
//  m_Device.GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
//  m_Device.GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );

  const HRESULT ret = m_Device.GetDevice()->DrawPrimitive( type, StartVertex, count );
  if( FAILED(ret) ) {MAID_WARNING(MAIDTEXT("IDirect3DDevice9::DrawPrimitive()")); return; }
}


}}

