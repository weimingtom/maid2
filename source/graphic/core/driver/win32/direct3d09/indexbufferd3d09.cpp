#include"indexbufferd3d09.h"

namespace Maid
{

IndexBufferD3D09::IndexBufferD3D09( IDirect3DIndexBuffer9* p )
{
	pBuffer.reset( p );

	pBuffer->GetDesc( &Desc );
}


}