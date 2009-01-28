#include"drawcommandexecuted3d09.h"
#include"deviced3d09.h"

//  ここは各種Drawを書く

namespace Maid { namespace Graphics {

void DrawCommandExecuteD3D09::Draw( size_t UseVertexCount, size_t StartVertex )
{
  const D3DPRIMITIVETYPE type = m_PrimitiveType;
  const int count = CalcPrimitiveCount(UseVertexCount);

  const HRESULT ret = m_Device.GetDevice()->DrawPrimitive( type, StartVertex, count );
  if( FAILED(ret) ) {MAID_WARNING(MAIDTEXT("IDirect3DDevice9::DrawPrimitive()")); return; }
}

void DrawCommandExecuteD3D09::DrawIndexed( size_t UseIndexCount, size_t StartIndex, size_t OffsetVertex )
{
  const D3DPRIMITIVETYPE type = m_PrimitiveType;
  const int count = CalcPrimitiveCount(UseIndexCount);

  const HRESULT ret = m_Device.GetDevice()->DrawIndexedPrimitive(
                        type,
                        StartIndex,
                        0,
                        UseIndexCount,
                        m_IndexOffset,
                        count
                        );
  if( FAILED(ret) ) {MAID_WARNING(MAIDTEXT("IDirect3DDevice9::DrawIndexedPrimitive()")); return; }
}

}}

