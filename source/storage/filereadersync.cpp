#include"filereadersync.h"
#include"../auxiliary/debug/warning.h"


namespace Maid
{
  /*!
      @class  FileReaderSync filereadersync.h
      @brief  同期ファイル読み込みクラス
              非同期したい場合は FileReader を使います
  */

  bool FileReaderSync::Open( const String& filename )
  {
    m_Reader.Open( filename );
    return m_Reader.Sync();
  }

  size_t FileReaderSync::Read( void* pData, size_t size )
  {
    if( size==0 ) { return 0; }

    const size_t read_size = m_Reader.Read( pData, size );
    m_Reader.Sync();

    return read_size;
  }

  bool FileReaderSync::Seek( POSITION Pos, int Size )
  {
    FileReader::POSITION p;
    switch( Pos )
    {
    case POSITION_BEGIN:    { p = FileReader::POSITION_BEGIN;   }break;
    case POSITION_END:      { p = FileReader::POSITION_END;     }break;
    case POSITION_CURRENT:  { p = FileReader::POSITION_CURRENT; }break;
    }

    m_Reader.Seek( p, Size );
    return m_Reader.Sync();
  }

  bool FileReaderSync::Close()
  {
    m_Reader.Close(true);
    return false;
  }

  size_t  FileReaderSync::GetSize() const
  {
    return m_Reader.GetSize();
  }

  size_t  FileReaderSync::GetPosition() const
  {
    return m_Reader.GetPosition();
  }



}

