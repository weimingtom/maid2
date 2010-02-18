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
FileReadNormal::FileReadNormal( const String& FileName ) 
	: m_FileName(FileName)
  , m_Handle(NULL)
  , m_Size(0)
  , m_Position(0)
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
    引数などは IFileRead::Open() を参照
 */
FileReadNormal::OPENRESULT FileReadNormal::Open()
{
  Close();

  const String& FileName = m_FileName;

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
//! 開いているか？
/*!
 	  引数などは IFileRead::IsOpen() を参照すること
 */
bool  FileReadNormal::IsOpen() const
{
  return m_Handle!=NULL;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルを読み込む
/*!
 	引数などは IFileRead::Read() を参照すること
 */
size_t FileReadNormal::Read( void* pData, size_t Size )
{
  MAID_ASSERT( !IsOpen(), "ファイルが開かれていません" );

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
 	引数などは IFileRead::GetSize() を参照すること
 */
size_t FileReadNormal::GetSize() const
{
  MAID_ASSERT( !IsOpen(), "ファイルが開かれていません" );
  return m_Size;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在のファイルポジションの取得
/*!
 	引数などは IFileRead::GetPosition() を参照すること
 */
size_t FileReadNormal::GetPosition() const
{
  MAID_ASSERT( !IsOpen(), "ファイルが開かれていません" );
  return m_Position;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルポジションの移動
/*!
 	引数などは IFileRead::Seek() を参照すること
 */
void FileReadNormal::Seek( IFileRead::POSITION Pos, int Size)
{
  MAID_ASSERT( !IsOpen(), "ファイルが開かれていません" );

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
 	引数などは IFileRead::Close() を参照すること
 */
void FileReadNormal::Close()
{
  if( !IsOpen() ) { return ; }

  if( m_Handle!=NULL )
  {
    ::fclose( m_Handle );
    m_Handle  = NULL;
    m_Size    = 0;
    m_Position= 0;
  }
}

}
