#include"oggpacket.h"
#include"../../../../auxiliary/macro.h"


namespace Maid { namespace Movie { namespace Xiph {

  OggPacket::OggPacket()
  {
    ZERO(&m_Packet,sizeof(m_Packet));
  }

  OggPacket::OggPacket( const ogg_packet& src )
  {
    m_Packet = src;
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

  long  OggPacket::GetSize() const
  {
    return m_Packet.bytes;
  }

  std::string OggPacket::GetDebugString() const
  {
    std::string Binary;
    {
      const int size = std::min(8,(int)m_Packet.bytes);
      for( int i=0; i<size; ++i )
      {
        char buf[256];
        sprintf( buf, "%02x.", m_Packet.packet[i] );
        Binary += buf;
      }
    }

    std::string Text;
    {
      const int byte = m_Packet.bytes;
      const int b_o_s = m_Packet.b_o_s;
      const int e_o_s = m_Packet.e_o_s;
      const int granulepos = m_Packet.granulepos;
      const int packetno = m_Packet.packetno;

      char buf[1024];
      sprintf( buf, "no:%06d granule:%06d bos:%03d eos:%03d bytes:%05d",
        packetno, granulepos, b_o_s, e_o_s, byte );
      Text += buf;
    }

    return Text + " packet:" + Binary;
 }


  void OggPacketEx::Copy( const OggPacket& SrcPacket )
  {
    const ogg_packet& src = SrcPacket.Get();

    m_Data = std::vector<unsigned char>( src.packet, src.packet+src.bytes );

    ogg_packet& dst = m_Packet;
    dst.packet = &(m_Data[0]);
    dst.bytes = src.bytes;
    dst.b_o_s = src.b_o_s;
    dst.e_o_s = src.e_o_s;
    dst.granulepos = src.granulepos;
    dst.packetno = src.packetno;
  }


}}}