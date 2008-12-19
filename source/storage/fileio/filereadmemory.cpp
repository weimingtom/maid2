#include"filereadmemory.h"
#include"../../auxiliary/debug/assert.h"

namespace Maid
{

/*!
  @class  FileReadMemory FileReadMemory.h
  @brief  メモリ上においてあるデータをファイルとして扱うクラス
 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
FileReadMemory::FileReadMemory() : m_pBegin(NULL),m_Size(0), m_Position(0)
{

}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
FileReadMemory::~FileReadMemory()
{
  Close();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルを開く(ポインタ先を保証しておくバージョン)
/*!
  @param	pBegin	[i ]	読み込み開始アドレス
  @param	Size    [i ]	読み込み範囲
 */
void FileReadMemory::Open( const boost::shared_array<unt08>& pBegin, size_t Size )
{
  OpenNoOwner( pBegin.get(), Size );
  m_pShared = pBegin;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルを開く(ポインタ先を保証しないバージョン)
/*!
 	このインスタンスがなくなるまで pBegin を解放してはいけません
 
 	@param	pBegin	[i ]	読み込み開始アドレス
 	@param	Size    [i ]	読み込み範囲 
 */
void	FileReadMemory::OpenNoOwner( const void* pBegin, size_t Size )
{
  MAID_ASSERT( pBegin==NULL, "引数の指定が不正です" );

  Close();

  m_pBegin = (unt08*)pBegin;
  m_Size   = Size;
  m_Position = 0;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルを読み込む
/*!
 *	引数などは FileRead::Read() を参照すること
 */
size_t FileReadMemory::Read( void* pData, size_t Size )
{
  MAID_ASSERT( !m_pBegin, "ファイルが開かれていません" );

  const size_t ReadSize = std::min( m_Size - m_Position, Size );

  if( ReadSize<=0 ) { return 0; }

  memcpy( pData, &(m_pBegin[m_Position]), ReadSize );

  m_Position += ReadSize;

  return ReadSize;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルサイズの取得
/*!
 *	引数などは FileRead::GetSize() を参照すること
 */
size_t FileReadMemory::GetSize() const
{
  MAID_ASSERT( !m_pBegin, "ファイルが開かれていません" );

  return m_Size;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在のファイルポジションの取得
/*!
 *	引数などは FileRead::GetPosition() を参照すること
 */
size_t FileReadMemory::GetPosition() const
{
  MAID_ASSERT( !m_pBegin, "ファイルが開かれていません" );

  return m_Position;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルポジションの移動
/*!
 *	引数などは FileRead::Seek() を参照すること
 */
void FileReadMemory::Seek( IFileRead::POSITION ePos, int Size )
{
  MAID_ASSERT( !m_pBegin, "ファイルが開かれていません" );

  switch( ePos )
  {
  case IFileRead::POSITION_BEGIN:
    {
      if( Size<0 )  { m_Position = 0; }
      else          { m_Position = Size; }
    }break;
  case IFileRead::POSITION_END:
    {
      if( 0<Size )  { m_Position = m_Size; }
      else          { m_Position = m_Size+Size; }
    }break;

  case IFileRead::POSITION_CURRENT:
    {
      m_Position += Size;
      if( m_Position < 0 )      { m_Position = 0; }
      if( m_Size < m_Position )	{ m_Position = m_Size; }
    }break;
  }
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 開いているファイルを閉じる
/*!
 *	引数などは FileRead::Close() を参照すること
 */
void FileReadMemory::Close()
{
  m_pShared.reset();
  m_pBegin     = NULL;
  m_Size       = 0;
  m_Position   = 0;
}


}
