#include"filewrite.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"

namespace Maid
{
/*!
 	@class	FileWrite FileWrite.h
 	@brief	ファイルにたいして書き込みのみのクラス
 */ 
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
FileWrite::FileWrite()
  :m_Handle(NULL)
{

}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
FileWrite::~FileWrite()
{
	Close();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 書き込むファイルを開く
/*!
 	@param	FileName	[i ]	開くファイル名
 	@param	eOption		[i ]	開くモード

  @return オープンに成功したら OPENRESULT_OK
\n        失敗したら OPENRESULT_ERROR
 */
FileWrite::OPENRESULT FileWrite::Open( const String& FileName, FileWrite::OPENOPTION eOption )
{
  Close();

  std::string OpenMode;

  switch( eOption )
  {
  case OPENOPTION_NEW:	{ OpenMode = "wb";  }break;
  case OPENOPTION_OPEN:	{ OpenMode = "r+b"; }break;
  case OPENOPTION_ADD:	{ OpenMode = "ab";  }break;
  }

  std::string str = String::ConvertMAIDtoSJIS(FileName);

  m_Handle = ::fopen( str.c_str(), OpenMode.c_str() );

  if( m_Handle==NULL )
  {
    MAID_WARNING( "ファイルのオープンに失敗" << str );
    return OPENRESULT_ERROR;
  }
  return OPENRESULT_OK;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 書き込みカーソルの移動
/*!
    ファイル範囲を超える指定があった場合　範囲内になるようにクリップされる
    @param	ePos	[i ]	基準点
    @param	Size	[i ]	移動量（バイト単位）
 */
void	FileWrite::Seek( POSITION ePos, int Size )
{
  MAID_ASSERT( m_Handle==NULL, "ファイルが開かれていません" );
  if( m_Handle==NULL ) { return ; }

  long Origin;
  switch( ePos )
  {
  case POSITION_BEGIN:		{ Origin = SEEK_SET; }break;
  case POSITION_END:      { Origin = SEEK_END; }break;
  case POSITION_CURRENT:	{ Origin = SEEK_CUR; }break;
  }

  ::fseek( m_Handle, Size, Origin );
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルに書き込む
/*!
    @param	pData     [i ]	書き込みたいデータの先頭アドレス
    @param	DataSize  [i ]	書き込むサイズ（バイト単位）

    @return	実際に書き込んだバイト数
 */
size_t FileWrite::Write( const void* pData, size_t DataSize )
{
  if( DataSize==0 ) { return 0; }

  MAID_ASSERT( m_Handle==NULL, "ファイルが開かれていません" );
  if( m_Handle==NULL ) { return 0; }

  const int count = (int)::fwrite( pData, DataSize, 1, m_Handle );

  if( count!=1 )
  {
    MAID_WARNING( "書き込んだ項目数が違います 戻り値 " << count );
  }

  return DataSize;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 開いているファイルを閉じる
/*!
 */
void FileWrite::Close()
{
  if( m_Handle==NULL ) { return ; }

  ::fclose( m_Handle );
  m_Handle = NULL;
}

size_t  FileWrite::GetSize()    const
{
  MAID_ASSERT( m_Handle==NULL, "ファイルが開かれていません" );
  if( m_Handle==NULL ) { return 0; }

  const long now = ::ftell( m_Handle );

  ::fseek( m_Handle, 0, SEEK_END );

  const int size = ::ftell( m_Handle );
  ::fseek( m_Handle, now, SEEK_SET );

  return size;
}

size_t  FileWrite::GetPosition()const
{
  MAID_ASSERT( m_Handle==NULL, "ファイルが開かれていません" );
  if( m_Handle==NULL ) { return 0; }
  return ::ftell( m_Handle );
}

}
