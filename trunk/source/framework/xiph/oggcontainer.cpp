#include"oggcontainer.h"

#include"../../auxiliary/macro.h"


namespace Maid { namespace Xiph {


OggContainer::OggContainer()
{
  ZERO( &m_State, sizeof(m_State) );
}

OggContainer::~OggContainer()
{

}


void OggContainer::Initialize( const String& FileName )
{
  m_hFile.Open( FileName );
  ogg_sync_init(&m_State);

  //  すぐにアクセスできるようにしておくのが紳士ってもんだ
  NextPage();
}

void OggContainer::Finalize()
{
  ogg_sync_clear(&m_State);
  m_hFile.Close();
}


const OggPage& OggContainer::GetCurrentPage()const
{
  return m_CurrentPage;
}


void OggContainer::NextPage()
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

    if( ret==1 ) { break; }
    ef( ret==0 )
    {
      //  すでに終端にいたらダメポ
      if( IsEnd() ) { return ; }
      UpdateBuffer();
    }
  }

}

bool OggContainer::IsEnd() const
{
  const size_t size = m_hFile.GetSize();
  const size_t pos  = m_hFile.GetPosition();

  return size <= pos ;
}

void OggContainer::UpdateBuffer()
{
  const long READSIZE = 4096;

  char* buffer = ogg_sync_buffer(&m_State,READSIZE);
  const size_t size = m_hFile.Read( buffer, READSIZE );

  ogg_sync_wrote(&m_State, size);

}


}}
