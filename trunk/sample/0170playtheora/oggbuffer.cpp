#include"oggbuffer.h"

#include"../../source/auxiliary/macro.h"



OggBuffer::OggBuffer()
{
  ZERO( &m_State, sizeof(m_State) );
}

OggBuffer::~OggBuffer()
{

}


void OggBuffer::Initialize()
{
  m_hFile.Open( MAIDTEXT("nc4259.ogv") );
  ogg_sync_init(&m_State);

  //  すぐにアクセスできるようにしておくのが紳士ってもんだ
  NextPage();
}

void OggBuffer::Finalize()
{
  ogg_sync_clear(&m_State);
  m_hFile.Close();
}


const OggPage& OggBuffer::GetCurrentPage()const
{
  return m_CurrentPage;
}


void OggBuffer::NextPage()
{
  while( true )
  {
    /*
       return values:
       -1) recapture (hole in data)
        0) need more data
        1) page returned
    */
    const int ret = ogg_sync_pageout(&m_State,&(m_CurrentPage.m_Page));

    if( ret > 0 ) { break; }
    else
    {
      //  すでに終端にいたらダメポ
      if( IsEnd() ) { return ; }
      UpdateBuffer();
    }
  }

}

bool OggBuffer::IsEnd() const
{
  const size_t size = m_hFile.GetSize();
  const size_t pos  = m_hFile.GetPosition();

  return size <= pos ;
}

void OggBuffer::UpdateBuffer()
{
  const long READSIZE = 4096;

  char* buffer = ogg_sync_buffer(&m_State,READSIZE);
  const size_t size = m_hFile.Read( buffer, READSIZE );

  ogg_sync_wrote(&m_State, size);

}

