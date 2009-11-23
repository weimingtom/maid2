//#define LOCAL_PROFILE


#include"MoviePlayer.h"

#include"../../auxiliary/mathematics.h"
#include"../../auxiliary/debug/warning.h"
#include"../../auxiliary/debug/profile.h"


#include"movieplayerlocal.h"
#include"xiph/codectheora.h"
#include"xiph/codecvorbis.h"

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



namespace Maid {

//  ここの実装はデコードスレッドが使う関数を書いておきます
//  ゲーム側が使う関数は 1.cpp を参照



//  シーク命令が出ているか？
bool MoviePlayer::IsSetSeek()const
{
  //  シーク中は新しいシーク命令を受け付けないので、m_State!=STATE_SEEKING の判定は問題ない
  return 0.0 <= m_SeekPosition && m_State!=STATE_SEEKING;
}




unt MoviePlayer::ThreadFunction( volatile ThreadController::BRIGEDATA& brige )
{
  InitializeStream( m_FileName );

  while( true )
  {
    PlayDecode( brige );

    //  関数を出たということは、スレッドの終了か、シーク命令がでたことになる
    if( brige.IsExit ) { break; }

    //  スレッド終了じゃないので、シーク用に再構築する

    FinalizeStream();
    m_State = STATE_INITIALIZING;
    InitializeStream( m_FileName );
  }

  m_State = STATE_FINALIZING;
  FinalizeStream();
  m_State = STATE_EMPTY;

  return 0;
}




void MoviePlayer::InitializeStream( const String& FileName )
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
      Xiph::SPCODEC   pCodec;
      SPSTREAMDECODERMULTITHREAD  pDecoder;

      pStream->Initialize( serial );
      pStream->PageIn( page );

      Xiph::OggPacket packet;
      
      pStream->PacketOut( packet );

      {
        ogg_packet& p = const_cast<ogg_packet&>(packet.Get());
        if( theora_packet_isheader(&p)==1 ) 
        {
          boost::shared_ptr<Xiph::CodecTheora> pTheora( new Xiph::CodecTheora );
          pTheora->Initialize();
          pTheora->Setup(packet);

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

          pCodec = pTheora;
          pDecoder.reset( new StreamDecoderMultiThreadVideo(FRAMECOUNT) );
        }
        else if( vorbis_synthesis_idheader(&p)==1 )
        {
          boost::shared_ptr<Xiph::CodecVorbis> pVorbis( new Xiph::CodecVorbis );
          pVorbis->Initialize();
          pVorbis->Setup(packet);

          FILEINFO& Info = m_FileInfo;
          Info.IsPCM = true;

          const vorbis_info& vi = pVorbis->GetInfo();

          FILEINFO::PCM& pcm = Info.Pcm;

          pcm.Format.SamplesPerSecond = vi.rate;
          pcm.Format.Channels = vi.channels;
          pcm.Format.BitPerSamples = 16;

          m_VorbisSerial = serial;

          pCodec = pVorbis;
          pDecoder.reset( new StreamDecoderMultiThreadSound(SAOUNDTIME) );
        }
      }

      if( pDecoder.get()!=NULL )
      {
        //  最初のページは１パケットしかないのでここでループする必要はない
        pDecoder->Initialize( m_Timer, pStream, pCodec );
        m_BindData[serial] = pDecoder;
      }
    }
    m_Buffer.NextPage();
  }

  m_Timer.SetOffset(0);
}

void MoviePlayer::PlayDecode( volatile ThreadController::BRIGEDATA& brige )
{
  bool IsLoop = true;
  while( IsLoop )
  {
    if( brige.IsExit ) { return; }

    if( IsSetSeek() )
    {
      const double target = m_SeekPosition;
      m_State = STATE_SEEKING;
      m_Timer.Stop();
      const double now = m_Timer.Get();

      if( now <= target )
      { //  未来に飛ぶときは時間を変更するだけ
        for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
        {
          IStreamDecoderMultiThread& decoder = *(ite->second);
          decoder.BeginSeek(target);
        }
        m_Timer.SetOffset(m_SeekPosition);
        m_SeekPosition = -1;

      }else
      { //  過去に飛ぶときはいったん終了して、再度先頭からシークする
        break;
      }
    }

    bool IsCacheFull = true;
    //  ページ待ちしてるとこにページを送る
    //  キャッシュが埋まってるかも調べる
    for( BINDDATA::iterator ite=m_BindData.begin(); ite!=m_BindData.end(); ++ite )
    {
      IStreamDecoderMultiThread& decoder = *(ite->second);
      const bool NoPacket = decoder.IsPacketEmpty();
      const int PacketCount = decoder.GetPacketCount();
      const bool IsCache = decoder.IsCacheFull();
      const bool IsEnd = decoder.IsStreamEnd();

//      if( NoPacket ) { PageSeek(brige, ite->first); }
      if( PacketCount < 10 )
      {
        #pragma  COMPILERMSG( "溜まってるデコード前のパケットが合計何秒あるかで判定したほうがいい" )
        PageSeek(brige, ite->first);
      }
      if( IsCache || IsEnd ){}
      else{ IsCacheFull = false; }
    }

    //  キャッシュが全部いっぱいになっているならお休みする
    if( IsCacheFull )
    {
      if( m_State==STATE_SEEKING )
      { //  ここにきたってことはキャッシュがそろった＆シークが終わっている
        //  つまり再稼動
        m_Timer.Resume();
        m_SeekPosition = -1;
      }

      m_State = STATE_WORKING;
      ThreadController::Sleep(1);
    }
  }
}


//  指定したシリアルのストリームを充填する
void MoviePlayer::PageSeek( volatile ThreadController::BRIGEDATA& brige, int Serial )
{

MAID_PROFILE();

  while( true )
  {
    if( brige.IsExit ) { return ; }
    if( m_Buffer.IsEnd() ) { return; }

    while( true )
    {
      const Xiph::OggPage& page = m_Buffer.GetCurrentPage();

      const int PageSerial = page.GetSerialNo();

      BINDDATA::iterator ite = m_BindData.find( PageSerial );

      if( ite!=m_BindData.end() )
      {
        IStreamDecoderMultiThread& decoder = *(ite->second);
        //  ストリームが終わってたらいらないよな
        if( decoder.IsStreamEnd() ) { return ; }
        decoder.PageIn( page );
      }
      m_Buffer.NextPage();

      if( PageSerial==Serial && (0<page.GetPacketCount()) ) { return; }
    }
  }
}

bool MoviePlayer::IsCacheFull( int DecoderSerial ) const
{
  if( DecoderSerial==DECODER_EMPTY ) { return true; }

  BINDDATA::const_iterator ite = m_BindData.find(DecoderSerial);
  if( ite==m_BindData.end() ) { return true; }

  const IStreamDecoderMultiThread& decoder  = *(ite->second);

  if( decoder.IsCacheFull() ) { return true; }
  if( decoder.IsStreamEnd() ) { return true; }

  return false;
}

bool MoviePlayer::IsSeeking( int DecoderSerial )const
{
  if( DecoderSerial==DECODER_EMPTY ) { return false; }

  BINDDATA::const_iterator ite = m_BindData.find(DecoderSerial);
  if( ite==m_BindData.end() ) { return false; }

  const IStreamDecoderMultiThread& decoder  = *(ite->second);

  return decoder.IsSeeking();
}

bool MoviePlayer::IsEnd( int DecoderSerial )const
{
  if( DecoderSerial==DECODER_EMPTY ) { return true; }

  BINDDATA::const_iterator ite = m_BindData.find(DecoderSerial);
  if( ite==m_BindData.end() ) { return true; }

  const IStreamDecoderMultiThread& decoder  = *(ite->second);

  return decoder.IsStreamEnd();
}



void MoviePlayer::Sleep( volatile ThreadController::BRIGEDATA& brige )
{
  //  すべてのストリームを調べてバッファがそろっているなら、減るまで待機
  //  ただしシーク中の場合はやっぱり戻る(IsCacheFull内で判定してます)
  while( true )
  {
    if( brige.IsExit ) { break; }
    if( IsSetSeek() ) { break; }

    if( !IsCacheFull(m_TheoraSerial) ) { return ; }
    if( !IsCacheFull(m_VorbisSerial) ) { return ; }

    if( m_State==STATE_SEEKING )
    { //  ここにきたってことはキャッシュがそろった＆シークが終わっている
      //  つまり再稼動
      m_Timer.Resume();
      m_SeekPosition = -1;
    }

    m_State = STATE_WORKING;
    ThreadController::Sleep(1);
  }
}



void MoviePlayer::FinalizeStream()
{
  m_TheoraSerial = DECODER_EMPTY;
  m_VorbisSerial = DECODER_EMPTY;

  m_BindData.clear();
  m_Buffer.Finalize();
}


}