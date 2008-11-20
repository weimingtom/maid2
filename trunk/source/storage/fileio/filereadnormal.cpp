#include"filereadnormal.h"

#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"

#include<sys/stat.h>

namespace Maid
{
/*!
 	@class	filereadnormal filereadnormal.h
 	@brief	通常のファイル読み込みクラス
\n			ライブラリはＣランタイムを使っています。
 
 */


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
FileReadNormal::FileReadNormal() 
	: m_Handle(NULL), m_Size(0), m_Position(0)
{

}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
FileReadNormal::~FileReadNormal()
{
	Close();
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルを開く
/*!
  @param	FileName [i ]	開くファイル名

  @return オープンに成功したら OPENRESULT_OK
\n        失敗したら OPENRESULT_ERROR
 */
FileReadNormal::OPENRESULT FileReadNormal::Open( const String& FileName )
{
  Close();

  std::string str = String::ConvertMAIDtoSJIS(FileName);

  m_Handle = ::fopen( str.c_str(), "rb" );

  if( m_Handle==NULL )
  {
    MAID_WARNING( "ファイルのオープンに失敗" << FileName );
    return OPENRESULT_ERROR;
  }

  //	ファイルサイズを調べる
  struct stat StatFile; 
  ::fstat( ::fileno(m_Handle), &StatFile );

  m_Size     = StatFile.st_size;
  m_Position = 0;

  return OPENRESULT_OK;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルを読み込む
/*!
 	引数などは FileRead::Read() を参照すること
 */
size_t FileReadNormal::Read( void* pData, size_t Size )
{
  MAID_ASSERT( m_Handle==NULL, "ファイルが開かれていません" );

  if( Size==0 ) { return 0; }
  //	ファイルの末端を越えないように読み込む
  int ReadSize;

  if( m_Position+Size>m_Size )  { ReadSize = m_Size-m_Position; }
  else                          { ReadSize = Size; }

  ::fread( pData, ReadSize, 1, m_Handle );

  m_Position += ReadSize;

  return ReadSize;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルサイズの取得
/*!
 	引数などは FileRead::GetSize() を参照すること
 */
size_t FileReadNormal::GetSize() const
{
	MAID_ASSERT( m_Handle==NULL, "ファイルが開かれていません" );
	return m_Size;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在のファイルポジションの取得
/*!
 	引数などは FileRead::GetPosition() を参照すること
 */
size_t FileReadNormal::GetPosition() const
{
	MAID_ASSERT( m_Handle==NULL, "ファイルが開かれていません" );
	return m_Position;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルポジションの移動
/*!
 	引数などは FileRead::Seek() を参照すること
 */
void FileReadNormal::Seek( IFileRead::POSITION Pos, int Size)
{
	MAID_ASSERT( m_Handle==NULL, "ファイルが開かれていません" );

	long Origin;
	switch( Pos )
	{
	case IFileRead::POSITION_BEGIN:		{ Origin = SEEK_SET; }break;
	case IFileRead::POSITION_END:      { Origin = SEEK_END; }break;
	case IFileRead::POSITION_CURRENT:	{ Origin = SEEK_CUR; }break;
	}
	
	::fseek( m_Handle, Size, Origin );

	m_Position = ::ftell( m_Handle );
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 開いているファイルを閉じる
/*!
 	引数などは FileRead::Close() を参照すること
 */
void FileReadNormal::Close()
{
	if( m_Handle!=NULL )
	{
		::fclose( m_Handle );
		m_Handle  = NULL;
		m_Size    = 0;
		m_Position= 0;
	}
}

}
