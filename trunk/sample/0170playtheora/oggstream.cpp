#include"oggstream.h"


OggStream::OggStream()
:m_IsEmpty(true)
{
  ZERO( &m_Stream, sizeof(m_Stream) );

}

OggStream::~OggStream()
{
  Finalize();
}


void OggStream::Initialize( int serial )
{
  ogg_stream_init( &m_Stream, serial );
  m_IsEmpty = true;
}

void OggStream::Finalize()
{
  ogg_stream_clear(&m_Stream);
}


void OggStream::PageIn( const OggPage& Page )
{
  ogg_page* p = (ogg_page*)&Page.m_Page;

  ogg_stream_pagein( &m_Stream, p );

  //  すぐにアクセスできるようにしておくのが紳士ってもんだ
  NextPacket();
}

const OggPacket& OggStream::GetCurrentPacket() const
{
  return m_Packet;
}

void OggStream::NextPacket()
{
  const int ret = ogg_stream_packetout(&m_Stream, &m_Packet.m_Packet );

  m_IsEmpty = !(ret>0);
}

bool OggStream::IsPacketEmpty() const
{
  return m_IsEmpty;
}


int OggStream::GetSerialNo() const
{
  return m_Stream.serialno;
}
