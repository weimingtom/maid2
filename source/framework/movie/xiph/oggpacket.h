﻿#ifndef maid2_framework_xiph_oggpacket_h
#define maid2_framework_xiph_oggpacket_h


#include"../../../config/define.h"

#include<ogg/ogg.h>
#include<vector>
#include<string>

namespace Maid { namespace Xiph {

  class OggPacket
  {
  public:
    OggPacket();
    const ogg_packet& Get() const;

    ogg_int64_t  GetGranulePosition() const;
    ogg_int64_t  GetPacketNo() const; 
    long  GetSize() const; 

    std::string GetDebugString() const;

    void Set( const ogg_packet& src );

  private:
    ogg_packet  m_Packet;
  };


  class OggPacketEx
    :public OggPacket
  {
  public:
    void Copy( const OggPacket& SrcPacket );

  private:
    std::vector<unsigned char> m_Packet;
  };


}}


#endif
