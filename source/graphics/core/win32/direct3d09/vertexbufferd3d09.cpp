#include"vertexbufferd3d09.h"



namespace Maid
{

VertexBufferD3D09::VertexBufferD3D09( IDirect3DVertexBuffer9* p, int str )
{
	pBuffer.reset( p );
	pBuffer->GetDesc( &Desc );
	Stride = str;
}



}


