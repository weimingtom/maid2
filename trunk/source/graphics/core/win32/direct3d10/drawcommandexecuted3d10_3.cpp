#include"drawcommandexecuted3d10.h"
#include"deviced3d10_0.h"

//  ここは各種Drawを書く

namespace Maid { namespace Graphics {

void DrawCommandExecuteD3D10::Draw( size_t UseVertexCount, size_t StartVertex )
{
  GetDevice()->Draw( UseVertexCount, StartVertex );
}

void DrawCommandExecuteD3D10::DrawIndexed( size_t UseIndexCount, size_t StartIndex, size_t OffsetVertex )
{
  GetDevice()->DrawIndexed( UseIndexCount, StartIndex, OffsetVertex );
}

}}

