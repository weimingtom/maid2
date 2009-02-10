#include"oggplayer.h"

#include"../auxiliary/mathematics.h"
#include"../auxiliary/debug/warning.h"
//#include"xiph/decodernull.h"


#include"oggplayerlocal.h"

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



using namespace Maid;

//  ここの実装はデコードスレッドが使う関数を書いておきます
//  ゲーム側が使う関数は 1.cpp を参照

  class SampleCacheCheckerTheora
    :public ISampleCacheChecker
  {
  public:
    virtual bool IsFull( const Xiph::SampleCache& cache ) const
    {
      const size_t count = cache.GetSize();
      return FRAMECOUNT <= count;
    }
  };


  class SampleCacheCheckerVorbis
    :public ISampleCacheChecker
  {
  public:
    virtual bool IsFull( const Xiph::SampleCache& cache ) const
    {
      const double time = cache.GetTotalTime();
      return SAOUNDTIME <= time;
   }
  };




//  シーク命令が出ているか？
bool OggPlayer::IsSetSeek()const
{
  return m_SeekPosition>=0.0;
}




unt OggPlayer::ThreadFunction( volatile ThreadController::BRIGEDATA& brige )
{
  InitializeStream( m_FileName );

  while( true )
  {
    const double pos = std::max(0.0,m_SeekPosition);
    m_SeekPosition = -1.0f;
    PlayDecode( pos, brige );

    //  関数を出たということは、スレッドの終了か、シーク命令がでたことになる
    if( brige.IsExit ) { break; }

    //  スレッド終了じゃないので、シーク用に再構築する

    FinalizeStream();
    InitializeStream( m_FileName );
  }

  m_State = STATE_FINALIZING;
  FinalizeStream();
  m_State = STATE_EMPTY;

  return 0;
}




void OggPlayer::InitializeStream( const String& FileName )
{
  m_Buffer.Initialize( FileName );


  //  oggコンテナになにがあるか(theora,vorbis等) を調べる
  //  theora,vorbis 以外はNULLにしちゃう
  //  ogg の構造上、各フォーマットデータは先頭にある。
  while( true )
  {
    if( m_Buffer.IsEnd() ) { break; }

    const Xiph::OggPage& page = m_Buffer.GetCurrentPage();

    if( !page.IsBeginOfStream() ) { break; }

    {
      const int serial = page.GetSerialNo();
      Xiph::SPOGGSTREAM pStream( new Xiph::OggStream );
      Xiph::SPDECODER   pDecoder;
      SPSAMPLECACHECHECKER pChecker;

      pStream->Initialize( serial );
      pStream->PageIn( page );

      Xiph::OggPacket packet;
      
      pStream->PacketOut( packet );

      {
        ogg_packet& p = const_cast<ogg_packet&>(packet.Get());
        if( theora_packet_isheader(&p)==1 ) 
        {
          boost::shared_ptr<Xiph::DecoderTheora> pTheora( new Xiph::DecoderTheora );
          pTheora->Initialize(packet);

          FILEINFO& Info = m_FileInfo;

          Info.IsImage = true;
          const theora_info& th = pTheora->GetInfo();

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

          m_TheoraSerial = serial;

          pDecoder = pTheora;
          pChecker.reset( new SampleCacheCheckerTheora );
        }
        ef( vorbis_synthesis_idheader(&p)==1 )
        {
          boost::shared_ptr<Xiph::DecoderVorbis> pVorbis( new Xiph::DecoderVorbis );
          pVorbis->Initialize(packet);

          FILEINFO& Info = m_FileInfo;
          Info.IsPCM = true;

          const vorbis_info& vi = pVorbis->GetInfo();

          FILEINFO::PCM& pcm = Info.Pcm;

          pcm.Format.SamplesPerSecond = vi.rate;
          pcm.Format.Channels = vi.channels;
          pcm.Format.BitPerSamples = 16;

          m_VorbisSerial = serial;
          pDecoder = pVorbis;

          pChecker.reset( new SampleCacheCheckerVorbis );
        }
      }

      if( pDecoder.get()!=NULL )
      {
        //  最初のページは１パケットしかないのでここでループする必要はない

        SPOGGDECODER pOggDecoder( new OggDecoder );
        pOggDecoder->Initialize(pStream,pDecoder,pChecker);
        m_BindData[serial] = pOggDecoder;
      }
    }

    m_Buffer.NextPage();
  }
}

void OggPlayer::PlayDecode( double StartTime, volatile Maid::ThreadController::BRIGEDATA& brige )
{
  //  ０より大きいときは指定箇所まで飛ぶ必要があるので全部シークモードにする
  //  OggPage の GranulePosition をみて飛んでいく方法もあるけど
  //  なんかめんどそうなのでしない。　改善案ですね。
/*
  if( 0<StartTime )
  {
    for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
    {
      OggDecoder& dat = *(ite->second);
      dat.BeginSeekMode( StartTime );
    }
  }
*/
  const unt timer_begin = m_Timer.Get();


  //  ではデコードの開始
  //  シークモードは各デコーダが勝手に解除します。
  bool IsLoop = true;
  while( IsLoop )
  {
    if( brige.IsExit ) { return; }
    if( IsSetSeek()  ) { break; }

    //  キャッシュが全部いっぱいになっているならお休みする
    Sleep( brige );

    for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
    {
      OggDecoder& decoder = *(ite->second);

      if( decoder.GetState()==OggDecoder::STATE_PAGEWAIT )
      {
        PageSeek(ite->first);
      }
    }

//    StreamCheck( brige );
  }
}


//  指定したシリアルのストリームを充填する
void OggPlayer::PageSeek( int Serial )
{
  while( true )
  {
    //  終端にいたら終了
    if( m_Buffer.IsEnd() ) { break; }

    const Xiph::OggPage& page = m_Buffer.GetCurrentPage();

    const int PageSerial = page.GetSerialNo();

    BINDDATA::iterator ite = m_BindData.find( PageSerial );

    if( ite!=m_BindData.end() )
    {
      OggDecoder& decoder = *(ite->second);

      decoder.PageIn( page );
    }
    m_Buffer.NextPage();

    if( PageSerial==Serial ) { break; }
  }
}

bool OggPlayer::IsCacheFull( int DecoderSerial ) const
{
  if( DecoderSerial==DECODER_EMPTY ) { return true; }

  BINDDATA::const_iterator ite = m_BindData.find(DecoderSerial);
  if( ite==m_BindData.end() ) { return true; }

  const OggDecoder& decoder  = *(ite->second);

  return decoder.GetState()==OggDecoder::STATE_CACHEFULL;
}


void OggPlayer::Sleep( volatile ThreadController::BRIGEDATA& brige )
{
  while( true )
  {
    if( brige.IsExit ) { break; }
    if( IsSetSeek() ) { break; }

    //  すべてのストリームを調べてバッファがそろっているなら、減るまで待機

    if( !IsCacheFull(m_TheoraSerial) ) { return ; }
    if( !IsCacheFull(m_VorbisSerial) ) { return ; }

    m_State = STATE_WORKING;
    ThreadController::Sleep(1);
  }
}

void OggPlayer::StreamCheck( volatile ThreadController::BRIGEDATA& brige )
{
  while( true )
  {
    if( brige.IsExit ) { break; }
    if( IsSetSeek() ) { break; }

    for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
    {
      const OggDecoder& decoder  = *(ite->second);

      if( decoder.GetState()!=OggDecoder::STATE_ENDOFSTREAM ) { return ; }
    }
    ThreadController::Sleep(1);
  }
}


void OggPlayer::FinalizeStream()
{
  m_TheoraSerial = DECODER_EMPTY;
  m_VorbisSerial = DECODER_EMPTY;
  m_BindData.clear();
  m_Buffer.Finalize();
}