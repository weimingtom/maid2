#include"vertexdeclarationbufferd3d09.h"


namespace Maid
{

VertexDeclarationBufferD3D09::VertexDeclarationBufferD3D09( IDirect3DVertexDeclaration9* pBuf )
{
	pBuffer.reset( pBuf );
}

}


