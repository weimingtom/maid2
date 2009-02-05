#include"decoder.h"

#include"../../source/auxiliary/mathematics.h"
#include"../../source/auxiliary/debug/warning.h"
#include"oggdecodernull.h"

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


#define TESTMODE

using namespace Maid;


static const int DECODER_EMPTY = -1;

Decoder::Decoder()
:m_State(STATE_EMPTY)
,m_TheoraSerial(DECODER_EMPTY)
,m_VorbisSerial(DECODER_EMPTY)
,m_SeekPosition(-1.0)
{

}

void Decoder::Initialize()
{
  m_State = STATE_INITIALIZING;


#ifdef TESTMODE
  ThreadController::BRIGEDATA brige;
  brige.IsExit = false;
  brige.IsExecute = false;
  brige.IsAssert = false;
  ThreadFunction( brige );
#else
  m_Thread.SetFunc( MakeThreadObject(&Decoder::ThreadFunction,this) );
  m_Thread.Execute();

#endif
}

bool Decoder::IsInitialized() const
{
  //  デコーダーの構築も終わってないならまだ
  if( m_State!=STATE_WORKING ) { return false; }

  if( m_TheoraSerial!=DECODER_EMPTY )
  {
    BINDDATA::const_iterator ite = m_BindData.find(m_TheoraSerial);
    const DATA& dat = *(ite->second);

    boost::shared_ptr<OggDecoderTheora> pTheora = boost::static_pointer_cast<OggDecoderTheora>(dat.pDecoder);

    if( !pTheora->IsSetupped() ) { return false; }
  }

  if( m_VorbisSerial!=DECODER_EMPTY )
  {
    BINDDATA::const_iterator ite = m_BindData.find(m_VorbisSerial);
    const DATA& dat = *(ite->second);

    boost::shared_ptr<OggDecoderVorbis> pVorbis = boost::static_pointer_cast<OggDecoderVorbis>(dat.pDecoder);
    if( !pVorbis->IsSetupped() ) { return false; }
  }

  return true;
}

bool Decoder::IsSeeking() const
{
  if( IsInitialized() ) { return false; }

  return m_State==STATE_SEEKING;
}


void Decoder::GetFileInfo( FILEINFO& Info ) const
{
  MAID_ASSERT( !IsInitialized(), "まだ初期化されていません" );

  Info.IsImage = false;
  Info.IsPCM = false;

  if( m_TheoraSerial!=DECODER_EMPTY )
  {
    Info.IsImage = true;
    BINDDATA::const_iterator ite = m_BindData.find(m_TheoraSerial);
    const DATA& dat = *(ite->second);

    const theora_info& th = boost::static_pointer_cast<OggDecoderTheora>(dat.pDecoder)->GetInfo();

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

    boost::shared_ptr<OggDecoderVorbis> pVorbis = boost::static_pointer_cast<OggDecoderVorbis>(dat.pDecoder);
    const vorbis_info& vi = pVorbis->GetInfo();

    FILEINFO::PCM& pcm = Info.Pcm;

    pcm.Format.SamplesPerSecond = vi.rate;
    pcm.Format.Channels = vi.channels;
    pcm.Format.BitPerSamples = 16;
  }
}

void Decoder::FlushImage( double time, SPOGGDECODEDBUFFERIMAGE& pOutput )
{
  if( !IsInitialized() ) { return ; }

  DATA& dat = *(m_BindData[m_TheoraSerial]);

  //  dat.DecodedInfo は時間が古い順に並んでいるので、それをtimeより新しいものを探す
  {
    ThreadMutexLocker lock( dat.Mutex );
    std::list<OGGDECODEDINFO>& InfoList = dat.DecodedInfo;

    while( true )
    {
      if( InfoList.empty() ) { break; }

      std::list<OGGDECODEDINFO>::iterator ite = InfoList.begin();

      if( time <= ite->BeginTime )
      {
        pOutput = boost::static_pointer_cast<OggDecodedBufferImage>(ite->pBuffer);
      }
      
      //  ついでに古いものは破棄
      InfoList.erase(ite);

      if( pOutput.get()!=NULL ) { break; }
    }
  }
}



void Decoder::FlushPCM( double time, Buffer& WaveData )
{
  if( !IsInitialized() ) { return ; }

  //  デコード後のデータが複数にあることがあるので、全部つなげる
  typedef std::list<SPOGGDECODEDBUFFERPCM> BUFFERLIST;
  BUFFERLIST bufferlist;

  double BeginTime=0;
  {
    //  とりあえずはデコードされてるやつを全部奪取
    DATA& dat = *(m_BindData[m_VorbisSerial]);
    ThreadMutexLocker lock( dat.Mutex );

    //  リストがないなら終了
    if( dat.DecodedInfo.empty() ){ return ; }

    std::list<OGGDECODEDINFO>& InfoList = dat.DecodedInfo;

    BeginTime = InfoList.begin()->BeginTime;

    for( std::list<OGGDECODEDINFO>::iterator ite=InfoList.begin();
                                              ite!=InfoList.end();
                                              ++ite )
    {
      //  指定した時間より、バッファが古い場合は破棄
      if( time < ite->BeginTime ) { continue; }

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


//  シーク命令が出ているか？
bool Decoder::IsSetSeek()const
{
  return m_SeekPosition>=0.0;
}


void Decoder::Seek( double time )
{
  m_SeekPosition = time;
  m_State = STATE_SEEKING;
  //  ここで設定したシーク状態は
  //  バッファが溜まるまで維持されます
}

void Decoder::InitializeStream( const String& FileName )
{
  m_Buffer.Initialize();

  //  oggコンテナになにがあるか(theora,vorbis等) を調べる
  //  theora,vorbis 以外はNULLにしちゃう
  //  ogg の構造上、各フォーマットデータは先頭にある。
  while( true )
  {
    if( m_Buffer.IsEnd() ) { break; }

    const OggPage& page = m_Buffer.GetCurrentPage();

    if( !page.IsBeginOfStream() ) { break; }

    const int serial = page.GetSerialNo();

    {
      boost::shared_ptr<DATA> pData( new DATA );
      pData->Stream.Initialize( serial );
      pData->Stream.PageIn( page );

      const OggPacket& packet = pData->Stream.GetCurrentPacket();
      ogg_packet& p = const_cast<ogg_packet&>(packet.Get());

      SPOGGDECODER pDec;

      {
        if( theora_packet_isheader(&p)==1 ) 
        {
          pDec.reset( new OggDecoderTheora );
          pDec->Initialize();
          m_TheoraSerial = serial;
        }
        ef( vorbis_synthesis_idheader(&p)==1 )
        {
          pDec.reset( new OggDecoderVorbis );
          pDec->Initialize();
          m_VorbisSerial = serial;
        }
        else { pDec.reset( new OggDecoderNULL ); }
      }

      //  最初のページは１パケットしかないのでここでループする必要はない
      pDec->PacketIn(packet);
      pData->pDecoder = pDec;
      m_BindData[serial] = pData;
    }

    m_Buffer.NextPage();
  }

}

void Decoder::FinalizeStream()
{
  for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
  {
    DATA& dat = *(ite->second);

    dat.pDecoder->Finalize();
    dat.Stream.Finalize();
  }

  m_TheoraSerial = DECODER_EMPTY;
  m_VorbisSerial = DECODER_EMPTY;
  m_BindData.clear();
  m_Buffer.Finalize();
}

void Decoder::SeekStream( double time, volatile Maid::ThreadController::BRIGEDATA& brige )
{
  bool IsSeeking = true;

  //  全デコーダーのシークが終わったかのチェック
  //  シリアル,シーク(true:した)
  typedef std::map<int,bool> SEEKCHECK;
  SEEKCHECK  SeekCheck;

  for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
  {
    SeekCheck[ite->first] = false;
  }


  while( IsSeeking )
  {
    if( brige.IsExit ) { return; }
    if( m_Buffer.IsEnd() ) { break; }

    {
      //  全デコーダの初期化が終わっていたら終了
      bool IsBreak = true;
      for( SEEKCHECK::iterator ite=SeekCheck.begin(); ite!=SeekCheck.end(); ++ite )
      {
        if( !ite->second ) { IsBreak = false; break; }
      }

      if( IsBreak ) { break; }
    }

    const OggPage& page = m_Buffer.GetCurrentPage();

    BINDDATA::iterator ite = m_BindData.find(page.GetSerialNo());

    DATA& dat = *(ite->second);

    IOggDecoder& decoder = *(dat.pDecoder);
    OggStream&   stream  = dat.Stream;

    stream.PageIn(page);

    while( true )
    {
      if( stream.IsPacketEmpty() ) { break; }
      const OggPacket& packet = stream.GetCurrentPacket();

      if( !decoder.IsSetupped() )
      {
        //  ストリームの初期化中なら、パケットを送るだけ
        decoder.PacketIn(packet);

        //  初期化が終わったら＝＝シーク開始の合図
        if( decoder.IsSetupped() )
        {
          //  でもすでに終わっているときもあるのでそのときはスルー
          if( decoder.IsDecoding() ) { decoder.BeginSeekMode(); }
          else { SeekCheck[stream.GetSerialNo()] = true; }
        }
      }else
      {
        const double begin = decoder.GetTime();

        decoder.PacketIn(packet);

        const double end = decoder.GetTime();

        if( begin <= time  )
        {
          if( !decoder.IsDecoding() || time < end )
          {
              //  シーク時間のフレームに入ったらシーク終了。
              //  ただしシーク時間が入っているフレームは破棄（とれる方法あるかな？）
              SeekCheck[stream.GetSerialNo()] = true;
              decoder.EndSeekMode();
          }
        }else
        {
          //  シーク終了後も続くようならバッファリングする
          OGGDECODEDINFO info;
          info.BeginTime = begin;
          info.EndTime   = end;
          info.pBuffer   = decoder.CreateDecodedBuffer();

          if( info.pBuffer.get()!=NULL )
          {
            dat.Mutex.Lock();
            dat.DecodedInfo.push_back(info);
            dat.Mutex.Unlock();
          }
        }
      }

      stream.NextPacket();
    }
    m_Buffer.NextPage();
  }



}

void Decoder::PlayDecode( double begintime, volatile Maid::ThreadController::BRIGEDATA& brige )
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

    const OggPage& page = m_Buffer.GetCurrentPage();

    BINDDATA::iterator ite = m_BindData.find(page.GetSerialNo());
    if( ite==m_BindData.end() )
    {
      m_Buffer.NextPage();
      continue;
    }

    DATA& dat = *(ite->second);

    IOggDecoder& decoder = *(dat.pDecoder);
    OggStream&   stream  = dat.Stream;

    stream.PageIn(page);

    while( true )
    {
      if( stream.IsPacketEmpty() ) { break; }
      if( IsSetSeek()  ) { break;; }

      const OggPacket& packet = stream.GetCurrentPacket();

      const double begin = decoder.GetTime();

      decoder.PacketIn(packet);

      const double end = decoder.GetTime();

      if( decoder.IsDecoding() )
      {
        OGGDECODEDINFO info;
        info.BeginTime = begin;
        info.EndTime   = end;
        info.pBuffer = decoder.CreateDecodedBuffer();

        if( info.pBuffer.get()!=NULL )
        {
          dat.Mutex.Lock();
          dat.DecodedInfo.push_back(info);
          dat.Mutex.Unlock();
        }
      }

      stream.NextPacket();
    }

    Sleep( brige );

    m_Buffer.NextPage();
  }

}


unt Decoder::ThreadFunction( volatile ThreadController::BRIGEDATA& brige )
{
  InitializeStream( String() );

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





int Decoder::Sleep( volatile ThreadController::BRIGEDATA& brige )
{
  while( true )
  {
    if( brige.IsExit ) { break; }
    if( IsSetSeek() ) { break; }

    //  すべてのストリームを調べてバッファがある程度あるなら、減るまで待機
    //  目安は１秒かな
    bool IsFull = true;

    for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
    {
      DATA& dat = *(ite->second);

      if( !dat.pDecoder->IsDecoding() ) { continue; }
      if( dat.DecodedInfo.empty() ) { continue; }

      double len = 0;
      {
        dat.Mutex.Lock();
        const double begin = dat.DecodedInfo.front().BeginTime;
        const double end = dat.DecodedInfo.back().EndTime;
        len = end - begin;
        dat.Mutex.Unlock();
      }
      if( len<1.0 ) 
      {
        IsFull = false; 
        break; 
      }
    }

    if( IsFull )
    {
      m_State = STATE_WORKING;
      ThreadController::Sleep(1);
    }else
    {
      break; 
    }
  }


  return 0;
}
