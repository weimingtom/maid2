#include"oggpacket.h"
#include"../../auxiliary/macro.h"


namespace Maid { namespace Xiph {

  OggPacket::OggPacket()
  {
    ZERO( &m_Packet, sizeof(m_Packet) );
  }

  const ogg_packet& OggPacket::Get() const
  {
    return m_Packet;
  }

  ogg_int64_t  OggPacket::GetGranulePosition() const
  {
    return m_Packet.granulepos;
  }

  ogg_int64_t  OggPacket::GetPacketNo() const
  {
    return m_Packet.packetno;
  }

  void OggPacket::Copy( std::vector<unsigned char>& packet, const OggPacket& SrcPacket )
  {
    const ogg_packet& src = SrcPacket.Get();

    packet.resize( src.bytes );
    ::memcpy( &(packet[0]), src.packet, src.bytes );

    m_Packet.packet = &(packet[0]);
    m_Packet.bytes = src.bytes;
    m_Packet.b_o_s = src.b_o_s;
    m_Packet.e_o_s = src.e_o_s;
    m_Packet.granulepos = src.granulepos;
    m_Packet.packetno = src.packetno;
  }

}}