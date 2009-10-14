#include"filereadfiles.h"

#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"

#include"../../import/md5/md5.h"

namespace Maid
{

/*!
 	@class	FileReadFiles filereadfiles.h
 	@brief	ファイルの特定区間を１ファイルとして扱うクラス 
 */

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! コンストラクタ
/*!
 */
FileReadFiles::FileReadFiles( const std::vector<ELEMENT>& dat )
  : m_Data(dat)
  , m_Handle(NULL)
  , m_Position(0)
  , m_OpenIndex(OPENINDEX_NONE)
{

}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! デストラクタ
/*!
 */
FileReadFiles::~FileReadFiles()
{
  Close();
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルを開く
/*!
    引数などは IFileRead::Open() を参照
 */
FileReadFiles::OPENRESULT FileReadFiles::Open()
{
  Close();

  const ELEMENT& element = m_Data[0];

  const OPENRESULT ret = FileOpen(element);
  if( ret!=OPENRESULT_OK ) { return ret; }

  m_OpenIndex = 0;

  return OPENRESULT_OK;
}

FileReadFiles::OPENRESULT FileReadFiles::FileOpen( const ELEMENT& element )
{
  const std::string str = String::ConvertMAIDtoSJIS(element.FileName);

  m_Handle = ::fopen( str.c_str(), "rb" );

  if( m_Handle==NULL )
  {
    MAID_WARNING( "ファイルのオープンに失敗" << element.FileName );
    return OPENRESULT_ERROR;
  }

  ::fseek( m_Handle, element.BeginPosition, SEEK_SET );

  m_Position = 0;
  m_ReadSize = element.Size;

  return OPENRESULT_OK;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 開いているか？
/*!
 	  引数などは IFileRead::IsOpen() を参照すること
 */
bool  FileReadFiles::IsOpen() const
{
  return m_OpenIndex!=OPENINDEX_NONE;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルを読み込む
/*!
 *	引数などは IFileRead::Read() を参照すること
 */
size_t FileReadFiles::Read( void* pData, size_t Size )
{
  MAID_ASSERT( !IsOpen(), "ファイルが開かれていません" );

  {
    //  末端まで読み込んでいたら　０ 
    if( (int)m_Data.size() <= m_OpenIndex ) { return 0; }
  }

  const size_t MAXSIZE = GetSize() - GetPosition();

  size_t len = std::min(MAXSIZE,Size);


  unt08* pTarget = (unt08*)pData;

  const ELEMENT& Element = m_Data[m_OpenIndex];

  while( true )
  {
    if( len==0 ) { break; }

    const size_t now_pos = ::ftell(m_Handle);
//    const size_t max_read= Element.BeginPosition+Element.Size - now_pos;

    const size_t readsize= std::min(len,m_ReadSize);
//    const size_t readbyte= ::fread( pTarget, readsize, 1, m_Handle );
    const size_t readbyte= ::fread( pTarget, 1, readsize, m_Handle );
    m_Position += readbyte;
    pTarget += readbyte;
    len -= readbyte;

    m_ReadSize -= readbyte;

    //  末端まで進んでいたら次のファイルへ
    //  次のファイルがなかったら終了
    if( m_ReadSize==0 )
    {
      ::fclose(m_Handle);

      ++m_OpenIndex;

      if( m_OpenIndex < (int)m_Data.size() )
      {
        FileOpen(m_Data[m_OpenIndex]);
      }else
      {
        m_Handle = NULL;
        break;
      }
    }
  }

  return Size-len;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルサイズの取得
/*!
 *	引数などは IFileRead::GetSize() を参照すること
 */
size_t FileReadFiles::GetSize() const
{
  MAID_ASSERT( !IsOpen(), "ファイルが開かれていません" );

  size_t ret = 0;

  for( int i=0; i<(int)m_Data.size(); ++i )
  {
    const ELEMENT& Element = m_Data[i];

    ret += Element.Size;
  }

  return ret;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 現在のファイルポジションの取得
/*!
 *	引数などは IFileRead::GetPosition() を参照すること
 */
size_t FileReadFiles::GetPosition() const
{
  MAID_ASSERT( !IsOpen(), "ファイルが開かれていません" );
  return m_Position;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! ファイルポジションの移動
/*!
 *	引数などは IFileRead::Seek() を参照すること
 */
void FileReadFiles::Seek( IFileRead::POSITION Pos, int Size )
{
  MAID_ASSERT( !IsOpen(), "ファイルが開かれていません" );

  fclose(m_Handle);

  size_t NewPos;

  const size_t TotalSize = GetSize();
  switch( Pos )
  {
  case IFileRead::POSITION_BEGIN:
    {
      if( Size<0 )  { NewPos = 0;    }
      else          { NewPos = Size; }
    }break;

  case IFileRead::POSITION_END:
    {
      if( 0<TotalSize )  { NewPos = TotalSize;    }
      else            { NewPos = Size; }
    }break;

  case IFileRead::POSITION_CURRENT:
    {
      NewPos = GetPosition() + Size;
      if( NewPos<0		  ) { NewPos = 0;			}
      if( TotalSize<NewPos	) { NewPos = TotalSize;    }

    }break;
  }

  size_t zan = NewPos;

  for( int i=0; i<(int)m_Data.size(); ++i )
  {
    const ELEMENT& e  = m_Data[i];

    if( zan < e.Size )
    {
      FileOpen( e );

      m_OpenIndex = i;
      ::fseek( m_Handle, SEEK_CUR, zan );
      m_ReadSize -= zan;
      break;
    }else
    {
      zan -= e.Size;
    }
  }

  m_Position = NewPos;

}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
//! 開いているファイルを閉じる
/*!
 *	引数などは IFileRead::Close() を参照すること
 */
void FileReadFiles::Close()
{
  if( m_Handle!=NULL )
  {
    fclose(m_Handle);
    m_Handle = NULL;
  }

  m_Position = 0;
  m_OpenIndex= OPENINDEX_NONE;
}


}
