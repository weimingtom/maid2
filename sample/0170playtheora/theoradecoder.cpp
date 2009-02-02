#include"theoradecoder.h"

#include"../../source/auxiliary/debug/assert.h"
#include"../../source/auxiliary/debug/warning.h"

void TheoraDecoder::Initialize()
{
  theora_info_init( &m_Info );
  theora_comment_init( &m_Comment );
}

void TheoraDecoder::Finalize()
{
  theora_clear(&m_State);
  theora_comment_clear(&m_Comment);
  theora_info_clear(&m_Info);
}

void TheoraDecoder::Update( const OggPacket& NewPacket )
{
  const ogg_int64_t no = NewPacket.GetPacketNo();
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  if( no<3 )
  {
    const int ret = theora_decode_header(&m_Info,&m_Comment,&packet);
    if( ret<0 )  { MAID_WARNING( "theora_decode_header" ); }
  }
}


