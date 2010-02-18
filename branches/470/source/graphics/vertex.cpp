#include"vertex.h"

namespace Maid
{
  /*!
      @class  Vertex vertex.h
      @brief  頂点バッファ
  */





Vertex::Vertex()
{

}

Vertex::~Vertex()
{
  Delete();
}


//! あとから書き込めるバッファ
/*!
    @param  Length [i ]  バッファの大きさ
 */
void Vertex::Create( size_t Length )
{
  Delete();
  const Graphics::SPDEVICE& pDevice = GlobalPointer<GraphicsCore>::Get()->GetDevice();

  Graphics::CREATEBUFFERPARAM param;

  param.Length = Length;
  param.Usage  = Graphics::RESOURCEUSAGE_DYNAMIC;
  param.BindFlags  = Graphics::RESOURCEBINDFLAG_VERTEX;
  param.CPUAccessFlags = Graphics::CPUACCESSFLAG_WRITE;
  param.MiscFlags = 0;

  Graphics::SPBUFFER obj = pDevice->CreateBuffer( param, NULL );

  IVertex::Set( obj );
}

//! 作成したが最後、書き込めないバッファ
/*!
    @param  pData  [i ]  流し込むデータ
    @param  Length [i ]  データの大きさ
 */
void Vertex::Create( const void* pData, size_t Length )
{
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