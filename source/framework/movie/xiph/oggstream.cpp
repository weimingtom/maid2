#include"oggstream.h"
#include"../../auxiliary/macro.h"
#include"../../auxiliary/debug/warning.h"

namespace Maid { namespace Xiph {

OggStream::OggStream()
  :m_PacketCount(0)
{
  ZERO( &m_Stream, sizeof(m_Stream) );
}

OggStream::~OggStream()
{
  Finalize();
}


void OggStream::Initialize( int serial )
{
  Finalize();
  const int ret = ogg_stream_init( &m_Stream, serial );
  if( ret<0 ) { MAID_WARNING( "ogg_stream_init" ); }
}

void OggStream::Finalize()
{
  const int ret = ogg_stream_clear(&m_Stream);
  if( ret<0 ) { MAID_WARNING( "ogg_stream_clear" ); }
}


void OggStream::PageIn( const OggPage& Page )
{
  ogg_page* p = (ogg_page*)&Page.m_Page;

  const int ret = ogg_stream_pagein( &m_Stream, p );
  if( ret<0 ) { MAID_WARNING( "ogg_stream_pagein" ); return ; }

  m_PacketCount += Page.GetPacketCount();
}

bool OggStream::PacketOut( OggPacket& out )
{
  ogg_packet p;
  const int ret = ogg_stream_packetout(&m_Stream, &p );
  const bool IsSuccess = ret>0;

  if( IsSuccess ) 
  {
    --m_PacketCount; 
    out.Set( p );
  }


  return IsSuccess;
}

bool OggStream::PacketPeek( OggPacket& out )
{
  ogg_packet p;
  const int ret = ogg_stream_packetout(&m_Stream, &p );
  const bool IsSuccess = ret>0;

  if( IsSuccess ) 
  {
    out.Set( p );
  }

  return IsSuccess;
}



int OggStream::GetSerialNo() const
{
  return m_Stream.serialno;
}

bool  OggStream::IsPacketEmpty() const
{
  return GetPacketCount()==0;
}

int  OggStream::GetPacketCount() const
{
  return m_PacketCount;
}

bool OggStream::IsEnd() const
{
  ogg_stream_state& os = const_cast<ogg_stream_state&>(m_Stream);
  const int ret = ogg_stream_eos( &os );

  return ret==1 && GetPacketCount()==0;

}

std::string OggStream::GetDebugString() const
{
  ogg_stream_state& os = const_cast<ogg_stream_state&>(m_Stream);

  char buf[1024];

  sprintf( buf, "serial:%d pageno:%d packetno:%d granulepos:%d bos:%d eod:%d",
    os.serialno, os.pageno, os.packetno, os.granulepos, os.b_o_s, os.e_o_s );


  return buf;
}


}}