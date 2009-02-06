#ifndef framework_xiph_oggpacket_h
#define framework_xiph_oggpacket_h


#include"../../config/define.h"

#include<ogg/ogg.h>

namespace Maid { namespace Xiph {

  class OggStream;
  class OggPacket
  {
    friend OggStream;
  public:
    OggPacket();

    const ogg_packet& Get() const;

    ogg_int64_t  GetGranulePosition() const;
    ogg_int64_t  GetPacketNo() const; 

  private:
    ogg_packet  m_Packet;
  };


}}


#endif
