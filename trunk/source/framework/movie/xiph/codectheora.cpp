//#define LOCAL_PROFILE

#include"codectheora.h"

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


CodecTheora::CodecTheora()
:m_MaxPostProcessLevel(-1)
,m_CurrentPostProcessLevel(-1)
,m_State(STATE_EMPTY)
,m_DecodeStartTime(-1)
,m_FrameCount(0)
,m_IsSkipMode(false)
{
  ZERO( &m_TheoraInfo, sizeof(m_TheoraInfo) );
  ZERO( &m_TheoraComment, sizeof(m_TheoraComment) );
  ZERO( &m_TheoraState, sizeof(m_TheoraState) );
}

CodecTheora::~CodecTheora()
{
  Finalize();
}


void CodecTheora::Initialize()
{
  Finalize();

  theora_info_init( &m_TheoraInfo );
  theora_comment_init( &m_TheoraComment );

  m_State = STATE_INITIALIZING;
}

void CodecTheora::Finalize()
{
  if( m_State==STATE_EMPTY ) { return ; }

  theora_clear(&m_TheoraState);
  theora_comment_clear(&m_TheoraComment);
  theora_info_clear(&m_TheoraInfo);
  m_State = STATE_EMPTY;
}


const theora_info& CodecTheora::GetInfo() const
{
  MAID_ASSERT( m_State==STATE_EMPTY, "まだ初期化されていません" );
  return m_TheoraInfo;
}

bool CodecTheora::IsSetupped() const
{
  return m_State==STATE_DECODING;
}


void CodecTheora::Setup( const OggPacket& NewPacket )
{
  MAID_ASSERT( m_State!=STATE_INITIALIZING, "初期化中のみ受け付けます" );

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

    SetPPLevel( pp );

    m_State = STATE_DECODING;
    m_FrameCount = 0;
  }
}


void CodecTheora::Decode( const OggPacket& NewPacket, SPSAMPLE& pOut )
{
  MAID_ASSERT( m_State!=STATE_DECODING, "デコード中のみ受け付けます" );

  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  MAID_ASSERT( theora_packet_isheader(&packet)==1, "ヘッダーがきました" );


  if( m_IsSkipMode )
  {
    //  スキップモードだった場合、次のキーフレームまではデコードをしない。
    if( theora_packet_iskeyframe(&packet)!=1 )
    { //  フレームの更新はしておく
      m_FrameCount = CalcPacketFrame(NewPacket);
      return ; 
    }

    m_IsSkipMode = false; //  終了！
  }


  const int ret = theora_decode_packetin(&m_TheoraState,&packet);
  if( ret!=0 ) { MAID_WARNING( "theora_decode_packetin" ); return; }

  {
    yuv_buffer src;

    const int ret = theora_decode_YUVout(&m_TheoraState, &src);
    if( ret<0 ) { MAID_WARNING( "theora_decode_YUVout" ); return ; }

    SPSAMPLEIMAGE pBuffer( new SampleImage );

    {
      SampleImage& buf = *pBuffer;
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

    pOut = pBuffer;
  }
  m_FrameCount = CalcPacketFrame(NewPacket);
}

void CodecTheora::Skip( const OggPacket& NewPacket )
{
  //  スキップするにしても、時間の設定はやっておく
  //  フレーム計算はパケット番号からやってるので、つかわないと思うけどｗ
  ogg_int64_t pos = NewPacket.GetGranulePosition();
  if( pos >= 0 )
  {
    theora_control(&m_TheoraState, TH_DECCTL_SET_GRANPOS, &pos, sizeof(pos));
  }

  m_FrameCount = CalcPacketFrame(NewPacket);
  m_IsSkipMode = true;  //  スキップモードになった。
}


double CodecTheora::GetTime()
{
  if( m_State!=STATE_DECODING    ) { return 0; }

  const double fps   = GetFPS();
  const double frame = double(m_FrameCount);

//  const double time = theora_granule_time( &m_TheoraState, m_TheoraState.granulepos );

  return frame * fps;
}



double CodecTheora::CalcPacketLength( const OggPacket& NewPacket )const
{
  //  theora は１パケ１フレーム
  return GetFPS() * 1.0;
}

int CodecTheora::CalcPacketFrame( const OggPacket& NewPacket )const
{
  return NewPacket.GetPacketNo() - 3; // 0,1,2 はヘッダ
}


double CodecTheora::GetFPS() const
{
  const double fps = double(m_TheoraInfo.fps_numerator) / double(m_TheoraInfo.fps_denominator);

  return 1.0 / fps;
}


void CodecTheora::SetPPLevel( int level )
{
  if( m_MaxPostProcessLevel < level )
  {
    level = m_MaxPostProcessLevel;
  }

  #pragma COMPILERMSG("画質がたいして変わらないのに処理負荷がすごいので潰しておく")	
  level = 0;


  int ret;
  ret = theora_control(&m_TheoraState,TH_DECCTL_SET_PPLEVEL,&level, sizeof(level));
  if( ret<0 )  { MAID_WARNING( "theora_control" ); return; }

  m_CurrentPostProcessLevel = level;
}



std::string CodecTheora::GetDebugString( const OggPacket& NewPacket )const
{
  ogg_packet& packet = (ogg_packet&)NewPacket.Get();

  std::string ret;

  if( m_State==STATE_INITIALIZING )
  {
    ret = "header";
  }
  else if( m_State==STATE_DECODING )
  {
    if( theora_packet_iskeyframe(&packet)==1 )
    {
      ret = "keyframe";
    }
    else
    {
      ret = "deltaframe";
    }

    const int pos = (int)m_TheoraState.granulepos;
    char buf[1024];
    sprintf( buf, " frame:%05d granu:%05d", m_FrameCount, pos );
    ret += buf;
  }

  return ret;
}

}}