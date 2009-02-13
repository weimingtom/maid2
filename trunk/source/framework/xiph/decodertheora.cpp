//#define LOCAL_PROFILE

#include"decodertheora.h"

#include"../../auxiliary/debug/assert.h"
#include"../../auxiliary/debug/warning.h"

#include"../../auxiliary/debug/profile.h"


/*
  theora の内部構造

  先頭パケット３つが theoraヘッダ。 theora_decode_init() を呼び続ける
  残りのパケットはフレームデータ 1フレーム 1パケット

  theora_decode_packetin() でパケットを送ることで、 theoraデコーダの時間が進みます。(同時にデコードも行われる）
  デコード済みのバッファを拾うには theora_decode_YUVout() でOK

*/

namespace Maid { namespace Xiph {


DecoderTheora::DecoderTheora()
:m_MaxPostProcessLevel(-1)
,m_CurrentPostProcessLevel(-1)
,m_State(STATE_EMPTY)
,m_DecodeStartTime(-1)
,m_FrameCount(0)
{
  ZERO( &m_TheoraInfo, sizeof(m_TheoraInfo) );
  ZERO( &m_TheoraComment, sizeof(m_TheoraComment) );
  ZERO( &m_TheoraState, sizeof(m_TheoraState) );
}

DecoderTheora::~DecoderTheora()
{
  Finalize();
}


void DecoderTheora::Initialize(  const OggPacket& FirstPacket )
{
  Finalize();

  theora_info_init( &m_TheoraInfo );
  theora_comment_init( &m_TheoraComment );

  m_State = STATE_INITIALIZING;
  OnSetup( FirstPacket );
}

void DecoderTheora::Finalize()
{
  if( m_State==STATE_EMPTY ) { return ; }

  theora_clear(&m_TheoraState);
  theora_comment_clear(&m_TheoraComment);
  theora_info_clear(&m_TheoraInfo);
  m_State = STATE_EMPTY;
}


const theora_info& DecoderTheora::GetInfo() const
{
  MAID_ASSERT( m_State==STATE_EMPTY, "まだ初期化されていません" );
  return m_TheoraInfo;
}

bool DecoderTheora::IsSetupped() const
{
  return m_State==STATE_DECODING;
}

void DecoderTheora::Decode( const OggPacket& NewPacket, SPBUFFER& pOut )
{
  switch( m_State )
  {
  case STATE_EMPTY:       { }break;
  case STATE_INITIALIZING:{ OnSetup ( NewPacket ); }break;
  case STATE_DECODING:    { OnDecode( NewPacket, pOut ); }break;
  }
}

double DecoderTheora::GetTime()
{
  if( m_State!=STATE_DECODING    ) { return 0; }

  const double time = theora_granule_time( &m_TheoraState, m_TheoraState.granulepos );

  return time;
}

void DecoderTheora::BeginSeekMode( double TargetTime )
{
  m_DecodeStartTime = TargetTime;
}

double DecoderTheora::CalcPacketLength( const OggPacket& NewPacket )const
{
  //  theora は１パケ１フレーム
  const double fps = double(m_TheoraInfo.fps_numerator) / double(m_TheoraInfo.fps_denominator);

  return 1.0 / fps;
}



void DecoderTheora::SetPPLevel( int level )
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


void DecoderTheora::OnSetup( const OggPacket& NewPacket )
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
    int pp;
    int ret;

    ret = theora_control(&m_TheoraState,TH_DECCTL_GET_PPLEVEL_MAX,&pp, sizeof(pp));
    if( ret<0 )  { MAID_WARNING( "theora_control" ); return; }
    m_MaxPostProcessLevel     = pp;
    m_CurrentPostProcessLevel = pp;

    #pragma COMPILERMSG("TODO:画質がたいして変わらないのに処理負荷がすごいので潰しておく")	
//    SetPPLevel( pp );
    SetPPLevel( 0 );

    m_State = STATE_DECODING;
    m_FrameCount = 0;
  }
}

void DecoderTheora::OnDecode( const OggPacket& NewPacket, SPBUFFER& pOut )
{
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  MAID_ASSERT( theora_packet_isheader(&packet)==1, "ヘッダーがきました" );

MAID_PROFILE();

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


  if( 0<= m_DecodeStartTime )
  {
    //  theora は１パケ１フレームなので、次の時間は予測できる
    //  最初のデータパケットを食べさせても時間すすまないけどね（・ω・）
    const double fps = double(m_TheoraInfo.fps_numerator) / double(m_TheoraInfo.fps_denominator);

    const double newframetime = double(NewPacket.GetPacketNo()-3) / fps;

    if( m_DecodeStartTime <= newframetime && theora_packet_iskeyframe(&packet)==1 )
    {
      m_DecodeStartTime = -1;
      SetPPLevel( m_MaxPostProcessLevel );
    }else
    {
       return ;
    }
  }

  const int ret = theora_decode_packetin(&m_TheoraState,&packet);
  if( ret!=0 ) { MAID_WARNING( "theora_decode_packetin" ); return; }

  m_FrameCount = (int)NewPacket.GetPacketNo()-3; // 0,1,2 はヘッダ
  pOut = CreateDecodedBuffer();

}

SPBUFFERIMAGE DecoderTheora::CreateDecodedBuffer() const
{
  yuv_buffer src;

  theora_state& state = const_cast<theora_state&>(m_TheoraState);

  const int ret = theora_decode_YUVout(&state, &src);
  if( ret<0 ) { MAID_WARNING( "theora_decode_YUVout" ); return SPBUFFERIMAGE(); }

  SPBUFFERIMAGE pBuffer( new BufferImage );

  {
    BufferImage& buf = *pBuffer;
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

std::string DecoderTheora::GetDebugString( const OggPacket& NewPacket )const
{
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  std::string ret;

  if( m_State==STATE_INITIALIZING )
  {
    ret = "header";
  }
  ef( m_State==STATE_DECODING )
  {
    if( theora_packet_iskeyframe(&packet)==1 )
    {
      ret = "keyframe";
    }
    else
    {
      ret = "deltaframe";
    }

    const int pos = m_TheoraState.granulepos;
    char buf[1024];
    sprintf( buf, " frame:%05d granu:%05d", m_FrameCount, pos );
    ret += buf;
  }

  return ret;
}

}}