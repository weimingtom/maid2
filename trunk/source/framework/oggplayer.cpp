#include"oggplayer.h"

#include"../auxiliary/mathematics.h"
#include"../auxiliary/debug/warning.h"
#include"xiph/decodernull.h"



//  デバッグするときに便利なオプション
//  VSはプロセスと違うスレッドのステップ実行でフリーズすることがあるんだよね。
//#define TESTMODE

using namespace Maid;


//  ここの実装はゲーム側が呼び出す関数を書いておきます
//  デコードスレッドが使う関数は 2.cpp を参照


OggPlayer::OggPlayer()
:m_State(STATE_EMPTY)
,m_TheoraSerial(DECODER_EMPTY)
,m_VorbisSerial(DECODER_EMPTY)
,m_SeekPosition(-1.0)
{

}

void OggPlayer::Initialize( const String& FileName )
{
  m_State = STATE_INITIALIZING;

  m_FileName = FileName;


#ifdef TESTMODE
  ThreadController::BRIGEDATA brige;
  brige.IsExit = false;
  brige.IsExecute = false;
  brige.IsAssert = false;
  ThreadFunction( brige );
#else
  m_Thread.SetFunc( MakeThreadObject(&OggPlayer::ThreadFunction,this) );
  m_Thread.Execute();

#endif
}

bool OggPlayer::IsInitialized() const
{
  //  デコーダーの構築も終わってないならまだ
  if( m_State!=STATE_WORKING ) { return false; }

  if( m_TheoraSerial!=DECODER_EMPTY )
  {
    BINDDATA::const_iterator ite = m_BindData.find(m_TheoraSerial);
    const DATA& dat = *(ite->second);

    if( !dat.pDecoder->IsSetupped() ) { return false; }
  }

  if( m_VorbisSerial!=DECODER_EMPTY )
  {
    BINDDATA::const_iterator ite = m_BindData.find(m_VorbisSerial);
    const DATA& dat = *(ite->second);

    if( !dat.pDecoder->IsSetupped() ) { return false; }
  }

  return true;
}

bool OggPlayer::IsSeeking() const
{
  if( IsInitialized() ) { return false; }

  return m_State==STATE_SEEKING;
}

bool OggPlayer::IsEnd() const
{
  if( !IsInitialized() ) { return false; }
  if( IsSeeking() ) { return false; }

  return m_Buffer.IsEnd();
}



void OggPlayer::GetFileInfo( FILEINFO& Info ) const
{
  MAID_ASSERT( !IsInitialized(), "まだ初期化されていません" );

  Info.IsImage = false;
  Info.IsPCM = false;

  if( m_TheoraSerial!=DECODER_EMPTY )
  {
    Info.IsImage = true;
    BINDDATA::const_iterator ite = m_BindData.find(m_TheoraSerial);
    const DATA& dat = *(ite->second);

    const theora_info& th = boost::static_pointer_cast<Xiph::DecoderTheora>(dat.pDecoder)->GetInfo();

    FILEINFO::IMAGE& img = Info.Image;
    img.EncodedSize = SIZE2DI(th.width, th.height);
    img.DisplaySize = SIZE2DI(th.frame_width,th.frame_width);
    img.DisplayOffset = POINT2DI( th.offset_x, th.offset_y );

    img.FpsNumerator = th.fps_numerator;
    img.FpsDenominator = th.fps_denominator;
    img.AspectNumerator = th.aspect_numerator;
    img.AspectDenominator = th.aspect_numerator;

    switch( th.pixelformat )
    {
    case OC_PF_444: { img.PixelFormat = FILEINFO::IMAGE::PIXELFORMAT_YUV444; }break;
    case OC_PF_422: { img.PixelFormat = FILEINFO::IMAGE::PIXELFORMAT_YUV422; }break;
    case OC_PF_420: { img.PixelFormat = FILEINFO::IMAGE::PIXELFORMAT_YUV420; }break;
    default: { MAID_ASSERT( true, "不明" ); }break;
    }
  }

  if( m_VorbisSerial!=DECODER_EMPTY )
  {
    Info.IsPCM = true;

    BINDDATA::const_iterator ite = m_BindData.find(m_VorbisSerial);
    const DATA& dat = *(ite->second);

    boost::shared_ptr<Xiph::DecoderVorbis> pVorbis = boost::static_pointer_cast<Xiph::DecoderVorbis>(dat.pDecoder);
    const vorbis_info& vi = pVorbis->GetInfo();

    FILEINFO::PCM& pcm = Info.Pcm;

    pcm.Format.SamplesPerSecond = vi.rate;
    pcm.Format.Channels = vi.channels;
    pcm.Format.BitPerSamples = 16;
  }
}



void OggPlayer::FlushImage( double time, Xiph::SPBUFFERIMAGE& pOutput )
{
  if( !IsInitialized() ) { return ; }
  if( m_TheoraSerial==DECODER_EMPTY ) { return ; }

  DATA& dat = *(m_BindData[m_TheoraSerial]);

  Xiph::SPSAMPLECACHE& pCache = dat.pCache;

  {
    Xiph::SPBUFFER pTmp; 
    pCache->Flush( time, pTmp );

    pOutput = boost::static_pointer_cast<Xiph::BufferImage>(pTmp);
  }
}

void OggPlayer::FlushPCM( double time, Xiph::SPBUFFERPCM& pOutput )
{
  if( !IsInitialized() ) { return ; }
  if( m_VorbisSerial==DECODER_EMPTY ) { return ; }

  DATA& dat = *(m_BindData[m_VorbisSerial]);

  Xiph::SPSAMPLECACHE& pCache = dat.pCache;

  {
    Xiph::SPBUFFER pTmp; 
    pCache->Flush( time, pTmp );

    pOutput = boost::static_pointer_cast<Xiph::BufferPCM>(pTmp);
  }
}

void OggPlayer::Seek( double time )
{
  m_SeekPosition = time;
  m_State = STATE_SEEKING;
  //  ここで設定したシーク状態は
  //  バッファが溜まるまで維持されます
}
