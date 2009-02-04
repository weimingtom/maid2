#include"oggdecodertheora.h"

#include"../../source/auxiliary/debug/assert.h"
#include"../../source/auxiliary/debug/warning.h"


#include"../../source/graphics/imagefile/bitmap.h"
#include"../../source/storage/fileio/filewrite.h"

using namespace Maid;


/*
  theora の内部構造

  先頭パケット３つが theoraヘッダ。 theora_decode_init() を呼び続ける
  残りのパケットはフレームデータ 1フレーム 1パケット

  theora_decode_packetin() でパケットを送ることで、 theoraデコーダの時間が進みます。(同時にデコードも行われる）
  デコード済みのバッファを拾うには theora_decode_YUVout() でOK


  こんな仕様なのでスキップ、シーク時などデコード処理を省きたいときにも
  theora_decode_packetin() を呼ぶ必要があるのですが、pp level を下げることで、負荷を低減できるはず

  int level = 0;
  theora_control(&m_TheoraState,TH_DECCTL_SET_PPLEVEL,&level,sizeof(level));

  こんな感じ

*/



OggDecoderTheora::OggDecoderTheora()
:m_MaxPostProcessLevel(-1)
,m_CurrentPostProcessLevel(-1)
,m_State(STATE_EMPTY)
{
  ZERO( &m_TheoraInfo, sizeof(m_TheoraInfo) );
  ZERO( &m_TheoraComment, sizeof(m_TheoraComment) );
  ZERO( &m_TheoraState, sizeof(m_TheoraState) );
}


void OggDecoderTheora::Initialize()
{
  theora_info_init( &m_TheoraInfo );
  theora_comment_init( &m_TheoraComment );

  m_State = STATE_INITIALIZING;
}

void OggDecoderTheora::Finalize()
{
  theora_clear(&m_TheoraState);
  theora_comment_clear(&m_TheoraComment);
  theora_info_clear(&m_TheoraInfo);
  m_State = STATE_EMPTY;
}

bool OggDecoderTheora::IsSetupped() const
{
  return m_State==STATE_BEGINDECODE
    ||   m_State==STATE_DECODING
    ;
}

const theora_info& OggDecoderTheora::GetInfo() const
{
  MAID_ASSERT( !IsSetupped(), "まだ初期化されていません" );
  return m_TheoraInfo;
}


void OggDecoderTheora::PacketIn( const OggPacket& NewPacket )
{
  switch( m_State )
  {
  case STATE_EMPTY:       { }break;
  case STATE_INITIALIZING:{ OnSetup ( NewPacket ); }break;
  case STATE_BEGINDECODE: { OnDecode( NewPacket ); m_State = STATE_DECODING; }break;
  case STATE_DECODING:    { OnDecode( NewPacket ); }break;
  }
}

double OggDecoderTheora::GetTime()
{
  if( m_State!=STATE_DECODING    ) { return 0; }

  const double time = theora_granule_time( &m_TheoraState, m_TheoraState.granulepos );
//  const int frame = theora_granule_frame( &m_TheoraState, m_TheoraState.granulepos );
  return time;
}

void OggDecoderTheora::BeginSeekMode()
{
  if( !IsSetupped() ) { return ; }
  SetPPLevel( 0 );
}

void OggDecoderTheora::EndSeekMode()
{
  if( !IsSetupped() ) { return ; }
  SetPPLevel( m_MaxPostProcessLevel );
}

bool OggDecoderTheora::IsDecoding() const
{
  return m_State==STATE_DECODING;
}

SPOGGDECODEDBUFFER OggDecoderTheora::CreateDecodedBuffer() const
{
  MAID_ASSERT( !IsDecoding(), "まだ準備できていません" );

  yuv_buffer src;

  theora_state& state = const_cast<theora_state&>(m_TheoraState);

  const int ret = theora_decode_YUVout(&state, &src);
  if( ret!=0 ) { MAID_WARNING( "theora_decode_YUVout" ); return SPOGGDECODEDBUFFER(); }

  boost::shared_ptr<OggDecodedBufferImage> pBuffer( new OggDecodedBufferImage );

  {
    OggDecodedBufferImage& buf = *pBuffer;
    buf.SizeY  = SIZE2DI( src.y_width, src.y_height );
    buf.PitchY = src.y_stride;
    buf.BitsY.resize( src.y_stride * src.y_height );
    ::memcpy( &(buf.BitsY[0]), src.y, buf.BitsY.size() );

    buf.SizeUV  = SIZE2DI( src.uv_width, src.uv_height );
    buf.PitchUV = src.uv_stride;
    buf.BitsU.resize( src.uv_stride * src.uv_height );
    ::memcpy( &(buf.BitsU[0]), src.u, buf.BitsU.size() );
    buf.BitsV.resize( src.uv_stride * src.uv_height );
    ::memcpy( &(buf.BitsV[0]), src.v, buf.BitsV.size() );
  }

  return pBuffer;
}

void OggDecoderTheora::SetPPLevel( int level )
{
  if( m_MaxPostProcessLevel < level )
  {
    level = m_MaxPostProcessLevel;
  }

  int ret;
  ret = theora_control(&m_TheoraState,TH_DECCTL_SET_PPLEVEL,&level, sizeof(level));
  if( ret<0 )  { MAID_WARNING( "theora_control" ); return; }

  m_CurrentPostProcessLevel = level;
}


void OggDecoderTheora::OnSetup( const OggPacket& NewPacket )
{
  const ogg_int64_t no = NewPacket.GetPacketNo();
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  const int ret = theora_decode_header(&m_TheoraInfo,&m_TheoraComment,&packet);
  if( ret<0 )  { MAID_WARNING( "theora_decode_header" ); return; }

  //  ヘッダパケットを全部処理したら初期化しておく
  if( no==2 )
  {
    theora_decode_init(&m_TheoraState,&m_TheoraInfo);

    //  ポストプロセスレベルを最大にしておく
    //  ポストプロセスレベルってデコードの精度ってことでいいのかな？
    int pp;
    int ret;

    ret = theora_control(&m_TheoraState,TH_DECCTL_GET_PPLEVEL_MAX,&pp, sizeof(pp));
    if( ret<0 )  { MAID_WARNING( "theora_control" ); return; }
    m_MaxPostProcessLevel     = pp;
    m_CurrentPostProcessLevel = pp;
    ret = theora_control(&m_TheoraState,TH_DECCTL_SET_PPLEVEL,&pp,sizeof(pp));
    if( ret<0 )  { MAID_WARNING( "theora_control" ); return; }

    m_State = STATE_BEGINDECODE;
  }

}

void OggDecoderTheora::OnDecode( const OggPacket& NewPacket )
{
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  {
    //  ヘッダーは受け付けない
    const int ret = theora_decode_header(&m_TheoraInfo,&m_TheoraComment,&packet);
    if( ret<0 )  { return; }
  }

//  if( theora_packet_isheader(&packet)==1 ) { return ; }

  {
    /*HACK: This should be set after a seek or a gap, but we might not have
       a granulepos for the first packet (we only have them for the last
       packet on a page), so we just set it as often as we get it.
      To do this right, we should back-track from the last packet on the
       page and compute the correct granulepos for the first packet after
       a seek or a gap.*/
    //  訳がよくわからんけどパケット間のギャップを埋めるためのものかな？
    ogg_int64_t pos = NewPacket.GetGranulePosition();
    if( pos >= 0 )
    {
      theora_control(&m_TheoraState, TH_DECCTL_SET_GRANPOS, &pos, sizeof(pos));
    }
  }

  const int ret = theora_decode_packetin(&m_TheoraState,&packet);
  if( ret!=0 ) { MAID_WARNING( "theora_decode_packetin" ); }
}
