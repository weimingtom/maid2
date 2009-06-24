#include"filereadrange.h"

#include"../../auxiliary/debug/assert.h"


namespace Maid
{

/*!
 	@class	FileReadRange FileReadRange.h
 	@brief	ファイルの特定区間を１ファイルとして扱うクラス 
 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
FileReadRange::FileReadRange( const String& FileName, size_t BeginPosition, size_t Size )
  : m_hFile(FileName)
  , m_BeginPosition(0)
  , m_Size(0)
{

}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
FileReadRange::~FileReadRange()
{
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルを開く
/*!
    引数などは IFileRead::Open() を参照
 */
FileReadRange::OPENRESULT FileReadRange::Open()
{
  Close();

  {
    const FileReadNormal::OPENRESULT ret = m_hFile.Open();
    if( ret!=FileReadNormal::OPENRESULT_OK ) { return OPENRESULT_ERROR; }
  }

  {
    //	開かれたファイルのファイルサイズを調べて　引数が大きすぎるようならエラー
    if( m_hFile.GetSize() < m_BeginPosition+m_Size )
    {
      MAID_ASSERT( m_hFile.GetSize() < m_BeginPosition+m_Size, 
        "指定されたファイルサイズが不正です" << m_BeginPosition << "-" << m_BeginPosition+m_Size );
    }
  }

  m_hFile.Seek( IFileRead::POSITION_BEGIN, m_BeginPosition );

  return OPENRESULT_OK;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 開いているか？
/*!
 	  引数などは IFileRead::IsOpen() を参照すること
 */
bool  FileReadRange::IsOpen() const
{
  return m_hFile.IsOpen();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルを読み込む
/*!
 *	引数などは IFileRead::Read() を参照すること
 */
size_t FileReadRange::Read( void* pData, size_t Size )
{
  MAID_ASSERT( !IsOpen(), "ファイルが開かれていません" );
  const size_t ReadSize = std::min(Size,m_Size-GetPosition());
  const size_t ret = m_hFile.Read( pData, ReadSize );

  return ret;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルサイズの取得
/*!
 *	引数などは IFileRead::GetSize() を参照すること
 */
size_t FileReadRange::GetSize() const
{
  MAID_ASSERT( !IsOpen(), "ファイルが開かれていません" );
  return m_Size;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在のファイルポジションの取得
/*!
 *	引数などは IFileRead::GetPosition() を参照すること
 */
size_t FileReadRange::GetPosition() const
{
  MAID_ASSERT( !IsOpen(), "ファイルが開かれていません" );
  return m_hFile.GetPosition()-m_BeginPosition;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルポジションの移動
/*!
 *	引数などは IFileRead::Seek() を参照すること
 */
void FileReadRange::Seek( IFileRead::POSITION Pos, int Size)
{
  MAID_ASSERT( !IsOpen(), "ファイルが開かれていません" );
  size_t NewPos;

  switch( Pos )
  {
  case IFileRead::POSITION_BEGIN:
    {
      if( Size<0 )  { NewPos = 0;    }
      else          { NewPos = Size; }
    }break;
  case IFileRead::POSITION_END:
    {
      if( 0<m_Size )  { NewPos = m_Size;    }
      else            { NewPos = Size; }
    }break;

  case IFileRead::POSITION_CURRENT:
    {
      NewPos = GetPosition() + Size;
      if( NewPos<0		  ) { NewPos = 0;			}
      if( m_Size<NewPos	) { NewPos = m_Size;    }

    }break;
  }

  m_hFile.Seek( IFileRead::POSITION_BEGIN, m_BeginPosition+NewPos );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 開いているファイルを閉じる
/*!
 *	引数などは IFileRead::Close() を参照すること
 */
void FileReadRange::Close()
{
  m_hFile.Close();
}


}
