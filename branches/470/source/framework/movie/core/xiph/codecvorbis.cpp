#include"codecvorbis.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"

namespace Maid { namespace Movie { namespace Xiph {

bool CodecVorbis::IsFirstPacket( const OggPacket& FirstPacket )
{
  ogg_packet& packet = (ogg_packet&)FirstPacket.Get();

  return vorbis_synthesis_idheader(&packet)==1;
}


CodecVorbis::CodecVorbis()
:m_State(STATE_EMPTY)
{
  ZERO( &m_VorbisInfo, sizeof(m_VorbisInfo) );
  ZERO( &m_VorbisComment, sizeof(m_VorbisComment) );
  ZERO( &m_VorbisState, sizeof(m_VorbisState) );
  ZERO( &m_VorbisBlock, sizeof(m_VorbisBlock) );
}

CodecVorbis::~CodecVorbis()
{
  Finalize();
}


void CodecVorbis::Initialize()
{
  Finalize();

  vorbis_info_init( &m_VorbisInfo );
  vorbis_comment_init( &m_VorbisComment );
  m_State = STATE_INITIALIZING;
}


void CodecVorbis::Finalize()
{
  if( m_State==STATE_EMPTY ) { return ; }

  vorbis_block_clear(&m_VorbisBlock);
  vorbis_dsp_clear(&m_VorbisState);
  vorbis_comment_clear(&m_VorbisComment);
  vorbis_info_clear(&m_VorbisInfo);
  m_State = STATE_EMPTY;
}

const vorbis_info& CodecVorbis::GetInfo() const
{
  MAID_ASSERT( m_State==STATE_EMPTY, "まだ初期化されていません" );
  return m_VorbisInfo;
}

PCMFORMAT CodecVorbis::GetFormat()
{
  MAID_ASSERT( m_State==STATE_EMPTY, "まだ初期化されていません" );

  PCMFORMAT fmt;

  fmt.SamplesPerSecond = m_VorbisInfo.rate;
  fmt.BitPerSamples = sizeof(int16)*8;
  fmt.Channels = m_VorbisInfo.channels;

  return fmt;
}



double CodecVorbis::CalcPacketLength( const OggPacket& NewPacket )const
{
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();
  const long sample = vorbis_packet_blocksize((vorbis_info *)&m_VorbisInfo, &packet ) /4;

  return double(sample) / double(m_VorbisInfo.rate);
}


void CodecVorbis::Decode( const OggPacket& NewPacket, SPSAMPLE& pOut )
{
  switch( m_State )
  {
  case STATE_EMPTY: { }break;
  case STATE_INITIALIZING: { Init(NewPacket); }break;
  case STATE_DECODING: { Dec(NewPacket,pOut); }break;
  }
}


void CodecVorbis::Skip( const OggPacket& NewPacket )
{
  SPSAMPLE tmp;
  Decode( NewPacket, tmp );
}
/*
void CodecVorbis::Setup( const OggPacket& NewPacket )
{
  const ogg_int64_t no = NewPacket.GetPacketNo();
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  const int ret = vorbis_synthesis_headerin(&m_VorbisInfo,&m_VorbisComment,&packet);
  if( ret<0 )  { MAID_WARNING( "vorbis_synthesis_headerin" ); return; }

  //  ヘッダパケットを全部処理したら初期化しておく
  if( no==2 )
  {
    vorbis_synthesis_init(&m_VorbisState,&m_VorbisInfo);
    vorbis_block_init(&m_VorbisState,&m_VorbisBlock);

    m_State = STATE_DECODING;
  }
}

void CodecVorbis::Decode( const OggPacket& NewPacket, SPSAMPLE& pOut )
{
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  {
    const int ret = vorbis_synthesis(&m_VorbisBlock,&packet);
    if( ret!=0 ) { return ; }
  }

  {
    const int ret = vorbis_synthesis_blockin(&m_VorbisState,&m_VorbisBlock);
  }

  float **pcm;
  const int length = vorbis_synthesis_pcmout(&m_VorbisState,&pcm);

  if( length <= 0 ) { return ; }

  SPSAMPLEPCM pBuffer( new SamplePCM );

  const int channels = m_VorbisInfo.channels;
  const size_t channellen = sizeof(float32) * length;

  pBuffer->Data.resize( channels );

  for( int i=0; i<channels; ++i )
  {
    SamplePCM::BUFFER& buf = pBuffer->Data[i];

    buf.resize(length);

    ::memcpy( &(buf[0]), pcm[i], channellen );
  }

  vorbis_synthesis_read(&m_VorbisState,length);

  pOut = pBuffer;
}

void CodecVorbis::Skip( const OggPacket& NewPacket )
{
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  {
    SPSAMPLE sample;
    Decode( NewPacket, sample );
  }
}
*/

double CodecVorbis::GetTime()
{
  if( m_State!=STATE_DECODING ) { return 0; }
  if( m_VorbisState.granulepos < 0 ) { return 0; }

  const double ret = vorbis_granule_time(&m_VorbisState, m_VorbisState.granulepos );

  return ret;
}



std::string CodecVorbis::GetDebugString( const OggPacket& NewPacket )const
{
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  std::string ret;


  if( m_State==STATE_INITIALIZING )
  {
    ret = "header";
  }
  else if( m_State==STATE_DECODING )
  {
    ret = "block";

    vorbis_dsp_state* pState = (vorbis_dsp_state*)&m_VorbisState;
    vorbis_info* pInfo = (vorbis_info*)&m_VorbisInfo;

    const double now_time = vorbis_granule_time( pState, m_VorbisState.granulepos);
    const double pack_time = vorbis_granule_time( pState, packet.granulepos);
    const int pos = m_VorbisState.granulepos;
    const int blocksize = vorbis_packet_blocksize(pInfo, &packet );
    char buf[1024];
    sprintf( buf, " pos:%f granu:%05d blocksize:%05d packettime:%f",
      now_time, pos, blocksize, pack_time );
    ret += buf;
  }

  return ret;
}



void CodecVorbis::Init( const OggPacket& NewPacket )
{
  const ogg_int64_t no = NewPacket.GetPacketNo();
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  const int ret = vorbis_synthesis_headerin(&m_VorbisInfo,&m_VorbisComment,&packet);
  if( ret<0 )  { MAID_WARNING( "vorbis_synthesis_headerin" ); return; }

  //  ヘッダパケットを全部処理したら初期化しておく
  if( no==2 )
  {
    vorbis_synthesis_init(&m_VorbisState,&m_VorbisInfo);
    vorbis_block_init(&m_VorbisState,&m_VorbisBlock);

    m_State = STATE_DECODING;
  }

}

void CodecVorbis::Dec( const OggPacket& NewPacket, SPSAMPLE& pOut )
{
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  {
    const int ret = vorbis_synthesis(&m_VorbisBlock,&packet);
    if( ret!=0 ) { return ; }
  }

  {
    const int ret = vorbis_synthesis_blockin(&m_VorbisState,&m_VorbisBlock);
  }

  float **pcm;
  const int length = vorbis_synthesis_pcmout(&m_VorbisState,&pcm);

  if( length <= 0 ) { return ; }

  SPSAMPLEPCM pBuffer( new SamplePCM );

  const int channels = m_VorbisInfo.channels;
  const size_t channellen = sizeof(float32) * length;

  pBuffer->Data.resize( channels );

  for( int i=0; i<channels; ++i )
  {
    SamplePCM::BUFFER& buf = pBuffer->Data[i];

    buf.resize(length);

    ::memcpy( &(buf[0]), pcm[i], channellen );
  }

  vorbis_synthesis_read(&m_VorbisState,length);

  pOut = pBuffer;
}


}}}
