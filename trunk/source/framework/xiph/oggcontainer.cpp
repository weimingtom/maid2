//#define LOCAL_PROFILE

#include"oggcontainer.h"

#include"../../auxiliary/macro.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/profile.h"


namespace Maid { namespace Xiph {


const long READSIZE_MIN = 4096;
const long READSIZE_MAX = 1024*1024;


OggContainer::OggContainer()
  :m_IsEnd(false)
  ,m_ReadSize(4096)
{
  ZERO( &m_State, sizeof(m_State) );
}

OggContainer::~OggContainer()
{

}


void OggContainer::Initialize( const String& FileName )
{
  Finalize();

  m_hFile.Open( FileName );
  ogg_sync_init(&m_State);

  //  読み取りサイズはファイルサイズの１％ぐらいにしておく
  //  最低4096byte 最高 1024*1024byte

  const size_t size = m_hFile.GetSize() / 100;

  if( size      < READSIZE_MIN ) { m_ReadSize = READSIZE_MIN; }
  ef( READSIZE_MAX < size ) { m_ReadSize = READSIZE_MAX; }
  else { m_ReadSize = size; }


  NextPage();
}

void OggContainer::Finalize()
{
  ogg_sync_clear(&m_State);
  m_hFile.Close();
  m_IsEnd = false;
}

const OggPage& OggContainer::GetCurrentPage() const
{
  return m_CurrentPage;

}

void OggContainer::NextPage()
{
  if( IsEnd() ) { return ; }
MAID_PROFILE();

  while( true )
  {
    /*
       return values:
       -1) recapture (hole in data)
        0) need more data
        1) page returned
    */
    const int ret = ogg_sync_pageout(&m_State,&(m_CurrentPage.m_Page));

    if( ret==1 ) { break; }
    ef( ret==-1 )
    {
    }
    ef( ret==0 )
    {
      //  すでに終端にいたらダメポ
      {
        const size_t size = m_hFile.GetSize();
        const size_t pos  = m_hFile.GetPosition();

        if( size <= pos ) { m_IsEnd = true; return ; }
      }

      char* buffer = ogg_sync_buffer(&m_State,m_ReadSize);
      const size_t size = m_hFile.Read( buffer, m_ReadSize );

      ogg_sync_wrote(&m_State, size);
    }
  }
}

bool OggContainer::IsEnd() const
{
  return m_IsEnd;
}


}}
