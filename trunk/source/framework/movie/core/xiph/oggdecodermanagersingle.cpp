#include"oggdecodermanagersingle.h"
#include"codectheora.h"
#include"codecvorbis.h"
#include"../decodersamplecache.h"
#include"../../../../auxiliary/debug/assert.h"

#include"oggdecoder.h"



namespace Maid { namespace Movie { namespace Xiph {


OggDecoderManagerSingle::OggDecoderManagerSingle()
  :m_State(STATE_INITIALIZING)
{

}

FUNCTIONRESULT OggDecoderManagerSingle::Initialize()
{

  return FUNCTIONRESULT_OK;
}

bool OggDecoderManagerSingle::Setuped()const
{
  return m_State==STATE_WORKING;
}



void OggDecoderManagerSingle::GetFormat( const String& id, SPSAMPLEFORMAT& pOut )const
{
  const INFO& info = FindInfo(id);

  pOut = info.pFormat;
}


void OggDecoderManagerSingle::AddSource( const SPSTORAGESAMPLE& buffer )
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
      {
        ThreadMutexLocker lock(ite->second.DecodeMutex);
        ite->second.pDecoder->AddSource( buffer );
      }
    }break;
  }

}

void OggDecoderManagerSingle::FlushSample( const String& id, double time, DECODERSAMPLELIST& pOut )
{
  INFO& info = FindInfo(id);

  {
    ThreadMutexLocker lock(info.CacheMutex);

    info.Cache.Pop( time, pOut );
  }

}

double OggDecoderManagerSingle::GetTime() const
{
  if( m_DecoderList.empty() ) { return 0.0; }

  double ret = DBL_MAX;

  for( DECODERLIST::const_iterator ite=m_DecoderList.begin(); ite!=m_DecoderList.end(); ++ite )
  {
    ret = std::min( ret, ite->second.pDecoder->GetTime() );
  }

  return ret;
}

bool OggDecoderManagerSingle::IsSourceFull() const
{
  //  デコーダがないということはまだデータが来ていないこともある。
  if( m_DecoderList.empty() ) { return false; }

  for( DECODERLIST::const_iterator ite=m_DecoderList.begin(); ite!=m_DecoderList.end(); ++ite )
  {
    const INFO& info = ite->second;
    {
      ThreadMutexLocker lock(((INFO&)info).DecodeMutex);

      const bool full = info.pChecker->IsSourceFull( *info.pDecoder );
      if( !full ) { return false; }
    }
  }

  return true;
}

bool OggDecoderManagerSingle::IsSampleFull() const
{
  if( m_DecoderList.empty() ) { return true; }

  for( DECODERLIST::const_iterator ite=m_DecoderList.begin(); ite!=m_DecoderList.end(); ++ite )
  {
    const INFO& info = ite->second;

    {
      ThreadMutexLocker lock(const_cast<INFO&>(info).CacheMutex);
      const bool full = info.pChecker->IsSampleFull( info.Cache );
      if( !full ) { return false; }
    }
  }

  return true;
}

void OggDecoderManagerSingle::BeginSkipMode( double time )
{
  for( DECODERLIST::iterator ite=m_DecoderList.begin(); ite!=m_DecoderList.end(); ++ite )
  {
    ite->second.pDecoder->BeginSkipMode( time );
  }
}

bool OggDecoderManagerSingle::IsDecodeEnd() const
{
  for( DECODERLIST::const_iterator ite=m_DecoderList.begin(); ite!=m_DecoderList.end(); ++ite )
  {
    const INFO& info = ite->second;

    {
      ThreadMutexLocker lock(const_cast<INFO&>(info).DecodeMutex);
      if( !(info.pDecoder->IsDecodeEnd()) ) { return false; }
    }
    {
      ThreadMutexLocker lock(const_cast<INFO&>(info).CacheMutex);
      if( 0<info.Cache.GetSize() ) { return false; }
    }
  }

  return true;
}

void OggDecoderManagerSingle::Update()
{
  for( DECODERLIST::iterator ite=m_DecoderList.begin(); ite!=m_DecoderList.end(); ++ite )
  {
    INFO& info = ite->second;

    if( IsCacheFull(info) ) { continue; }
    DECODERSAMPLE sample;
    {
      ThreadMutexLocker lock(info.DecodeMutex);
      info.pDecoder->UpdateTime( sample );
    }
    if( sample.pSample.get()!=NULL )
    {
      ThreadMutexLocker lock(info.CacheMutex);
      info.Cache.PushBack(sample);
    }
  }
}

enum THREADMASK
{
  THREADMASK_THEORA = 1<<0,
  THREADMASK_VORBIS = 1<<1,
};

void OggDecoderManagerSingle::AddDecoder( const OggPage& page )
{
  SPOGGSTREAM pStream( new OggStream );
  pStream->Initialize( page.GetSerialNo() );
  pStream->PageIn( page );

  OggPacket packet;
  pStream->PacketOut( packet );

  const int Serial = page.GetSerialNo();
  String  DecID;
  SPOGGDECODER pDecoder;
  SPSAMPLEFORMAT  pFormat;
  SPCACHECHECKER  pChecker;

  {
    if( CodecTheora::IsFirstPacket(packet) )
    {
      SPCODECTHEORA pTheora( new CodecTheora );
      pTheora->Initialize();
      pTheora->Decode( packet, SPSAMPLE() );

      const theora_info& th = pTheora->GetInfo();
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
      pDecoder = SPOGGDECODER( new OggDecoder(pStream, pTheora) );
      pChecker = SPCACHECHECKER( new CacheCheckerTheora(5,10) );
      pFormat  = pInfo;
    }
    else if( CodecVorbis::IsFirstPacket(packet) )
    {
      SPCODECVORBIS pVorbis( new CodecVorbis );
      pVorbis->Initialize();
      pVorbis->Decode( packet, SPSAMPLE() );

      const PCMFORMAT& fmt = pVorbis->GetFormat();

      SPSAMPLEFORMATPCM pInfo( new SampleFormatPCM );
      pInfo->Format = fmt;

      DecID = DECODERID_PCM1;
      pDecoder = SPOGGDECODER( new OggDecoder(pStream, pVorbis) );
      pChecker = SPCACHECHECKER( new CacheCheckerVorbis(5,2) );
      pFormat  = pInfo;
    }
  }

  if( pDecoder.get()==NULL ) { return ; }
  pDecoder->Initialize();

  INFO& info = m_DecoderList[Serial];

  info.pDecoder = pDecoder;
  info.ID       = DecID;
  info.pFormat  = pFormat;
  info.pChecker  = pChecker;
}

const OggDecoderManagerSingle::INFO& OggDecoderManagerSingle::FindInfo( const String& id ) const
{
  for( DECODERLIST::const_iterator ite=m_DecoderList.begin(); ite!=m_DecoderList.end(); ++ite )
  {
    if( ite->second.ID == id ) { return ite->second; }
  }
  MAID_ASSERT( true, MAIDTEXT("存在していないidは使用できません ") + id );
  static INFO ret;
  return ret;
}


OggDecoderManagerSingle::INFO& OggDecoderManagerSingle::FindInfo( const String& id )
{
  for( DECODERLIST::iterator ite=m_DecoderList.begin(); ite!=m_DecoderList.end(); ++ite )
  {
    if( ite->second.ID == id ) { return ite->second; }
  }
  MAID_ASSERT( true, MAIDTEXT("存在していないidは使用できません ") + id );
  static INFO ret;
  return ret;
}

bool OggDecoderManagerSingle::IsSourceFull( const INFO& info ) const
{
  ThreadMutexLocker lock(const_cast<INFO&>(info).DecodeMutex);
  return info.pChecker->IsSourceFull( *info.pDecoder );
}

bool OggDecoderManagerSingle::IsCacheFull( const INFO& info ) const
{
  ThreadMutexLocker lock(const_cast<INFO&>(info).CacheMutex);
  return info.pChecker->IsSampleFull( info.Cache );
}



}}}


