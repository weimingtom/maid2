#ifndef framework_xiph_oggstream_h
#define framework_xiph_oggstream_h


#include"../../config/define.h"

#include<ogg/ogg.h>

#include"oggpage.h"
#include"oggpacket.h"


namespace Maid { namespace Xiph {

  class OggStream
  {
  public:
    OggStream();
    ~OggStream();

    void Initialize( int serial );
    void Finalize();

    void PageIn( const OggPage& Page );

    void PacketOut( OggPacket& out );

    bool IsPacketEmpty() const;

    int GetSerialNo() const;

    bool IsEnd() const;

  private:
    ogg_stream_state m_Stream;
  };

}}

#endif
