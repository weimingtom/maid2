#include"oggfile.h"

#include"oggpage.h"


namespace Maid { namespace Movie { namespace Xiph {

const long READSIZE_MIN = 4096;
const long READSIZE_MAX = 1024*1024;


OggFile::OggFile( const String& filename )
  :m_FileName(filename)
  ,m_ReadSize(0)
  ,m_IsEnd(true)
{
  ZERO( &m_State, sizeof(m_State) );

}

void OggFile::Initialize()
{
  Finalize();

  const bool IsError = m_hFile.Open( m_FileName );
  if( IsError ) { return ; }
  m_IsEnd = false;

  ogg_sync_init(&m_State);

  //  読み取りサイズはファイルサイズの１％ぐらいにしておく
  //  最低4096byte 最高 1024*1024byte

  const size_t size = m_hFile.GetSize() / 100;

  if( size      < READSIZE_MIN ) { m_ReadSize = READSIZE_MIN; }
  else if( READSIZE_MAX < size ) { m_ReadSize = READSIZE_MAX; }
  else { m_ReadSize = size; }
}

void OggFile::Finalize()
{
  ogg_sync_clear(&m_State);
  m_IsEnd = true;
  m_hFile.Close();
}

void OggFile::Read( SPSTORAGESAMPLE& pSample )
{
  if( IsEnd() ) { return ; }

  ogg_page page;
  while( true )
  {
    /*
       return values:
       -1) recapture (hole in data)
        0) need more data
        1) page returned
    */
    const int ret = ogg_sync_pageout(&m_State,&(page));

    if( ret==1 )
    {
      break; 
    }
    else if( ret==-1 )
    {
      return;
    }
    else if( ret==0 )
    {
      //  すでに終端にいたらダメポ
      const size_t size = m_hFile.GetSize();
      const size_t pos  = m_hFile.GetPosition();
      if( size <= pos ) { m_IsEnd = true; return; }

      char* buffer = ogg_sync_buffer(&m_State,m_ReadSize);
      const int readsize = m_hFile.Read( buffer, m_ReadSize );
      ogg_sync_wrote(&m_State, readsize);
    }
  }

  pSample.reset( new OggPage(page) );
}

bool OggFile::IsEnd() const
{
  return m_IsEnd;
}




}}}



