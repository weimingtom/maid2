#ifndef oggpacket_h
#define oggpacket_h


#include"../../source/config/define.h"

#include<ogg/ogg.h>


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


#endif
