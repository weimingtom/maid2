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

void Vertex::Create( size_t Length )
{
  //  あとから読み書きできるバッファ
  Delete();
  const Graphics::SPDEVICE& pDevice = GlobalPointer<GraphicsCore>::Get()->GetDevice();

  Graphics::SUBRESOURCE Data;
  Data.pData = NULL;
  Data.Pitch = 0;
  Data.Slice = 0;

  Graphics::CREATEBUFFERPARAM param;

  param.Length = Length;
  param.Usage  = Graphics::RESOURCEUSAGE_DYNAMIC;
  param.BindFlags  = Graphics::RESOURCEBINDFLAG_VERTEX;
  param.CPUAccessFlags = Graphics::CPUACCESSFLAG_WRITE;
  param.MiscFlags = 0;

  Graphics::SPBUFFER obj = pDevice->CreateBuffer( param, &Data );

  IVertex::Set( obj );
}

void Vertex::Create( const void* pData, size_t Length )
{
  //  最初から固定的なバッファ
  Delete();
  const Graphics::SPDEVICE& pDevice = GlobalPointer<GraphicsCore>::Get()->GetDevice();

  Graphics::SUBRESOURCE Data;
  Data.pData = pData;
  Data.Pitch = Length;
  Data.Slice = 0;

  Graphics::CREATEBUFFERPARAM param;

  param.Length = Length;
  param.Usage  = Graphics::RESOURCEUSAGE_IMMUTABLE;
  param.BindFlags = Graphics::RESOURCEBINDFLAG_VERTEX;
  param.CPUAccessFlags = 0;
  param.MiscFlags = 0;

  Graphics::SPBUFFER obj = pDevice->CreateBuffer( param, &Data );

  IVertex::Set( obj );
}

void Vertex::Delete()
{
  IVertex::Clear();
}

}