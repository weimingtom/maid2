#include"oggstream.h"
#include"../../auxiliary/macro.h"
#include"../../auxiliary/debug/warning.h"

namespace Maid { namespace Xiph {

OggStream::OggStream()
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
  ogg_stream_init( &m_Stream, serial );
}

void OggStream::Finalize()
{
  ogg_stream_clear(&m_Stream);
}


void OggStream::PageIn( const OggPage& Page )
{
  ogg_page* p = (ogg_page*)&Page.m_Page;

  ogg_stream_pagein( &m_Stream, p );
}

void OggStream::PacketOut( OggPacket& out )
{
  ogg_stream_packetout(&m_Stream, &out.m_Packet );
}


bool OggStream::IsPacketEmpty() const
{
  ogg_stream_state& os = const_cast<ogg_stream_state&>(m_Stream);

  const int ret = ogg_stream_packetpeek( &os, NULL );

  return ret==0;
}


int OggStream::GetSerialNo() const
{
  return m_Stream.serialno;
}


bool OggStream::IsEnd() const
{
  ogg_stream_state& os = const_cast<ogg_stream_state&>(m_Stream);
  const int ret = ogg_stream_eos( &os );

  return ret!=0;

}


}}