#include"texture2dmemory.h"


namespace Maid
{

Texture2DMemory::Texture2DMemory()
{

}

Texture2DMemory::Texture2DMemory( const Graphics::SPTEXTURE2D& pTex )
{
  ITexture2D::Set( pTex );
}

}