#include"oggdecodervorbis.h"
#include"../../source/auxiliary/debug/assert.h"
#include"../../source/auxiliary/debug/warning.h"

OggDecoderVorbis::OggDecoderVorbis()
:m_State(STATE_EMPTY)
,m_IsSeekMode(false)
{
  ZERO( &m_VorbisInfo, sizeof(m_VorbisInfo) );
  ZERO( &m_VorbisComment, sizeof(m_VorbisComment) );
  ZERO( &m_VorbisState, sizeof(m_VorbisState) );
  ZERO( &m_VorbisBlock, sizeof(m_VorbisBlock) );
}

void OggDecoderVorbis::Initialize()
{
  vorbis_info_init( &m_VorbisInfo );
  vorbis_comment_init( &m_VorbisComment );
  m_State = STATE_INITIALIZING;
}


void OggDecoderVorbis::Finalize()
{
  vorbis_block_clear(&m_VorbisBlock);
  vorbis_dsp_clear(&m_VorbisState);
  vorbis_comment_clear(&m_VorbisComment);
  vorbis_info_clear(&m_VorbisInfo);
  m_State = STATE_EMPTY;
}

bool OggDecoderVorbis::IsSetupped() const
{
  return m_State==STATE_BEGINDECODE
    ||   m_State==STATE_DECODING
    ;
}
bool OggDecoderVorbis::IsDecoding() const
{
  return m_State==STATE_DECODING;
}



const vorbis_info& OggDecoderVorbis::GetInfo() const
{
  MAID_ASSERT( !IsSetupped(), "まだ初期化されていません" );
  return m_VorbisInfo;
}

void OggDecoderVorbis::BeginSeekMode()
{
  if( !IsSetupped() ) { return ; }
  m_IsSeekMode = true; 
}

void OggDecoderVorbis::EndSeekMode()
{
  if( !IsSetupped() ) { return ; }
  m_IsSeekMode = false; 
}


void OggDecoderVorbis::PacketIn( const OggPacket& NewPacket )
{
  switch( m_State )
  {
  case STATE_EMPTY:       { }break;
  case STATE_INITIALIZING:{ OnSetup ( NewPacket ); }break;
  case STATE_BEGINDECODE: { OnDecode( NewPacket ); m_State = STATE_DECODING; }break;
  case STATE_DECODING:    { OnDecode( NewPacket ); }break;
  }
}



double OggDecoderVorbis::GetTime()
{
  if( m_State!=STATE_DECODING ) { return 0; }

  const ogg_int64_t pos = m_VorbisState.granulepos;

  const ogg_int64_t time = 1000*pos / m_VorbisInfo.rate;

  return double(time) / 1000.0;
}

SPOGGDECODEDBUFFER OggDecoderVorbis::CreateDecodedBuffer() const
{
  MAID_ASSERT( !IsDecoding(), "まだ準備できていません" );

  vorbis_dsp_state& state = const_cast<vorbis_dsp_state&>(m_VorbisState);

  float **pcm;
  const int length = vorbis_synthesis_pcmout(&state,&pcm);

  if( length <= 0 ) { return SPOGGDECODEDBUFFER(); }

  boost::shared_ptr<OggDecodedBufferPCM> pBuffer( new OggDecodedBufferPCM );

  const int channels = m_VorbisInfo.channels;

  pBuffer->Buffer.resize( length*channels );

  int16* dst = &(pBuffer->Buffer[0]);

  int index = 0;
  for( int i=0; i<length; ++i )
  {
    for( int j=0; j<channels; ++j )
    {
      int val = int(pcm[j][i]*32767.0f + 0.5f);
      if(val> 32767){ val= 32767; }
      ef(val<-32768){ val=-32768; }
      dst[index] = val;
      ++index;
    }
  }

  vorbis_synthesis_read(&state,length);

  return pBuffer;
}

void OggDecoderVorbis::OnSetup( const OggPacket& NewPacket )
{
  const ogg_int64_t no = NewPacket.GetPacketNo();
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  const int ret = vorbis_synthesis_headerin(&m_VorbisInfo,&m_VorbisComment,&packet);
  if( ret<0 )  { MAID_WARNING( "theora_decode_header" ); return; }

  //  ヘッダパケットを全部処理したら初期化しておく
  if( no==2 )
  {
    vorbis_synthesis_init(&m_VorbisState,&m_VorbisInfo);
    vorbis_block_init(&m_VorbisState,&m_VorbisBlock);

    m_State = STATE_BEGINDECODE;
  }
}

void OggDecoderVorbis::OnDecode( const OggPacket& NewPacket )
{
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  //  シークしてるときは前回までのバッファを捨てる
  if( m_IsSeekMode )
  {
    float **pcm;
    const int length = vorbis_synthesis_pcmout(&m_VorbisState,&pcm);
    if( length>0 )
    {
      vorbis_synthesis_read(&m_VorbisState,length);
    }
  }

  const int ret = vorbis_synthesis(&m_VorbisBlock,&packet);

  if( ret==0 )
  { //  成功した！
    vorbis_synthesis_blockin(&m_VorbisState,&m_VorbisBlock);


  }
}