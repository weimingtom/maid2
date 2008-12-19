#include"vertex.h"


namespace Maid
{

Vertex::Vertex()
{

}

Vertex::~Vertex()
{
  Delete();
}

void Vertex::Create( size_t Length, unt32 Format )
{
  Create( NULL, Length, Format );
}

void Vertex::Create( const void* pData, size_t Length, unt32 Format )
{
  const Graphics::SPDEVICE& pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();

  Graphics::IDevice::CREATEVERTEXPARAM param;
  param.pData  = pData;
  param.Length = Length;
  param.Format = Format;

  Graphics::hVertex obj = pDevice->CreateVertex( param );

  IVertex::Set( obj );
}

void Vertex::Delete()
{
  const Graphics::hVertex& obj = IVertex::Get();
  if( obj.empty() ) { return ; }

  const Graphics::SPDEVICE& pDevice = GlobalPointer<Graphics::Core>::Get()->GetDevice();

  pDevice->DeleteObject( obj );
  IVertex::Clear();

}

}