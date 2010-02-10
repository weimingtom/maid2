#include"oggdecodermanager.h"
#include"codectheora.h"
#include"codecvorbis.h"
#include"../decodersamplecache.h"
#include"../../../../auxiliary/debug/assert.h"

#include"oggdecodermultithread.h"


/*

  ogg の内部構造の解説

  ogg はファイルフォーマットみたいなもので RIFF だと思うとわかりやすい

  v１つあたりが１画面
  s１つあたりがサウンド単位

  v0,v1,v2,v3    s0,s1,s2,s3




  それぞれの単位に ogg packet(p) がついてる

  p+v0,p+v1,p+v2,p+v3  p+s0,p+s1,p+s2,p+s3




  packet の塊が ogg page

  page[pv0,pv1]page[pv2,pv3,,,]   page[ps0,ps1,ps2,ps3,,,]



  ogg page の塊がoggコンテナ（実ファイル）となっています
  pageの種類を区別するのにシリアルナンバーが振ってあります
  oggが勝手に割り振るので、固定してはいけません

  page,page,page,,,,



  ogg page を、パケット単位でアクセスできるようにしてるのが ogg stream

  stream.PageIn(page[pv0,pv1]);
  stream.PageIn(page[pv2,pv3]);
  stream[0] == pv0;
  stream[1] == pv1;
  stream[2] == pv2;
  stream[3] == pv3;


  そのた細かいことは
  http://d.hatena.ne.jp/Ko-Ta/searchdiary?of=7&word=*[theora]
  を参照！
*/




namespace Maid { namespace Movie { namespace Xiph {


OggDecoderManager::OggDecoderManager()
  :m_State(STATE_INITIALIZING)
{

}

FUNCTIONRESULT OggDecoderManager::Initialize()
{

  return FUNCTIONRESULT_OK;
}

bool OggDecoderManager::Setuped()const
{
  return m_State==STATE_WORKING;
}



void OggDecoderManager::GetFormat( const String& id, SPSAMPLEFORMAT& pOut )const
{
  const INFO& info = FindInfo(id);

  pOut = info.pFormat;
}


void OggDecoderManager::AddSource( const SPSTORAGESAMPLE& buffer )
{
  const OggPage& page = *boost::shared_static_cast<OggPage>(buffer);

  switch( m_State )
  {
  case STATE_INITIALIZING:
    {
      //  bos は連続していて、bosでないページが出てきたら、組み合わせが完了する
      if( page.IsBeginOfStream() ) { AddDecoder( page ); break; }
      m_State = STATE_WORKING;

    }//break; //  だからスルー

  case STATE_WORKING:
    {
      const int Serial = page.GetSerialNo();
      DECODERLIST::iterator ite = m_DecoderList.find(Serial);

      if( ite==m_DecoderList.end() ) { return ; }
      ite->second.pDecoder->AddSource( buffer );
    }break;
  }

}

void OggDecoderManager::FlushSample( const String& id, double time, DECODERSAMPLELIST& pOut )
{
  const INFO& info = FindInfo(id);

  info.pDecoder->FlushSample( time, pOut );

}

double OggDecoderManager::GetTime() const
{
  if( m_DecoderList.empty() ) { return 0.0; }

  double ret = DBL_MAX;

  for( DECODERLIST::const_iterator ite=m_DecoderList.begin(); ite!=m_DecoderList.end(); ++ite )
  {
    ret = std::min( ret, ite->second.pDecoder->GetTime() );
  }

  return ret;
}

bool OggDecoderManager::IsSourceFull() const
{
  //  デコーダがないということはまだデータが来ていないこともある。
  if( m_DecoderList.empty() ) { return false; }

  for( DECODERLIST::const_iterator ite=m_DecoderList.begin(); ite!=m_DecoderList.end(); ++ite )
  {
    if( !(ite->second.pDecoder->IsSourceFull()) ) { return false; }
  }

  return true;
}

bool OggDecoderManager::IsSampleFull() const
{
  if( m_DecoderList.empty() ) { return true; }

  for( DECODERLIST::const_iterator ite=m_DecoderList.begin(); ite!=m_DecoderList.end(); ++ite )
  {
    if( !(ite->second.pDecoder->IsSampleFull()) ) { return false; }
  }

  return true;
}

void OggDecoderManager::BeginSkipMode( double time )
{
  for( DECODERLIST::iterator ite=m_DecoderList.begin(); ite!=m_DecoderList.end(); ++ite )
  {
    ite->second.pDecoder->BeginSkipMode( time );
  }
}

bool OggDecoderManager::IsDecodeEnd() const
{
  for( DECODERLIST::const_iterator ite=m_DecoderList.begin(); ite!=m_DecoderList.end(); ++ite )
  {
    if( !(ite->second.pDecoder->IsDecodeEnd()) ) { return false; }
  }

  return true;
}


void OggDecoderManager::AddDecoder( const OggPage& page )
{
  SPOGGSTREAM pStream( new OggStream );
  pStream->Initialize( page.GetSerialNo() );
  pStream->PageIn( page );

  OggPacket packet;
  pStream->PacketOut( packet );

  const int Serial = page.GetSerialNo();
  String  DecID;
  SPDECODER pDecoder;
  SPSAMPLEFORMAT  pFormat;

  {
    if( CodecTheora::IsFirstPacket(packet) )
    {
      SPCODECTHEORA pTheora( new CodecTheora );
      pTheora->Initialize();
      pTheora->Decode( packet, SPSAMPLE() );

      const theora_info& th = pTheora->GetInfo();
      SPCACHECHECKER pChecker( new CacheCheckerTheora(5,10) );
      SPSAMPLEFORMATFRAME pInfo( new SampleFormatFrame );

      {
        pInfo->EncodedSize = SIZE2DI(th.width, th.height);
        pInfo->DisplaySize = SIZE2DI(th.frame_width,th.frame_width);
        pInfo->DisplayOffset = POINT2DI( th.offset_x, th.offset_y );

        pInfo->FpsNumerator = th.fps_numerator;
        pInfo->FpsDenominator = th.fps_denominator;
        pInfo->AspectNumerator = th.aspect_numerator;
        pInfo->AspectDenominator = th.aspect_numerator;

        switch( th.pixelformat )
        {
        case OC_PF_444: { pInfo->PixelFormat = SampleFormatFrame::PIXELFORMAT_YUV444; }break;
        case OC_PF_422: { pInfo->PixelFormat = SampleFormatFrame::PIXELFORMAT_YUV422; }break;
        case OC_PF_420: { pInfo->PixelFormat = SampleFormatFrame::PIXELFORMAT_YUV420; }break;
        default: { MAID_ASSERT( true, "不明" ); }break;
        }
      }

      DecID = DECODERID_FRAME1;
      pDecoder = SPDECODER( new OggDecoderMultiThread(pStream, pTheora, pChecker) );
      pFormat  = pInfo;
    }
    else if( CodecVorbis::IsFirstPacket(packet) )
    {
      SPCODECVORBIS pVorbis( new CodecVorbis );
      pVorbis->Initialize();
      pVorbis->Decode( packet, SPSAMPLE() );

      const PCMFORMAT& fmt = pVorbis->GetFormat();

      SPCACHECHECKER pChecker( new CacheCheckerVorbis(5,2) );
      SPSAMPLEFORMATPCM pInfo( new SampleFormatPCM );
      pInfo->Format = fmt;

      DecID = DECODERID_PCM1;
      pDecoder = SPDECODER( new OggDecoderMultiThread(pStream, pVorbis, pChecker) );
      pFormat  = pInfo;
    }
  }

  if( pDecoder.get()==NULL ) { return ; }
  pDecoder->Initialize();

  INFO& info = m_DecoderList[Serial];

  info.pDecoder = pDecoder;
  info.ID       = DecID;
  info.pFormat  = pFormat;
}

const OggDecoderManager::INFO& OggDecoderManager::FindInfo( const String& id ) const
{
  for( DECODERLIST::const_iterator ite=m_DecoderList.begin(); ite!=m_DecoderList.end(); ++ite )
  {
    if( ite->second.ID == id ) { return ite->second; }
  }
  MAID_ASSERT( true, MAIDTEXT("存在していないidは使用できません ") + id );
  static INFO ret;
  return ret;
}



}}}


