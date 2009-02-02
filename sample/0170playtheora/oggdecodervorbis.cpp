#include"oggdecodervorbis.h"
#include"../../source/auxiliary/debug/assert.h"
#include"../../source/auxiliary/debug/warning.h"

OggDecoderVorbis::OggDecoderVorbis()
{
  ZERO( &m_Info, sizeof(m_Info) );
  ZERO( &m_Comment, sizeof(m_Comment) );
  ZERO( &m_State, sizeof(m_State) );
  ZERO( &m_Block, sizeof(m_Block) );
}

void OggDecoderVorbis::Initialize()
{
  vorbis_info_init( &m_Info );
  vorbis_comment_init( &m_Comment );
}


void OggDecoderVorbis::Finalize()
{
  vorbis_block_clear(&m_Block);
  vorbis_dsp_clear(&m_State);
  vorbis_comment_clear(&m_Comment);
  vorbis_info_clear(&m_Info);
}

void OggDecoderVorbis::Update( const OggPacket& NewPacket )
{
  const ogg_int64_t no = NewPacket.GetPacketNo();
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  if( no<3 )
  {
    const int ret = vorbis_synthesis_headerin( &m_Info, &m_Comment, &packet );
    if( ret<0 )  { MAID_WARNING( "vorbis_synthesis_headerin" ); }

  }
}
