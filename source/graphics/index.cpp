#include"index.h"

namespace Maid
{
  /*!
      @class  Index index.h
      @brief  頂点バッファ
  */


Index::Index()
{

}

Index::~Index()
{
  Delete();
}

//! 作成したが最後、書き込めないバッファ
/*!
    @param  pData  [i ]  流し込むデータ
    @param  Length [i ]  データの大きさ
 */
void Index::Create( const void* pData, size_t Length )
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
  param.BindFlags = Graphics::RESOURCEBINDFLAG_INDEX;
  param.CPUAccessFlags = 0;
  param.MiscFlags = 0;

  Graphics::SPBUFFER obj = pDevice->CreateBuffer( param, &Data );

  IIndex::Set( obj );
}

void Index::Delete()
{
  IIndex::Clear();
}

}