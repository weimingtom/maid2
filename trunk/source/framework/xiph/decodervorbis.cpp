#include"decodervorbis.h"
#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"

namespace Maid { namespace Xiph {

#if 0
SampleCacheVorbis::SampleCacheVorbis( const PCMFORMAT& fmt )
:m_Format(fmt)
{

}

void SampleCacheVorbis::Flush( double TargetTime, SPBUFFER& pOutput )
{
  //  デコード後のデータが複数にあることがあるので、全部つなげる

  typedef std::list<SPBUFFERPCM> BUFFERLIST;
  BUFFERLIST bufferlist;

  double BeginTime=0;
  {
    ThreadMutexLocker lock(m_Mutex);
    //  リストがないなら終了
    if( m_List.empty() ){ return ; }

    SAMPLELIST& InfoList = m_List;

    BeginTime = InfoList.begin()->BeginTime;

    for( SAMPLELIST::iterator ite =InfoList.begin();
                              ite!=InfoList.end();
                                   ++ite )
    {
      //  指定した時間より、バッファが古い場合は破棄
      SAMPLE& sample = *ite;
      if( sample.EndTime < TargetTime ) { continue; }

      bufferlist.push_back( 
          boost::static_pointer_cast<BufferPCM>(ite->pBuffer)
        );
    }

    InfoList.clear();
  }

  if( bufferlist.empty() ) { return ; }

  SPBUFFERPCM pRet( new BufferPCM );




  {
    //  time がバッファの BeginTime で始まってるわけないので
    //  そのギャップを入れる
    const PCMFORMAT& fmt = m_Format;
    const float sa = float(TargetTime - BeginTime);
    const size_t len = size_t(double(fmt.SamplesPerSecond) * sa +0.5);

    if( 0<len )
    {
      SPBUFFERPCM pBuff( new BufferPCM );

      pBuf->Data.resize( fmt.Channels );

      for( int i=0; i<fmt.Channels; ++i )
      {
        BufferPCM::BUFFER& buf = pBuf->Data[i];

        buf.resize( len )
        ZERO( &(buf[0]), len );
      }
      bufferlist.push_front(pBuff); //  ほほいと挿入
    }
  }
  { 
    //  続いてバッファの全長を計算＆確保する
    const PCMFORMAT& fmt = m_Format;
    size_t total_sample = 0;
    for( BUFFERLIST::iterator ite=bufferlist.begin();
                              ite!=bufferlist.end();
                                ++ite )
    {
      total_sample += (*ite)->Data[0].size();
    }

    pRet->Data.resize(fmt.Channels);
    for( int i=0; i<fmt.Channels; ++i )
    {
      pRet->Data[i].resize( total_sample );
    }
  }


  {
    //  １チャンネルずつつなげていく

    for( int 

    for( BUFFERLIST::iterator ite=bufferlist.begin();
                              ite!=bufferlist.end();
                                ++ite )
    {
      void* pDst = pRet->WaveData.GetPointer(pos);
      const void* pSrc = (*ite)->WaveData.GetPointer(0);
      const size_t len = (*ite)->WaveData.GetSize();

      ::memcpy( pDst, pSrc, len );

      pos += len;
    }
  }


/*
  pBuffer->WaveData.Initialize();
  pBuffer->WaveData.Resize( length*channels*sizeof(int16) );

  int16* dst = (int16*)pBuffer->WaveData.GetPointer(0);

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
*/
  pOutput = pRet;
}


#endif












DecoderVorbis::DecoderVorbis()
:m_State(STATE_EMPTY)
,m_DecodeStartTime(0)
,m_DecodeSample(0)
{
  ZERO( &m_VorbisInfo, sizeof(m_VorbisInfo) );
  ZERO( &m_VorbisComment, sizeof(m_VorbisComment) );
  ZERO( &m_VorbisState, sizeof(m_VorbisState) );
  ZERO( &m_VorbisBlock, sizeof(m_VorbisBlock) );
}

DecoderVorbis::~DecoderVorbis()
{
  Finalize();
}


void DecoderVorbis::Initialize( const OggPacket& FirstPacket )
{
  Finalize();

  vorbis_info_init( &m_VorbisInfo );
  vorbis_comment_init( &m_VorbisComment );
  m_State = STATE_INITIALIZING;

  OnSetup( FirstPacket );
}


void DecoderVorbis::Finalize()
{
  if( m_State==STATE_EMPTY ) { return ; }

  vorbis_block_clear(&m_VorbisBlock);
  vorbis_dsp_clear(&m_VorbisState);
  vorbis_comment_clear(&m_VorbisComment);
  vorbis_info_clear(&m_VorbisInfo);
  m_State = STATE_EMPTY;
  m_DecodeStartTime = 0;
  m_DecodeSample = 0;
}

bool DecoderVorbis::IsSetupped() const
{
  return m_State==STATE_DECODING;
}

bool DecoderVorbis::IsInitialized() const
{
  return m_State!=STATE_EMPTY;
}


const vorbis_info& DecoderVorbis::GetInfo() const
{
  MAID_ASSERT( m_State==STATE_EMPTY, "まだ初期化されていません" );
  return m_VorbisInfo;
}

PCMFORMAT DecoderVorbis::GetFormat()
{
  MAID_ASSERT( m_State==STATE_EMPTY, "まだ初期化されていません" );

  PCMFORMAT fmt;

  fmt.SamplesPerSecond = m_VorbisInfo.rate;
  fmt.BitPerSamples = sizeof(int16)*8;
  fmt.Channels = m_VorbisInfo.channels;

  return fmt;
}

void DecoderVorbis::BeginSeekMode( double TargetTime )
{
  if( !IsSetupped() ) { return ; }

  m_DecodeStartTime = TargetTime;
  m_DecodeSample = 0;
}


void DecoderVorbis::Decode( const OggPacket& NewPacket, SPBUFFER& pOut )
{
  switch( m_State )
  {
  case STATE_EMPTY:       { }break;
  case STATE_INITIALIZING:{ OnSetup ( NewPacket ); }break;
  case STATE_DECODING:    { OnDecode( NewPacket, pOut ); }break;
  }
}



double DecoderVorbis::GetTime()
{
  if( m_State!=STATE_DECODING ) { return 0; }

  const ogg_int64_t sample = m_DecodeSample;

  const size_t tani = GetFormat().SamplesPerSecond;

  const double ret = double(sample) / double(tani);

  return ret;
}

SPBUFFERPCM DecoderVorbis::CreateDecodedBuffer()
{
  MAID_ASSERT( !IsSetupped(), "まだ準備できていません" );

  vorbis_dsp_state& state = m_VorbisState;

  float **pcm;
  const int length = vorbis_synthesis_pcmout(&state,&pcm);

  if( length <= 0 ) { return SPBUFFERPCM(); }

  SPBUFFERPCM pBuffer( new BufferPCM );

  const int channels = m_VorbisInfo.channels;
  const size_t channellen = sizeof(float32) * length;

  pBuffer->Data.resize( channels );

  for( int i=0; i<channels; ++i )
  {
    BufferPCM::BUFFER& buf = pBuffer->Data[i];

    buf.resize(length);

    ::memccpy( &(buf[0], pcm[j], channellen );
  }

  vorbis_synthesis_read(&state,length);

  m_DecodeSample += length;

  return pBuffer;
}

void DecoderVorbis::OnSetup( const OggPacket& NewPacket )
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

void DecoderVorbis::OnDecode( const OggPacket& NewPacket, SPBUFFER& pOut )
{
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  const int ret = vorbis_synthesis(&m_VorbisBlock,&packet);

  if( ret==0 )
  { //  成功した！
    vorbis_synthesis_blockin(&m_VorbisState,&m_VorbisBlock);

    const double oldtime = GetTime();
    SPBUFFERPCM pBuffer = CreateDecodedBuffer();
    const double newtime = GetTime();

  //  あまりにも古かったらダメ
    if( newtime < m_DecodeStartTime ) { return ; }

    pOut = pBuffer;
  }
}


}}
