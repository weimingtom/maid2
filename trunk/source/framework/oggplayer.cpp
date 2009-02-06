#include"oggplayer.h"

#include"../auxiliary/mathematics.h"
#include"../auxiliary/debug/warning.h"
#include"xiph/decodernull.h"

/*

  ogg の内部構造の解説

  ogg はファイルフォーマットみたいなもので RIFF だと思うとわかりやすい

  v１つあたりが１画面
  s１つあたりがサウンド単位

  v0,v1,v2,v3    s0,s1,s2,s3




  それぞれの単位に ogg packet(p) がついてる

  p+v0,p+v1,p+v2,p+v3  p+s0,p+s1,p+s2,p+s3




  packet の塊が ogg page

  page[pv0,pv1,pv2,pv3]   page[ps0,ps1,ps2,ps3]




  ogg page を、パケット単位でアクセスできるようにしてるのが ogg stream

  stream.PageIn(page[pv0,pv1,pv2,pv3]);
  stream[0] == pv0;
  stream[1] == pv1;
  stream[2] == pv2;
  stream[3] == pv3;


  そのた細かいことは
  http://d.hatena.ne.jp/Ko-Ta/searchdiary?of=7&word=*[program,theora]
  を参照！
*/


//#define TESTMODE

using namespace Maid;


static const int DECODER_EMPTY = -1;

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



#if 0
void OggPlayer::FlushImage( double time, SPOGGDECODEDBUFFERIMAGE& pOutput )
{
  if( !IsInitialized() ) { return ; }

  DATA& dat = *(m_BindData[m_TheoraSerial]);

  //  dat.DecodedInfo は時間が古い順に並んでいるので、それをtimeより新しいものを探す
  {
    OGGDECODESAMPLELIST SampleList;

    dat.pDecoder->Flush(SampleList);

    m_ImageChache.insert( m_ImageChache.end(), SampleList.begin(), SampleList.end() );

    OGGDECODESAMPLELIST& dat = m_ImageChache;
    while( true )
    {
      if( dat.empty() ) { break; }

      OGGDECODESAMPLELIST::iterator ite = dat.begin();

      if( time <= ite->BeginTime )
      {
        pOutput = boost::static_pointer_cast<OggDecodedBufferImage>(ite->pBuffer);
      }
      
      //  ついでに古いものは破棄
      dat.erase(ite);

      if( pOutput.get()!=NULL ) { break; }
    }
  }
}



void OggPlayer::FlushPCM( double time, Buffer& WaveData )
{
  if( !IsInitialized() ) { return ; }

  //  デコード後のデータが複数にあることがあるので、全部つなげる


  typedef std::list<SPOGGDECODEDBUFFERPCM> BUFFERLIST;
  BUFFERLIST bufferlist;

  double BeginTime=0;
  {
    //  とりあえずはデコードされてるやつを全部奪取
    DATA& dat = *(m_BindData[m_VorbisSerial]);

    OGGDECODESAMPLELIST SampleList;

    dat.pDecoder->Flush(SampleList);

    //  リストがないなら終了
    if( SampleList.empty() ){ return ; }

    OGGDECODESAMPLELIST& InfoList = SampleList;

    BeginTime = InfoList.begin()->BeginTime;

    for( OGGDECODESAMPLELIST::iterator ite =InfoList.begin();
                                       ite!=InfoList.end();
                                          ++ite )
    {
      //  指定した時間より、バッファが古い場合は破棄
      OGGDECODESAMPLE& sample = *ite;
      if( sample.EndTime < time ) { continue; }

      bufferlist.push_back( 
          boost::static_pointer_cast<OggDecodedBufferPCM>(ite->pBuffer)
        );
    }

    InfoList.clear();
  }


  {
    //  time がバッファの BeginTime で始まってるわけないので
    //  そのギャップを入れる
    FILEINFO info;

    GetFileInfo( info );
    const PCMFORMAT& fmt = info.Pcm.Format;
    const float sa = float(time - BeginTime);
    const size_t len = fmt.CalcLength(sa);

    if( 0<len )
    {
      SPOGGDECODEDBUFFERPCM pBuff( new OggDecodedBufferPCM );
      pBuff->WaveData.Initialize();
      pBuff->WaveData.Resize( len );

      ZERO( pBuff->WaveData.GetPointer(0), len );

      bufferlist.push_front(pBuff); //  ほほいと挿入
    }
  }
  { 
    //  続いてバッファの長さを計算する
    size_t len = 0;
    for( BUFFERLIST::iterator ite=bufferlist.begin();
                              ite!=bufferlist.end();
                                ++ite )
    {
      len += (*ite)->WaveData.GetSize();
    }

    WaveData.Initialize();
    WaveData.Resize( len );
  }


  {
    size_t pos = 0;
    for( BUFFERLIST::iterator ite=bufferlist.begin();
                              ite!=bufferlist.end();
                                ++ite )
    {
      void* pDst = WaveData.GetPointer(pos);
      const void* pSrc = (*ite)->WaveData.GetPointer(0);
      const size_t len = (*ite)->WaveData.GetSize();

      ::memcpy( pDst, pSrc, len );

      pos += len;
    }
  }

}
#endif


//  シーク命令が出ているか？
bool OggPlayer::IsSetSeek()const
{
  return m_SeekPosition>=0.0;
}


void OggPlayer::Seek( double time )
{
  m_SeekPosition = time;
  m_State = STATE_SEEKING;
  //  ここで設定したシーク状態は
  //  バッファが溜まるまで維持されます
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

    const int serial = page.GetSerialNo();

    {
      boost::shared_ptr<DATA> pData( new DATA );
      pData->Stream.Initialize( serial );
      pData->Stream.PageIn( page );

      const Xiph::OggPacket& packet = pData->Stream.GetCurrentPacket();
      ogg_packet& p = const_cast<ogg_packet&>(packet.Get());

      Xiph::SPDECODER pDec;
      Xiph::SPSAMPLECACHE pCache;

      {
        if( theora_packet_isheader(&p)==1 ) 
        {
          boost::shared_ptr<Xiph::DecoderTheora> th( new Xiph::DecoderTheora );
          boost::shared_ptr<Xiph::SampleCacheTheora> ch( new Xiph::SampleCacheTheora );

          th->Initialize(packet);

          pDec = th;
          pCache = ch;
          m_TheoraSerial = serial;
        }
        ef( vorbis_synthesis_idheader(&p)==1 )
        {
          boost::shared_ptr<Xiph::DecoderVorbis> vb( new Xiph::DecoderVorbis );
          vb->Initialize(packet);

          boost::shared_ptr<Xiph::SampleCacheVorbis> ch( new Xiph::SampleCacheVorbis(vb->GetFormat()) );

          pDec = vb;
          pCache = ch;
          m_VorbisSerial = serial;
        }
        else 
        {
          pDec.reset( new Xiph::DecoderNULL ); 
          pCache.reset( new Xiph::SampleCacheNULL ); 
        }
      }

      //  最初のページは１パケットしかないのでここでループする必要はない
      pData->pDecoder = pDec;
      pData->pCache = pCache;
      m_BindData[serial] = pData;
    }

    m_Buffer.NextPage();
  }

}

void OggPlayer::FinalizeStream()
{
  for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
  {
    DATA& dat = *(ite->second);

    dat.pDecoder->Finalize();
    dat.pCache->Clear();
    dat.Stream.Finalize();
  }

  m_TheoraSerial = DECODER_EMPTY;
  m_VorbisSerial = DECODER_EMPTY;
  m_BindData.clear();
  m_Buffer.Finalize();
}

void OggPlayer::SeekStream( double StartTime, volatile Maid::ThreadController::BRIGEDATA& brige )
{
  bool IsSeeking = true;


  while( IsSeeking )
  {
    if( brige.IsExit ) { return; }
    if( m_Buffer.IsEnd() ) { break; }

    {
      //  全デコーダが時間まで進んだら終了
      bool IsBreak = true;
      for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
      {
        DATA& dat = *(ite->second);

        const double now = dat.pDecoder->GetTime();

        if( dat.pDecoder->IsSetupped() && StartTime <= now ) { continue; }

        IsBreak = false;
        break;
      }
      if( IsBreak ) { break; }
    }


    const Xiph::OggPage& page = m_Buffer.GetCurrentPage();

    BINDDATA::iterator ite = m_BindData.find(page.GetSerialNo());

    DATA& dat = *(ite->second);

    Xiph::IDecoder&     decoder = *(dat.pDecoder);
    Xiph::OggStream&    stream  = dat.Stream;
    Xiph::ISampleCache& cache   = *(dat.pCache);

    stream.PageIn(page);

    while( true )
    {
      if( stream.IsPacketEmpty() ) { break; }
      const Xiph::OggPacket& packet = stream.GetCurrentPacket();

      Xiph::SPBUFFER pOut;
      const double begin = decoder.GetTime();

      if( !decoder.IsSetupped() )
      {
        //  ストリームの初期化中なら、パケットを送るだけ
        decoder.Decode(packet,pOut);

        //  初期化が終わったら＝＝シーク開始の合図
        if( decoder.IsSetupped() )
        {
          decoder.BeginSeekMode(StartTime);
        }
      }else
      {
        decoder.Decode(packet,pOut);
      }

      if( pOut.get()!=NULL )
      {
        const double end = decoder.GetTime();
        cache.Add( begin, end, pOut );
      }

      stream.NextPacket();
    }
    m_Buffer.NextPage();
  }

}

void OggPlayer::PlayDecode( double begintime, volatile Maid::ThreadController::BRIGEDATA& brige )
{
  //  目標時間までシークする
  {
    SeekStream( begintime, brige );
  }

  //  ではデコードの開始
  while( true )
  {
    if( brige.IsExit ) { return; }
    if( IsSetSeek()  ) { break; }

    if( m_Buffer.IsEnd() )
    { //  末端まですすんだらお休み
      ThreadController::Sleep(1);
      continue;
    }

    const Xiph::OggPage& page = m_Buffer.GetCurrentPage();

    BINDDATA::iterator ite = m_BindData.find(page.GetSerialNo());
    DATA& dat = *(ite->second);

    Xiph::IDecoder&     decoder = *(dat.pDecoder);
    Xiph::OggStream&    stream  = dat.Stream;
    Xiph::ISampleCache& cache   = *(dat.pCache);

    stream.PageIn(page);

    while( true )
    {
      if( stream.IsPacketEmpty() ) { break; }
      if( IsSetSeek()  ) { break;; }


      const Xiph::OggPacket& packet = stream.GetCurrentPacket();

      Xiph::SPBUFFER pOut;
      const double begin = decoder.GetTime();

      decoder.Decode(packet, pOut);

      if( pOut.get()!=NULL )
      {
        const double end = decoder.GetTime();
        cache.Add( begin, end, pOut );
      }

      stream.NextPacket();
    }

    Sleep( brige );

    m_Buffer.NextPage();
  }

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
    InitializeStream( String() );
  }

  m_State = STATE_FINALIZING;
  FinalizeStream();
  m_State = STATE_EMPTY;

  return 0;
}





void OggPlayer::Sleep( volatile ThreadController::BRIGEDATA& brige )
{
  while( true )
  {
    if( brige.IsExit ) { break; }
    if( IsSetSeek() ) { break; }

    //  すべてのストリームを調べてバッファがそろっているなら、減るまで待機
    bool IsBreak = false;

    for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
    {
      DATA& dat = *(ite->second);

      if( !dat.pDecoder->IsSetupped() ) { return; }
      if( !dat.pCache->IsFull() ) { return; }
    }

    m_State = STATE_WORKING;
    ThreadController::Sleep(1);
  }
}
