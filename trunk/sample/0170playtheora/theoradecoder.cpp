#include"theoradecoder.h"

#include"../../source/auxiliary/debug/assert.h"
#include"../../source/auxiliary/debug/warning.h"

void TheoraDecoder::Initialize( const OggPacket& NewPacket )
{
  MAID_ASSERT( NewPacket.GetPacketNo()!=0, "パケット番号が合いません" );


  theora_info_init( &m_Info );
  theora_comment_init( &m_Comment );

  ogg_packet& packet = (ogg_packet&)NewPacket.Get();


  const int ret = theora_decode_header( &m_Info, &m_Comment, &packet );
  if( ret<0 )  { MAID_WARNING( "theora_decode_header" ); }
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

  if( no<3 )
  {

  }
}


